#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION, float4 col : COLOR, float sca : SCALE, float rot : ROTATION)
{
    VSOutput output; // ピクセルシェーダーに渡す値
    output.svpos =  pos;
    output.color = col;
    output.scale = sca;
    output.rotation = rot;
    return output;
}