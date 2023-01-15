struct VSOutput
{
    // システム用頂点座標
    float4 svpos : SV_POSITION;
    // uv値
    float4 col : COLOR;
};

//3D変換行列
cbuffer ConstBufferDataTransform : register(b0)
{
    matrix mat; //3D変換行列
};