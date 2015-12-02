 

#define THREADGROUP_WIDTH 160 // 1 SIMD16 thread per EU 

cbuffer globals
{
    row_major float4x4 mViewProj;
    float2 g_vResolution; 
};
cbuffer drawcall
{
    row_major float4x4 mWorld;
    uint g_nTriangles;
};

// Public service announcement.  Raw buffers are SIGNIFICANTLY more efficient than TBuffers
//  Buffer<xxx> and friends.   Because of stupid DX constraint that R32G32B32 views cannot be used on VB/IB
//   it is necessary to load single elements from these.  
//  Using ByteAddressBuffer instead gives me a sizable boost because we can load multiple dwords at once

ByteAddressBuffer g_Verts;
ByteAddressBuffer g_Indices; 

RWTexture2D<uint> g_ZBuffer : register(u0);


[numthreads(THREADGROUP_WIDTH,1,1)]
void main( uint3 ThreadID      : SV_DispatchThreadID,
            uint3 GroupID       : SV_GroupID,
            uint3 GroupThreadID : SV_GroupThreadID )
{ 
    uint tid        = ThreadID.x;

    [branch]
    if( tid < g_nTriangles )
    {
        // fetch verts
        //   NOTE: No Vertex re-use.   Possible to add some?
        uint3 Indices = g_Indices.Load3( 12*tid );
        float3 P0 = asfloat( g_Verts.Load3( 12*Indices.x ) );
        float3 P1 = asfloat( g_Verts.Load3( 12*Indices.y ) );
        float3 P2 = asfloat( g_Verts.Load3( 12*Indices.z ) );
		
        // transform verts
        float3 P0w = mul( float4(P0,1),  mWorld ).xyz;
        float3 P1w = mul( float4(P1,1),  mWorld ).xyz;
        float3 P2w = mul( float4(P2,1),  mWorld ).xyz;
        float4 P0p = mul( float4(P0w,1), mViewProj );
        float4 P1p = mul( float4(P1w,1), mViewProj );
        float4 P2p = mul( float4(P2w,1), mViewProj );
        P0p.w   = rcp(P0p.w); 
        P1p.w   = rcp(P1p.w);
        P2p.w   = rcp(P2p.w);
        P0p.xyz *= P0p.w; 
        P1p.xyz *= P1p.w;
        P2p.xyz *= P2p.w;

        // clip-space to viewport transform
        P0p.xy = g_vResolution*(float2(0.5,-0.5)*P0p.xy + float2(0.5,0.5));
        P1p.xy = g_vResolution*(float2(0.5,-0.5)*P1p.xy + float2(0.5,0.5));
        P2p.xy = g_vResolution*(float2(0.5,-0.5)*P2p.xy + float2(0.5,0.5));

        // compute AABB
        float2 vBBMin = min(P0p.xy,min(P1p.xy,P2p.xy));
        float2 vBBMax = max(P0p.xy,max(P1p.xy,P2p.xy));
        
        // clip AABB to screen
        vBBMin = max(vBBMin.xy,0);
        vBBMax = min(vBBMax.xy,g_vResolution);

        // reject out-of-bounds triangles
        [branch]
        if( all(vBBMin < vBBMax) )
        {
            // back-face test
            float2 v01 = P1p.xy-P0p.xy;
            float2 v20 = P0p.xy-P2p.xy;
            [branch]
            if( (v01.x*v20.y - v20.x*v01.y) < 0 )
            {
                // Setup Edge functions
                float2 N01 = float2(-v01.y,v01.x);
                float2 N20 = float2(-v20.y,v20.x);
                float3 E01 = float3(N01.xy,-dot(N01,P0p.xy));
                float3 E20 = float3(N20.xy,-dot(N20,P0p.xy));
                E01 *= rcp(dot(E01,float3(P2p.xy,1)));
                E20 *= rcp(dot(E20,float3(P1p.xy,1)));

                // Setup interpolation functions for Z and 1/w
                float3 ZPlane = float3(P0p.z, P2p.z-P0p.z, P1p.z-P0p.z);
                float3 WPlane = float3(P0p.w, P2p.w-P0p.w, P1p.w-P0p.w);

                // Round AABB to pixels
                float2 TriMin = floor(vBBMin);
                float2 TriMax = ceil(vBBMax);

                // offset to pixel center
                TriMin += 0.5f;
                TriMax += 0.5f;

                // iterate pixels
                
                float2 vPos;
                for( vPos.y=TriMin.y; vPos.y < TriMax.y; vPos.y++ )
                {
                    for( vPos.x=TriMin.x; vPos.x < TriMax.x; vPos.x++ )
                    {
                        // TODO: Incremental?
                        float u = dot( float3(vPos,1), E01 );
                        float v = dot( float3(vPos,1), E20 );
                        [branch]
                        if( u>=0 && v>=0 && (u+v)<=1 )
                        {
                            // Interpolate depth
                            float z = dot( float3(1,u,v),ZPlane );
                            float w = dot( float3(1,u,v),WPlane );
                            float depth = z/w;

                            // Surprisingly, reading the Z-buffer first
                            //    and then skipping redundant Interlocked ops
                            //     results in a massive boost
                            //
                            //  I'm not so sure whether this is due to depth complexity or
                            //   if we're somehow speeding up the atomics by ensuring
                            //   that the pixels are in cache first
                            //
                            uint zb = g_ZBuffer[uint2(vPos.xy)];

                            [branch]
                            if( zb > asuint(depth) )
                                InterlockedMin(g_ZBuffer[uint2(vPos.xy)], asuint(depth) );
                        }
                    }
                }
            }
        }
    }
}
