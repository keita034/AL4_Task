#include "Particle.hlsli"

float4 rotate(float4 pos, float angle)
{
    return float4(
    pos.x * cos(angle) + pos.y * -sin(angle),
    pos.x * sin(angle) + pos.y * cos(angle),
    pos.z,
    pos.w);
}

//四角形の頂点
static const uint vnum = 4;

//センターからオフセット
static const float4 offset_array[vnum] =
{
    float4(-0.5f, -0.5f, 0, 0), //左下
    float4(-0.5f, +0.5f, 0, 0), //左上
    float4(+0.5f, -0.5f, 0, 0), //右下
    float4(+0.5f, +0.5f, 0, 0) //右上
};

//左上が0,0 右下が1,1

static const float2 uv_array[vnum] =
{
    float2(0, 1), //左下
    float2(0, 0), //左上
    float2(1, 1), //右下
    float2(1, 0), //右上
};

[maxvertexcount(vnum)]

void main(point VSOutput input[1] : SV_POSITION,
	inout TriangleStream<GSOutput> output)
{
    GSOutput element;
    for (uint i = 0; i < vnum; i++)
    {
        //中心からのオフセットをスケーリング
        float4 offset = offset_array[i] * input[0].scale;

       //中心からのオフセットをローテーション
        offset = rotate(offset, input[0].rotation);

        //中心からのオフセットをビルボード回転
        offset = mul(matBillboard, offset);

        //オフセット分ずらす
        element.svpos = input[0].svpos + offset;

        //ビュープロジェクション変換
        element.svpos = mul(matWorld, element.svpos);
        element.uv = uv_array[i];
        element.color = input[0].color;
        output.Append(element);
    }
	
 
}