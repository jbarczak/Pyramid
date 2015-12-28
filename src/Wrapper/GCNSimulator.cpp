#pragma unmanaged
#include "GCNSimulator.h"
#include "GCNIsa.h"

#include <string.h>
#include <algorithm>
#include <assert.h>

namespace GCN{
namespace Simulator{

    namespace _INTERNAL
    {
        enum
        {
            MAX_WAVES_PER_THREADGROUP    = 16,
            NUM_SIMDS                    = 4,
            MAX_WAVES_PER_SIMD           = 10,
            MAX_WAVES_PER_CU             = NUM_SIMDS * MAX_WAVES_PER_SIMD,

            MAX_LCGMCNT                  = 31,
            MAX_VMCNT                    = 15,
            MAX_EXPCNT                   = 7,

            NUM_LDS_QUEUES               = 2
        };

     

        struct ThreadGroupState;

        struct WaveState
        {
            size_t nCurrentOp;
            size_t nStartClock;
            ThreadGroupState* pThreadGroup;
            
            uint8 nSIMD;
            uint8 lkgmcnt;
            uint8 vmcnt;
            uint8 expcnt;

            uint8 nWaveIDInGroup; ///< Index of this wave in its thread group
        };

        struct ThreadGroupState
        {
            uint16 nWaitMask;       ///< Bit-mask of waves presently waiting at a barrier
            uint8 nWaves;           ///< Number of waves not yet launched
            uint8 nLaunchedWaves;   ///< Number of running waves
            uint8 nRetiredWaves;    ///< Number of waves that have run to completion
        };


        template< class T, int MAX >
        class RingQueue
        {
        public:
            RingQueue() : m_nFirst(0), m_nLast(0){}

            bool empty() const { return m_nFirst == m_nLast; }

            T& front() { return m_Items[(m_nFirst) % MAX]; }

            void pop_front() { m_nFirst++; };

            void push_back( const T& op )
            {
                size_t nLoc = m_nLast++;
                m_Items[nLoc % MAX] = op;
                assert( m_nLast - m_nFirst <= MAX );
            }

            size_t size() const { return m_nLast - m_nFirst; }

            const T& at( size_t i ) const { return m_Items[ (m_nFirst+i)%MAX ]; }

        private:

            T m_Items[MAX];
            size_t m_nFirst;
            size_t m_nLast;
        };

      
        struct SMemOp
        {
            size_t nDWORDsLeft;
            WaveState* pWave;
        };
        struct VMemOp
        {
            size_t nClocksLeft;
            bool bIsExport;
            WaveState* pWave;
        };
        struct ExportOp
        {
            size_t nClocksLeft;
            WaveState* pWave;
        };
        struct DSOp
        {
            size_t nClocksLeft;
            WaveState* pWave;
        };

        // If your compiler lacks 'PopCount' then you can do something about it here
        static unsigned int PopCount( unsigned int n )
        {
            return __popcnt((unsigned int)n);
        }

        static bool IsBarrier( const Instruction* pOp )
        {
            if( pOp->GetClass() == IC_SCALAR )
            {
                const ScalarInstruction* pScalar = static_cast<const ScalarInstruction*>(pOp);
                if( pScalar->GetOpcode() == S_BARRIER )
                    return true;
            }
            return false;
        }

        
        /// Search for a threadgroup structure that isn't in use
        static ThreadGroupState* FindAvailableThreadGroup( ThreadGroupState pGroups[MAX_WAVES_PER_CU], size_t nOccupancyLimit )
        {
            for( size_t i=0; i<nOccupancyLimit; i++ )
            {
                if( !pGroups[i].nWaves )
                    return &pGroups[i];
            }
            return 0;
        }

        /// Search for a WaveState structure that isn't in use
        static WaveState* FindAvailableWave( WaveState pWaves[MAX_WAVES_PER_CU] )
        {
            for( size_t i=0; i<MAX_WAVES_PER_CU; i++ )
            {
                if( !pWaves[i].pThreadGroup )
                    return &pWaves[i];
            }
            return 0;
        }

        static WaveState* FindLDSToIssue( const SimOp* pOps, WaveState** ppWaves, size_t nWaves )
        {
            for( size_t i=0; i<nWaves; i++ )
            {
                // Throttle instruction issue of there are more scalars in flight
                //  than we have counter bits for
                //
                // CLARIFICATION NEEDED:  Is this what HW does?
                //
                if( ppWaves[i]->lkgmcnt == MAX_LCGMCNT )
                    continue;

                const GCN::Instruction* pOp = pOps[ppWaves[i]->nCurrentOp].pInstruction;
                if( pOp->GetClass() != GCN::IC_DS )
                    continue;

                const GCN::DataShareInstruction* pDS = static_cast<const GCN::DataShareInstruction*>(pOp);
                if( pDS->IsGDS() )
                    continue;

                return ppWaves[i];
            }

            return 0;
        }

        /// Search list of waves for oldest one that wants to issue a scalar
        static WaveState* FindScalarToIssue( const SimOp* pOps, WaveState** pWaves, size_t nWaves )
        {
            WaveState* pIssueWave=0;

            // look for a wave which has a scalar op, where said scalar op is not
            // one of the freebies
            for( size_t i=0; i<nWaves; i++ )
            {
                const GCN::Instruction* pOp = pOps[pWaves[i]->nCurrentOp].pInstruction;
                if( pOp->GetClass() != GCN::IC_SCALAR_MEM && pOp->GetClass() != GCN::IC_SCALAR )
                    continue;

                // Throttle instruction issue of there are more scalars in flight
                //  than we have counter bits for
                //
                // CLARIFICATION NEEDED:  Is this what HW does?
                //
                if( pOp->GetClass() == GCN::IC_SCALAR_MEM && pWaves[i]->lkgmcnt == MAX_LCGMCNT )
                    continue;

                if( pOp->GetClass() == IC_SCALAR )
                {
                    const GCN::ScalarInstruction* pScalar = static_cast<const GCN::ScalarInstruction*>( pOp );
                    switch( pScalar->GetOpcode() )
                    {
                    case S_NOP           :         
                    case S_ENDPGM        :        
                    case S_BARRIER       :        
                    case S_SETHALT       :        
                    case S_SLEEP         :        
                    case S_SETPRIO       :        
                    case S_ENDPGM_SAVED  : 
                    case S_WAITCNT       : 
                        continue;
                    }
                }

                // We can issue this one.  
                //  but give priority to older waves
                if( !pIssueWave || pIssueWave->nStartClock > pWaves[i]->nStartClock )
                    pIssueWave = pWaves[i];
            }

            return pIssueWave;
        }

        /// Search list of waves for oldest one that wants to issue a valu
        static WaveState* FindVALUToIssue( const SimOp* pOps, WaveState** pWaves, size_t nWaves )
        {
            WaveState* pIssueWave=0;

            // look for a wave which has a VALU op to issue
            for( size_t i=0; i<nWaves; i++ )
            {
         
                // CLARIFICATION NEEDED
                //   Do VECTOR_INTERP instructions need any special handling?
                //   They interact with the LDS, but does that have any extra cost?
                const GCN::Instruction* pOp = pOps[pWaves[i]->nCurrentOp].pInstruction;
                if( pOp->GetClass() != GCN::IC_VECTOR &&
                    pOp->GetClass() != GCN::IC_VECTOR_INTERP)
                    continue;

                // We can issue this one.  
                //  but give priority to older waves
                if( !pIssueWave || pIssueWave->nStartClock > pWaves[i]->nStartClock )
                    pIssueWave = pWaves[i];
            }

            return pIssueWave;
        }

        /// Is instruction a vector memory write?
        static bool IsVMEMWrite( const GCN::Instruction* pOp )
        {
            switch( pOp->GetClass() )
            {
            default:
                return false;

            case GCN::IC_IMAGE:
                {
                    const GCN::ImageInstruction* pImg = static_cast<const GCN::ImageInstruction*>(pOp);
                    return pImg->IsMemoryWrite();
                }
                break;
                     
            case GCN::IC_BUFFER:
                {
                    const GCN::BufferInstruction* pImg = static_cast<const GCN::BufferInstruction*>(pOp);
                    return pImg->IsMemoryWrite();
                }
                break;
            }
        }

        /// Search a list of waves for oldest one that wants to issue a vmem
        static WaveState* FindVMEMToIssue( const SimOp* pOps, WaveState** pWaves, size_t nWaves )
        {
            WaveState* pIssueWave=0;

            // look for a wave which has a VALU op to issue
            for( size_t i=0; i<nWaves; i++ )
            {
                const GCN::Instruction* pOp = pOps[pWaves[i]->nCurrentOp].pInstruction;
                if( pOp->GetClass() != GCN::IC_BUFFER &&
                    pOp->GetClass() != GCN::IC_IMAGE )
                    continue;

                
                // Throttle instruction issue of there are more OPS in flight
                //  than we have counter bits for
                //
                // CLARIFICATION NEEDED:  Is this what HW does?
                //
                if( pWaves[i]->vmcnt == MAX_VMCNT ) 
                    continue;
                if( IsVMEMWrite(pOp) && pWaves[i]->expcnt == MAX_EXPCNT )
                    continue;

                // We can issue this one.  
                //  but give priority to older waves
                if( !pIssueWave || pIssueWave->nStartClock > pWaves[i]->nStartClock )
                    pIssueWave = pWaves[i];
            }

            return pIssueWave;
        }

        /// Search a list of waves for oldest one that wants to issue an export
        static WaveState* FindExportToIssue( const SimOp* pOps, WaveState** pWaves, size_t nWaves )
        {
            WaveState* pIssueWave=0;

            // look for a wave which has an export to issue
            for( size_t i=0; i<nWaves; i++ )
            {
           
                const GCN::Instruction* pOp = pOps[pWaves[i]->nCurrentOp].pInstruction;
                if( pOp->GetClass() != GCN::IC_EXPORT )
                    continue;

                 
                // Throttle instruction issue of there are more OPS in flight
                //  than we have counter bits for
                //
                // CLARIFICATION NEEDED:  Is this what HW does?
                //
                if( pWaves[i]->expcnt == MAX_EXPCNT )
                    continue;

                // We can issue this one.  
                //  but give priority to older waves
                if( !pIssueWave || pIssueWave->nStartClock > pWaves[i]->nStartClock )
                    pIssueWave = pWaves[i];
            }

            return pIssueWave;
        }

      

        /// Determine the cost in cycles of a VMEM instruction,
        ///  Taking format and filtering into account
        static size_t GetVMEMCost( const SimOp* pSimOp )
        {
            const GCN::Instruction* pOp = pSimOp->pInstruction;
            switch( pOp->GetClass() )
            {
            default:
                return 4;

            case GCN::IC_BUFFER:
                {
                    // 16 "load store units" --> 4 clocks/wave/dword
                    const GCN::BufferInstruction* pBuff = static_cast<const GCN::BufferInstruction*>(pOp);
                    return pBuff->GetResultWidthInDWORDS()*4;
                }
                break;

            case GCN::IC_IMAGE:
                {                    
                    const GCN::ImageInstruction* pImg = static_cast<const GCN::ImageInstruction*>(pOp);
                    if( pImg->IsFilteredFetch() )
                    {
                        
                        size_t nBaseCost=0;

                        // perf depends on format and filter type
                        if( pSimOp->eFilter == FILT_POINT )
                            nBaseCost = 16; // point sampling is 4 pix/clk for all formats
                        else
                        {
                            // baseline for bilinear is 4 pixels/clk
                            size_t nCost = 0;
                            switch( pSimOp->eFormat )
                            {
                            case FMT_RGBA32F:
                                nCost = 64; // 1/4 rate
                                break;
                            case FMT_RGBA16:
                            case FMT_RGBA16F:
                            case FMT_RG32F:
                            case FMT_BC6:
                                nCost = 32; // 1/2 rate
                                break;
                            default:
                                nCost = 16;
                                break;
                            }

                            // modify by filter type
                            switch( pSimOp->eFilter )
                            {
                            case FILT_TRILINEAR:
                            case FILT_ANISO_2X:
                                nCost *= 2;
                                break;
                            case FILT_ANISO_4X:
                                nCost *= 4;
                                break;
                            case FILT_ANISO_8X:
                                nCost *= 8;
                                break;
                            }

                            nBaseCost= nCost;
                        }

                        // APPROXIMATION: Gradient fetches are approximately 2x cost
                        //
                        //  It's actually full rate for 4 dwords/thread of input
                        //     and an extra clock for each additional 4
                        //
                        //  This 2x assumes a 2D gradient fetch where derivatives
                        //     can all go in one extra clock
                        //  3D is more expensive, 1D possibly less
                        //  Unfortunately ISA doesn't tell us 
                        //   what tx type we're dealing with
                        //
                        //  Not clear if a thread can overlap extra argument cost
                        //    with expensive filtering and have these cancel out
                        //
                        if( pImg->IsGradientFetch() )
                            nBaseCost *= 2;
                        
                        return nBaseCost;
                    }
                    else if( pImg->IsGather() )
                    {
                        // gather is just like a point-sampled fetch.  4x4 pix/clk
                        return 16;
                    }
                    else
                    {
                        // AMD says that Image load/store gets capped at 4 pixels/clk
                        //   due to more complicated addressing
                        return 16;
                    }
                }
                break;
            }
        }


        /// Determine the cost in cycles of a vector instruction
        static size_t GetVALUCost( const GCN::Instruction* pOp )
        {
            if( pOp->GetClass() == GCN::IC_VECTOR_INTERP )
            {
                // CLARIFICATION NEEDED:
                //  Assuming that interps are as cheap as regular VALU.  Is this correct?
                return 4; 
            }
            else
            {
                const GCN::VectorInstruction* pVALU = static_cast<const GCN::VectorInstruction*>( pOp );

                // Costs based on humus's presentation, here:
                //    http://www.humus.name/Articles/Persson_LowlevelShaderOptimization.pdf
                //
                // CLARIFICAITON NEEDED:
                //  What did I miss?
                switch( pVALU->GetOpcode() )
                {
                    // anything not expressly called out below is 4 clocks
                default:
                    return 4;

                // Int32 multiply:  1/4 rate
                case V_MUL_LO_U32:   
                case V_MUL_HI_U32:   
                case V_MUL_LO_I32:   
                case V_MUL_HI_I32:   
                    return 16;

                    // float transcendentals: 1/4 rate
                case V_EXP_F32             :
                case V_LOG_CLAMP_F32       :
                case V_LOG_F32             :
                case V_RCP_CLAMP_F32       :
                case V_RCP_LEGACY_F32      :
                case V_RCP_F32             :
                case V_RCP_IFLAG_F32       :
                case V_RSQ_CLAMP_F32       :
                case V_RSQ_LEGACY_F32      :
                case V_RSQ_F32             :
                case V_SQRT_F32            :
                case V_SIN_F32             :
                case V_COS_F32             :
                case V_LOG_LEGACY_F32      :
                case V_EXP_LEGACY_F32      :
                case V_RCP_F16             :
                case V_SQRT_F16            :
                case V_RSQ_F16             :
                case V_LOG_F16             :
                case V_EXP_F16             :
                case V_SIN_F16             :
                case V_COS_F16             :
                case V_DIV_SCALE_F32       : 
                case V_DIV_FIXUP_F32       : // Clarification needed
                case V_DIV_FMAS_F32        : //   Do these divs count as transcendentals? 
                    return 16;

                // DP add:  
                case V_ADD_F64 :
                    return 8;

                // DP mul/mad
                case V_FMA_F64 :       
                case V_MUL_F64 : 
                    return 16;

                // CLARIFICATION NEEDED
                //   Are DP transcendentals 1/2 float rate?  Can't find a source.
                case V_RCP_F64          :
                case V_RCP_CLAMP_F64    :
                case V_RSQ_F64          :
                case V_RSQ_CLAMP_F64    :
                case V_SQRT_F64         :
                case V_DIV_SCALE_F64    :
                case V_DIV_FIXUP_F64    :
                case V_DIV_FMAS_F64     :
                    return 32;
                }
            }
        }

        static uint GetExportCost( const GCN::Instruction* pOp, uint nBaseCost )
        {
            const GCN::ExportInstruction* pExp = static_cast<const GCN::ExportInstruction*>( pOp );
            if( pExp->GetCompressBit() )
                return nBaseCost;
            else
                return nBaseCost*2;
        }


        /// Test whether a given wave can issue one of the "free" scalar operations
        static bool CanIssueFreeScalarOp( const GCN::Instruction* pOp, WaveState* pWave )
        {
            if( pOp->GetClass() != GCN::IC_SCALAR )
                return false;

            const GCN::ScalarInstruction* pScalar = static_cast<const GCN::ScalarInstruction*>( pOp );
            
            switch( pScalar->GetOpcode() )
            {
            case S_NOP           :         
            case S_SETHALT       :        
            case S_SLEEP         :        
            case S_SETPRIO       :
                return true;

                // don't move past a barrier until the wave's corresponding bit in the wait mask
                //  has been cleared.  This happens during the barrier processing phase, which 
                //  comes before this point in the sim cycle
            case S_BARRIER:
                {
                    size_t nWaveBit = 1 << pWave->nWaveIDInGroup;
                    if( pWave->pThreadGroup->nWaitMask & nWaveBit )
                        return false;
                    
                    return true;
                }
                break;

                // don't issue endpgm until all the counters drop to zero
            case S_ENDPGM        :        
            case S_ENDPGM_SAVED  : 
                if( pWave->lkgmcnt == 0 &&
                    pWave->expcnt == 0 &&
                    pWave->vmcnt == 0 )
                {
                    return true;
                }
                break;
            case S_WAITCNT       :        
                {
                    if( pWave->lkgmcnt <= pScalar->GetLCGMWaitCount() &&
                        pWave->expcnt <= pScalar->GetEXPWaitCount() &&
                        pWave->vmcnt <= pScalar->GetVMwaitCount() )
                    {
                        return true;
                    }
                }
                break;
            }

            return false;
        }
    }

    void Simulate( Results& rResults, const Settings& rSettings, SimOp* pOps, size_t nOps )
    {
        using namespace _INTERNAL;

        size_t nClock        = 0;
        size_t nRetiredWaves = 0;
        size_t nIssuedWaves  = 0;
        size_t nGroupsInFlight = 0;
        size_t nWavesToExecute = rSettings.nWavesPerThreadGroup*rSettings.nGroupsToExecute;

        ThreadGroupState pThreadGroups[MAX_WAVES_PER_CU] = {0};
        WaveState pWaveFronts[MAX_WAVES_PER_CU] = {0};
        WaveState* pWavesInFlight[NUM_SIMDS][MAX_WAVES_PER_SIMD] = {0};
        uint8 pWaveCount[NUM_SIMDS] = {0};

        ThreadGroupState* pIssuingThreadGroup=0;
        WaveState* pWavesInVALU[NUM_SIMDS] = {0}; ///< Wave currently occupying each VALU
        size_t pVALUCycles[NUM_SIMDS] = {0};      ///< Number of cycles remaining on each VALU op
    
        RingQueue<SMemOp,MAX_WAVES_PER_CU*32>   SMemQueue;
        RingQueue<VMemOp,MAX_WAVES_PER_CU*16>   VMemQueue;
        RingQueue<ExportOp,MAX_WAVES_PER_CU*16> EXPQueue;

        // LDS/SIMD interface looks like this:
        //
        //   SIMD 0/1          SIMD 2/3
        //    16 lanes         16 lanes
        //
        //  However, each "lane" is two dwords wide (so 32 dwords/wave)
        //   At peak, two waves can read two DWORDS/thread over four clocks
        //
        RingQueue<DSOp,MAX_WAVES_PER_CU*32>     LDSQueue[NUM_LDS_QUEUES];


        while(1)
        {
            // Issue new wavefront if its time
            if( nIssuedWaves < nWavesToExecute )
            {
                if( !pIssuingThreadGroup && (nClock % rSettings.nGroupIssueRate) == 0 )
                {
                    // start issuing a new thread group after the specified time interval has elapsed
                    //  This is to model the effect of fixed-function units (ACE, VGT, Rasterizer, etc)
                    //   round-robining waves across available CUs
                    
                    pIssuingThreadGroup = FindAvailableThreadGroup(pThreadGroups,rSettings.nMaxGroupsPerCU);
                    if( pIssuingThreadGroup )
                    {
                        pIssuingThreadGroup->nWaves = rSettings.nWavesPerThreadGroup;
                        pIssuingThreadGroup->nLaunchedWaves = 0;
                        pIssuingThreadGroup->nRetiredWaves = 0;
                        pIssuingThreadGroup->nWaitMask = 0;
                        nGroupsInFlight++;
                    }
                }
                
                // If we have a thread group to issue waves from, then issue from it each and every cycle
                //   Layla slides state that the ACE's an issue one wave per clock to the CUs.  We'll assume
                //   that the scheduler shoots a wave to the current CU every clock until its done, then moves on ot the next one
                if( pIssuingThreadGroup )
                {
                    WaveState* pWave = FindAvailableWave( pWaveFronts );
                    if( !pWave )
                    {
                        // means that the CU is completely full.  Good for us.
                        // TODO: Track this...?
                    }
                    else
                    {
                        // CLARIFICATION NEEDED:
                        // I don't know precisely what the rules are for mapping newly issued waves to SIMDs
                        //   let's assume simple round-robin until somebody tells us different
                        size_t nSIMD = nIssuedWaves % NUM_SIMDS;
                        if( pWaveCount[nSIMD] < rSettings.nMaxWavesPerSIMD )
                        {
                            pWavesInFlight[nSIMD][pWaveCount[nSIMD]++] = pWave;
                            pWave->nSIMD       = nSIMD;
                            pWave->nStartClock = nClock;
                            pWave->nCurrentOp  = 0;
                            pWave->pThreadGroup = pIssuingThreadGroup;
                            pWave->nWaveIDInGroup = pIssuingThreadGroup->nLaunchedWaves;
                            nIssuedWaves++;

                            pIssuingThreadGroup->nLaunchedWaves++;
                            if( pIssuingThreadGroup->nLaunchedWaves == pIssuingThreadGroup->nWaves )
                                pIssuingThreadGroup = NULL;
                            
                        }
                    }
                }
            }

            // Tick the LDS pipe
            
            // Count as busy if either of the two Queues are running
            for( size_t i=0; i<NUM_LDS_QUEUES; i++ )
            {
                if( !LDSQueue[i].empty() )
                {
                    DSOp& op = LDSQueue[i].front();
                    op.nClocksLeft--;
                    if( op.nClocksLeft == 0 )
                    {
                        op.pWave->lkgmcnt--;
                        LDSQueue[i].pop_front();
                    }

                    rResults.nLDSBusy++;
                }
            }
           

            // tick the vmem pipe
            if( !VMemQueue.empty() )
            {
                VMemOp& op = VMemQueue.front();
                op.nClocksLeft--;
                if( op.nClocksLeft == 0 )
                {
                    op.pWave->vmcnt--;
                    if( op.bIsExport )
                        op.pWave->expcnt--;

                    VMemQueue.pop_front();
                }

                rResults.nVMemBusy++;
            }

            // Tick the SMem pipe
            //    SMem rate is 4 dwords/CU/clk
            //
            // CLARIFICATION NEEDED:
            //  For simplicity, assume here that we only retire one instruction per clock
            //     but I don't know if the hardware is smarter than that.  They have 16 bytes/clk
            //     of bandwidth, and they might be able to parcel it out to multiple waiting waves.
            if( !SMemQueue.empty() )
            {
                SMemOp& op = SMemQueue.front();
                if( op.nDWORDsLeft <= 4 )
                {
                    op.pWave->lkgmcnt--;
                    SMemQueue.pop_front();
                }
                else
                {
                    op.nDWORDsLeft -= 4;
                }

                rResults.nSMemBusy++;
            }

            // Tick the EXP pipe
            if( !EXPQueue.empty() )
            {
                ExportOp& exp = EXPQueue.front();
                exp.nClocksLeft--;
                if( exp.nClocksLeft == 0 )
                {
                    exp.pWave->expcnt--;
                    EXPQueue.pop_front();
                }

                rResults.nExpBusy++;
            }

           
            // tick all the VALUs
            for( size_t i=0; i<NUM_SIMDS; i++ )
            {
                if( pVALUCycles[i] )
                {
                    rResults.nVALUBusy[i]++;
                    pVALUCycles[i]--;
                    if( pVALUCycles[i] == 0 )
                    {
                        pWavesInVALU[i]->nCurrentOp++;
                        pWavesInVALU[i]=0;
                    }
                }
            }

            size_t nCurrentSIMD =  nClock % NUM_SIMDS;
            WaveState** pSIMDWaves = pWavesInFlight[nCurrentSIMD];
            size_t nWaves = pWaveCount[nCurrentSIMD];

            // Try and issue an LDS
            WaveState* pLDSWave = FindLDSToIssue( pOps, pSIMDWaves, nWaves );
            if( pLDSWave )
            {
                // TODO: Assuming best-case latency of 4 clocks.  What abount Bank conflicts?  
                //  How to model this in a way that's useful?
                DSOp ds;
                ds.nClocksLeft = 4;
                ds.pWave = pLDSWave;
                LDSQueue[nCurrentSIMD/2].push_back( ds );
                pLDSWave->lkgmcnt++;
            }

            // Try and issue a scalar
            WaveState* pScalarWave = FindScalarToIssue( pOps, pSIMDWaves, nWaves );
            if( pScalarWave )
            {
                const SimOp* pOp = &pOps[pScalarWave->nCurrentOp];
                const GCN::Instruction* pInst = pOp->pInstruction;
                if( pInst->GetClass() == IC_SCALAR_MEM )
                {
                    // NOTE: Ignoring the possibility the the counters might overflow
                    //    If that happens, the simulator will be unsable
                    //    but I'm assuming I won't be asked to simulate incorrect code
                    //
                    pScalarWave->lkgmcnt++;

                    const ScalarMemoryInstruction* pSMem = static_cast<const ScalarMemoryInstruction*>(pInst);
                    SMemOp op;
                    op.nDWORDsLeft = pSMem->GetResultWidthInDWORDs();
                    op.pWave = pScalarWave;
                    SMemQueue.push_back(op);
                }
                else
                {
                    // assume SALU is always free and finishes instantly
                    rResults.nSALUBusy++;
                }
            }

            // Try and issue a valu if the ALU is not busy
            //  ALU can still be busy if it is stuck doing a long-latency op like an rsq
            WaveState* pVALUWave = 0;
            if( pWavesInVALU[nCurrentSIMD] == 0 )
            {
                pVALUWave    = FindVALUToIssue( pOps, pSIMDWaves, nWaves );
                if( pVALUWave )
                {
                    size_t nCost = GetVALUCost( pOps[pVALUWave->nCurrentOp].pInstruction );
                    pVALUCycles[nCurrentSIMD]   = nCost;
                    pWavesInVALU[nCurrentSIMD]  = pVALUWave;
                }
            }

            // Try and issue a vmem
            WaveState* pVMemWave = FindVMEMToIssue( pOps, pSIMDWaves, nWaves );
            if( pVMemWave )
            {
                VMemOp op;
                op.nClocksLeft = GetVMEMCost( &pOps[pVMemWave->nCurrentOp] );
                op.pWave = pVMemWave;
                op.bIsExport = IsVMEMWrite( pOps[pVMemWave->nCurrentOp].pInstruction );

                if( op.bIsExport )
                    pVMemWave->expcnt++;
               
                pVMemWave->vmcnt++;
                VMemQueue.push_back( op );
            }

            // Try and issue an export
            WaveState* pExpWave = FindExportToIssue( pOps, pSIMDWaves, nWaves );
            if( pExpWave )
            {
                ExportOp op;
                op.nClocksLeft = GetExportCost( pOps[pExpWave->nCurrentOp].pInstruction, rSettings.nExportCost );
                
                op.pWave = pExpWave;
                pExpWave->expcnt++;
                EXPQueue.push_back(op);
            }

            // Barrier pre-processing
            //    - Mark all waves on this SIMD which have reached a barrier instruction
            //       When all threads reach the barrier, their IPs will be advanced
            //         during the "Free Scalar" phase which follows
            //
            //     This scheme is safe, because a released thread on one SIMD will execute
            //        at most one other instruction before all the remaining threads have been released
            //
            //      In the case of two S_BARRIER's back to back, the
            //         use of bit-masks should ensure that we do the right thing
            //         because each thread will advance only if ITs mask is cleared
            //
            for( size_t i=0; i<nWaves; i++ )
            {
                WaveState* pWave = pSIMDWaves[i];
                ThreadGroupState* pGroup = pWave->pThreadGroup;
                
                const SimOp* pOp = &pOps[pWave->nCurrentOp];
                if( IsBarrier(pOp->pInstruction) )
                {
                    // add this wave to the list of waiting waves
                    size_t nWaitMask = pGroup->nWaitMask;
                    nWaitMask |= (1<<pWave->nWaveIDInGroup);

                    // release waiting waves once all waves' bits have been added to the mask
                    size_t nGroupWaves = (pWave->pThreadGroup->nWaves - pWave->pThreadGroup->nRetiredWaves);
                    if( PopCount(nWaitMask) == nGroupWaves )
                        pGroup->nWaitMask=0;
                    else
                        pGroup->nWaitMask = (uint16)nWaitMask;
                }
            }
            
            // Issue "helper" instructions for all waves that can issue them
            //    nop, sleep, wait, barrier, setprio, s_endpgm
            size_t nFreeOps=0;
            for( size_t i=0; i<nWaves; i++ )
            {
                WaveState* pWave = pSIMDWaves[i];
                
                const SimOp* pOp = &pOps[pWave->nCurrentOp];
                if( CanIssueFreeScalarOp(pOp->pInstruction, pWave) )
                {
                    nFreeOps++;
                    pWave->nCurrentOp++;
                }
            }



            // retire any waves that have finished their trace
            for( size_t i=0; i<MAX_WAVES_PER_CU; i++ )
            {
                if( pWaveFronts[i].pThreadGroup &&
                    pWaveFronts[i].nCurrentOp == nOps )
                {
                    pWaveFronts[i].pThreadGroup->nRetiredWaves++;
                    if( pWaveFronts[i].pThreadGroup->nRetiredWaves == pWaveFronts[i].pThreadGroup->nWaves )
                    {
                        // retire thread groups for which all waves are done
                        memset(pWaveFronts[i].pThreadGroup, 0, sizeof(*(pWaveFronts[i].pThreadGroup)));
                        nGroupsInFlight--;
                    }

                    pWaveFronts[i].pThreadGroup = 0;
                    nRetiredWaves++;
                }
            }

            // purge any retired waves from the in-flight lists
            for( size_t i=0; i<NUM_SIMDS; i++ )
            {
                size_t nLive=0;
                size_t nWaves=pWaveCount[i];
                for( size_t w=0; w<nWaves; w++ )
                    if( pWavesInFlight[i][w]->pThreadGroup )
                        pWavesInFlight[i][nLive++] = pWavesInFlight[i][w];

                pWaveCount[i] = nLive;
            }

            nClock++;

            if( nRetiredWaves == nWavesToExecute )
                break;

            // track peak occupancy
            size_t nWaveOcc=0;
            for( size_t i=0; i<NUM_SIMDS; i++ )
                nWaveOcc += pWaveCount[i];
            if(nWaveOcc > rResults.nPeakWaveOccupancy)
                rResults.nPeakWaveOccupancy = nWaveOcc;
            if( nWaveOcc == 0 )
                rResults.nStarveCycles++;

            if( nGroupsInFlight > rResults.nPeakGroupOccupancy )
                rResults.nPeakGroupOccupancy = nGroupsInFlight;

            
            if( pLDSWave )
            {
                rResults.nLDSIssued++;
                pLDSWave->nCurrentOp++;
            }

            if( pScalarWave )
            {
                if( pOps[pScalarWave->nCurrentOp].pInstruction->GetClass() == IC_SCALAR_MEM )
                    rResults.nSMEMIssued++;
                else
                    rResults.nSALUIssued++;

                pScalarWave->nCurrentOp++;
            }

            // VALU waves don't get their IP incremented
            //   until after the op completes.  This is done to ensure that the wave
            //   blocks itself if it's doing a long-latency operation
            if( pVALUWave )
            {
                rResults.nVALUIssued++;
            }

            if( pVMemWave )
            {
                pVMemWave->nCurrentOp++;
                rResults.nVMemIssued++;
            }
            if( pExpWave )
            {
                pExpWave->nCurrentOp++;
                rResults.nExpIssued++;
            }

            // check for clocks where nothing happens and track them
            //  Don't count as a stall if VALU is occupied by a long-latency op
            bool bVALUStarved = !pVALUWave && !pWavesInVALU[nCurrentSIMD];

            if( !pScalarWave && bVALUStarved && !pVMemWave && !pExpWave && !pLDSWave && !nFreeOps && pWaveCount[nCurrentSIMD] ) 
            {
                rResults.nStallCycles[nCurrentSIMD]++;

                // find the set of distinct SimOps on which we're stalled
                size_t pDistinctSimOps[MAX_WAVES_PER_SIMD];
                size_t nDistinctSimOps=0;
                for( size_t w=0; w<pWaveCount[nCurrentSIMD]; w++ )
                    pDistinctSimOps[nDistinctSimOps++] = pSIMDWaves[w]->nCurrentOp;
                
                // de-duplicate, and increment stall count on each stalled SimOp
                std::sort( pDistinctSimOps, pDistinctSimOps+nDistinctSimOps );
                for( size_t i=0; i<nDistinctSimOps; )
                {
                    size_t i0 = i;
                    rResults.pSimOpStallCounts[pDistinctSimOps[i0]];
                    
                    do
                    {
                        i++;
                    } while( i<nDistinctSimOps && pDistinctSimOps[i] == pDistinctSimOps[i0] );
                }

                // do the same for instructions
                size_t pStallInstructions[MAX_WAVES_PER_SIMD];
                size_t nInstructions=0;
                for( size_t i=0; i<nDistinctSimOps; i++ )
                    pStallInstructions[nInstructions++] = pOps[pDistinctSimOps[i]].nInstructionID;

                std::sort( pStallInstructions, pStallInstructions+nInstructions );
                for( size_t i=0; i<nInstructions; )
                {
                    size_t i0 = i;
                    rResults.pInstructionStallCounts[pStallInstructions[i0]]++;                    
                    do
                    {
                        i++;
                    } while( i<nOps && pStallInstructions[i] == pStallInstructions[i0] );
                }
            }
        } 

        rResults.nCycles = nClock;
    }

}}