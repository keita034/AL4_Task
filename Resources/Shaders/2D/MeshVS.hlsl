#include "Mesh.hlsli"

VSOutput main(float4 pos : POSITION,float4 col : COLOR)
{
    VSOutput vsOutput;
    vsOutput.svpos = mul(mat, pos);
    vsOutput.col = col;
    return vsOutput;
}