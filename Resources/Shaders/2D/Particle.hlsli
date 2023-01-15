cbuffer cbuff0 : register(b0)
{
    matrix matWorld; // 
    matrix matBillboard;
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float4 color : COLOR; // 色(RGBA)
    float scale : SCALE;//スケール
    float rotation : ROTATION;//回転角
};

struct GSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float4 color : COLOR; // 色(RGBA)
    float2 uv : TEXCOORD; // uv値

};