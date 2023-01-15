#include"Lambert.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    // 法線にワールド行列によるスケーリング・回転を適用
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
 
     // ピクセルシェーダーに渡す値
    VSOutput output;
    output.svpos = mul(matWorld, pos);
    output.uv = uv;
    output.color.rgb = dot(lightv, wnormal.xyz) * m_diffuse * lightcolor.rgb;
    output.color.a = m_alpha;
    return output;
}