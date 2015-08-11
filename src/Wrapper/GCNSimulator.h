#ifndef _GCN_SIMULATOR_H_
#define _GCN_SIMULATOR_H_

namespace GCN{
    
    class Instruction;

namespace Simulator{

    struct Settings
    {
        size_t nWaveIssueRate;      ///< Issue one wave every N clocks
        size_t nWavesToExecute;     ///< Execute this many waves before stopping
        size_t nMaxWavesPerSIMD;    ///< Occupancy limit per SIMD
        size_t nExportCost;         ///< How long an export should take
                                    ///<  This is presumed to be a function of the number of CUs to a slice

    };

    struct Results
    {
        ///< Count the number of clocks where at least one wave was stalled on a given instruction
        ///<  Indexed by the InstructionID field in the 'SimOp'
        ///< Caller must allocate and zero before simulating
        size_t* pInstructionStallCounts; 

        ///< Number of times that at least one wave was stalled on each SimOp
        ///<   Index space is the same as the SimOp array passed to the simulator
        ///< Each op's counter is Incremented once per stalled clock/stalled wave  
        size_t* pSimOpStallCounts;

        ///< Caller must zero all these fields before simulating
        size_t nCycles;
        size_t nSALUBusy;
        size_t nVALUBusy[4];
        size_t nVMemBusy;
        size_t nExpBusy;
        size_t nSMemBusy;

        size_t nPeakOccupancy;

        size_t nStallCycles[4];    ///< Number of times each SIMD was stalled
        size_t nStarveCycles; ///< Number of clocks during which CU is completely empty
    };


    enum Format
    {
        FMT_R8,
        FMT_RG8,
        FMT_RGBA8,
        FMT_R16,
        FMT_RG16,
        FMT_RGBA16,
        FMT_R16F,
        FMT_RG16F,
        FMT_RGBA16F,
        FMT_R32F,
        FMT_RG32F,
        FMT_RGBA32F,
        FMT_BC1,
        FMT_BC2,
        FMT_BC3,
        FMT_BC4,
        FMT_BC5,
        FMT_BC6,
        FMT_BC7
    };

    enum Filter
    {
        FILT_POINT,
        FILT_BILINEAR,
        FILT_TRILINEAR,
        FILT_ANISO_2X,
        FILT_ANISO_4X,
        FILT_ANISO_8X
    };



    struct SimOp
    {
        const GCN::Instruction* pInstruction;
        size_t nInstructionID;  ///< Index of the instruction in the results.pInstructionStallCounts

        // These are used only if 'pInstruction' is a corresponding image instruction
        Format eFormat;
        Filter eFilter;

    };


    /// Simulate wavefront execution on a single CU
    /// 
    void Simulate( Results& rResults, const Settings& rSettings, SimOp* pOps, size_t nOps );

}};


#endif