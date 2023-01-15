#include "ModelCube.h"
#include"DirectX12Core.h"
#include"TextureManager.h"

ModelCube::ModelCube()
{
}

ModelCube::~ModelCube()
{
}

void ModelCube::Create(bool smoothing)
{
	static_cast<void>(smoothing);
	//頂点バッファ・インデックス生成
	CreatVertexIndexBuffer();

	//定数バッファ生成(マテリアル)
	DirectX12Core::GetInstance()->CreateConstBuff(constMapMaterial, constBuffMaterial);
	AliceMathF::Vector3 one = { 0.8f,0.8f,0.8f };
	constMapMaterial->ambient = one;
	constMapMaterial->diffuse = one;
	constMapMaterial->specular = one;
	constMapMaterial->alpha = modelMaterial.alpha;

	// 頂点データ
	//正面
	PosNormalUv tmp = { { -1.0f, -1.0f, -1.0f }, {}, { 0.0f, 1.0f } };//左上インデックス0
	vertices.push_back(tmp);
	tmp = { {-1.0f, 1.0f,-1.0f},{},{ 0.0f, 0.0f} };//左下インデックス1
	vertices.push_back(tmp);
	tmp = { { 1.0f,-1.0f,-1.0f},{},{ 1.0f, 1.0f} };//右上インデックス2
	vertices.push_back(tmp);
	tmp = { { 1.0f, 1.0f,-1.0f},{},{ 1.0f, 0.0f} };//右下インデックス3
	vertices.push_back(tmp);

	//右
	tmp = { { 1.0f,-1.0f,-1.0f},{},{ 0.0f, 1.0f} };//左上インデックス4
	vertices.push_back(tmp);
	tmp = { { 1.0f, 1.0f,-1.0f},{},{ 0.0f, 0.0f} };//左下インデックス5
	vertices.push_back(tmp);
	tmp = { { 1.0f,-1.0f, 1.0f},{},{ 1.0f, 1.0f} };//右上インデックス6
	vertices.push_back(tmp);
	tmp = { { 1.0f, 1.0f, 1.0f},{},{ 1.0f, 0.0f} };//右下インデックス7
	vertices.push_back(tmp);

	//左
	tmp = { {-1.0f,-1.0f, 1.0f},{},{ 0.0f, 1.0f} };//左上インデックス8
	vertices.push_back(tmp);
	tmp = { {-1.0f, 1.0f, 1.0f},{},{ 0.0f, 0.0f} };//左下インデックス9
	vertices.push_back(tmp);
	tmp = { {-1.0f,-1.0f,-1.0f},{},{ 1.0f, 1.0f} };//右上インデックス10
	vertices.push_back(tmp);
	tmp = { {-1.0f, 1.0f,-1.0f},{},{ 1.0f, 0.0f} };//右下インデックス11
	vertices.push_back(tmp);
	
	//裏
	tmp = { { 1.0f,-1.0f, 1.0f},{},{ 0.0f, 1.0f} };//左上インデックス12
	vertices.push_back(tmp);
	tmp = { { 1.0f, 1.0f, 1.0f},{},{ 0.0f, 0.0f} };//左下インデックス13
	vertices.push_back(tmp);
	tmp = { {-1.0f,-1.0f, 1.0f},{},{ 1.0f, 1.0f} };//右上インデックス14
	vertices.push_back(tmp);
	tmp = { {-1.0f, 1.0f, 1.0f},{},{ 1.0f, 0.0f} };//右下インデックス15
	vertices.push_back(tmp);

	//上
	tmp = { {-1.0f, 1.0f,-1.0f},{},{ 0.0f, 1.0f} };//左上インデックス16
	vertices.push_back(tmp);
	tmp = { {-1.0f, 1.0f, 1.0f},{},{ 0.0f, 0.0f} };//左下インデックス17
	vertices.push_back(tmp);
	tmp = { { 1.0f, 1.0f,-1.0f},{},{ 1.0f, 1.0f} };//右上インデックス18
	vertices.push_back(tmp);
	tmp = { { 1.0f, 1.0f, 1.0f},{},{ 1.0f, 0.0f} };//右下インデックス19
	vertices.push_back(tmp);

	//底
	tmp = { {-1.0f,-1.0f, 1.0f},{},{ 0.0f, 1.0f} };//左上インデックス20
	vertices.push_back(tmp);
	tmp = { {-1.0f,-1.0f,-1.0f},{},{ 0.0f, 0.0f} };//左下インデックス21
	vertices.push_back(tmp);
	tmp = { { 1.0f,-1.0f, 1.0f},{},{ 1.0f, 1.0f} };//右上インデックス22
	vertices.push_back(tmp);
	tmp = { { 1.0f,-1.0f,-1.0f},{},{ 1.0f, 0.0f} };//右下インデックス23
	vertices.push_back(tmp);

// インデックスデータ

	//正面
	indices.push_back(0), indices.push_back(1), indices.push_back(3);//0
	indices.push_back(3), indices.push_back(2), indices.push_back(0);//1

	//右
	indices.push_back(4), indices.push_back(5), indices.push_back(7);//2
	indices.push_back(7), indices.push_back(6), indices.push_back(4);//3

	//左
	indices.push_back(8), indices.push_back(9), indices.push_back(11);//4
	indices.push_back(11), indices.push_back(10), indices.push_back(8);//5

	//裏
	indices.push_back(12), indices.push_back(13), indices.push_back(15);//6
	indices.push_back(15), indices.push_back(14), indices.push_back(12);//7

	//上
	indices.push_back(16), indices.push_back(17), indices.push_back(19);//8
	indices.push_back(19), indices.push_back(18), indices.push_back(16);//9

	//底
	indices.push_back(20), indices.push_back(21), indices.push_back(23);//10
	indices.push_back(23), indices.push_back(22), indices.push_back(20);//11

	for (size_t i = 0; i < cubeMaxIndex / 3; i++)
	{
		//三角形1つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数に入れる
		unsigned short Index0 = indices[i * 3 + 0];
		unsigned short Index1 = indices[i * 3 + 1];
		unsigned short Index2 = indices[i * 3 + 2];
		//三角形を構成する頂点座標をベクトルに代入
		AliceMathF::Vector3 p0 = vertices[Index0].pos;
		AliceMathF::Vector3 p1 = vertices[Index1].pos;
		AliceMathF::Vector3 p2 = vertices[Index2].pos;
		//p0->p1ベクトル、p0->p2ベクトルを計算(ベクトルの減算)
		AliceMathF::Vector3 v1 = p1 - p0;
		AliceMathF::Vector3 v2 = p2 - p0;
		//外積は両方から垂直なベクトル
		AliceMathF::Vector3 normal = v1.cross(v2);
		//正規化する
		normal.normal();
		vertices[Index0].normal = normal;
		vertices[Index1].normal = normal;
		vertices[Index2].normal = normal;

	}
	//頂点バッファへのデータ転送
	for (size_t i = 0; i < vertices.size(); i++)
	{
		vertMap[i] = vertices[i]; // 座標をコピー
	}

	//インデックスバッファへのデータ転送
	for (size_t i = 0; i < indices.size(); i++)
	{
		indexMap[i] = indices[i];   // インデックスをコピー
	}

	matWorld = AliceMathF::MakeIdentity();
}

void ModelCube::Draw(Transform* transform, Material* material)
{
	ModelDraw(transform,material);
}

void ModelCube::Create(const char* filePath, bool smoothing)
{
	static_cast<void>(filePath);
	static_cast<void>(smoothing);
}

void ModelCube::Initialize()
{
	ModelInitialize();

	maxIndex = cubeMaxIndex;
	maxVert = cubeMaxVert;

	Create();

}
