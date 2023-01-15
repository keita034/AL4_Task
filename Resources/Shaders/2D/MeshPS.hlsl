#include "Mesh.hlsli"

float4 main(VSOutput input) : SV_TARGET
{
    return input.col;
}