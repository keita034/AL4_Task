#include "ModelSphere.h"
#include"TextureManager.h"

ModelSphere::ModelSphere()
{
}

ModelSphere::~ModelSphere()
{
}

void ModelSphere::Create(bool smoothing)
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
	float u = 0.0f;
	float v = 0.0f;

	PosNormalUv tmp = { {0, 0, 0},{1,1,1},{0,0} };

	vertices.push_back(tmp);

	size_t dividedInVertical = 16;
	size_t dividedInHorizontal = 32;
	float radius = 1;
	float Deg2Rad = (AliceMathF::AX_PI * 2.0f) / 360.0f;


	for (size_t p = 1; p < dividedInVertical; p++)
	{
		tmp.pos.y = std::cos(Deg2Rad * static_cast<float>(p) * 180.0f / static_cast<float>(dividedInVertical)) * radius;
		float t = std::sin(Deg2Rad * static_cast<float>(p) * 180.0f / static_cast<float>(dividedInVertical)) * radius;

		v += 1.0f / 16.0f;
		u = -1.0f / 32.0f;

		for (size_t q = 0; q < dividedInHorizontal + 1; q++)
		{
			u += 1.0f / 32.0f;
			tmp.uv = { u,v };
			tmp.pos.x = std::cos(Deg2Rad * static_cast<float>(q) * 360.0f / static_cast<float>(dividedInHorizontal)) * t;
			tmp.pos.z = std::sin(Deg2Rad * static_cast<float>(q) * 360.0f / static_cast<float>(dividedInHorizontal)) * t;
			vertices.push_back(tmp);
		}
	}

	u = 0.0f;
	v = 0.0f;
	for (size_t i = 0; i < dividedInHorizontal; i++)
	{
		u += 1.0f / 31.0f;
		tmp.uv = { u,v };
		tmp.pos = AliceMathF::Vector3(0, radius, 0);
		vertices.push_back(tmp);
	}

	u = 0.0f;
	v = 1.0f;
	for (size_t i = 0; i < dividedInHorizontal; i++)
	{
		u += 1.0f / 31.0f;
		tmp.uv = { u,v };
		tmp.pos = AliceMathF::Vector3(0, -radius, 0);
		vertices.push_back(tmp);
	}

#pragma endregion

#pragma region 頂点順序を格納

	uint16_t apexIndex = 496;

	for (size_t i = 0; i < dividedInHorizontal; i++)
	{
		if (i == dividedInHorizontal - 1)
		{
			indices.push_back(static_cast<uint16_t>(apexIndex));
			indices.push_back(static_cast<uint16_t>(i + 2));
			indices.push_back(static_cast<uint16_t>(i + 1));
			break;
		}

		indices.push_back(static_cast<uint16_t>(apexIndex++));
		indices.push_back(static_cast<uint16_t>(i + 2));
		indices.push_back(static_cast<uint16_t>(i + 1));
	}

	for (size_t p = 0; p < dividedInVertical - 1; p++)
	{
		auto firstIndexInLayer = p * dividedInHorizontal + 1;

		for (size_t q = 0; q < dividedInHorizontal; q++)
		{
			if (q == dividedInHorizontal - 1)
			{
				indices.push_back(static_cast<uint16_t>(firstIndexInLayer + q));
				indices.push_back(static_cast<uint16_t>(firstIndexInLayer));
				indices.push_back(static_cast<uint16_t>(firstIndexInLayer + dividedInHorizontal));

				indices.push_back(static_cast<uint16_t>(firstIndexInLayer + q));
				indices.push_back(static_cast<uint16_t>(firstIndexInLayer + dividedInHorizontal));
				indices.push_back(static_cast<uint16_t>(firstIndexInLayer + q + dividedInHorizontal));

				break;
			}

			indices.push_back(static_cast<uint16_t>(firstIndexInLayer + q));
			indices.push_back(static_cast<uint16_t>(firstIndexInLayer + q + 1));
			indices.push_back(static_cast<uint16_t>(firstIndexInLayer + q + 1 + dividedInHorizontal));

			indices.push_back(static_cast<uint16_t>(firstIndexInLayer + q));
			indices.push_back(static_cast<uint16_t>(firstIndexInLayer + q + dividedInHorizontal + 1));
			indices.push_back(static_cast<uint16_t>(firstIndexInLayer + q + dividedInHorizontal));
		}
	}

	apexIndex = 528;
	for (size_t i = 0; i < dividedInHorizontal; i++)
	{
		if (i == dividedInHorizontal - 1)
		{
			indices.push_back(static_cast<uint16_t>(apexIndex));
			indices.push_back(static_cast<uint16_t>(static_cast<size_t>(496) - 1 - dividedInHorizontal + i));
			indices.push_back(static_cast<uint16_t>(496 - dividedInHorizontal + i));
			break;
		}
		indices.push_back(static_cast<uint16_t>(apexIndex++));
		indices.push_back(static_cast<uint16_t>(static_cast<size_t>(496) - 1 - dividedInHorizontal + i));
		indices.push_back(static_cast<uint16_t>(496 - dividedInHorizontal + i));
	}

#pragma endregion



	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		//三角形1つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数に入れる
		unsigned short Index0 = indices[static_cast<std::vector<uint16_t, std::allocator<uint16_t>>::size_type>(i) * 3 + 0];
		unsigned short Index1 = indices[static_cast<std::vector<uint16_t, std::allocator<uint16_t>>::size_type>(i) * 3 + 1];
		unsigned short Index2 = indices[static_cast<std::vector<uint16_t, std::allocator<uint16_t>>::size_type>(i) * 3 + 2];
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

void ModelSphere::Draw(Transform* transform, Material* material)
{
	ModelDraw(transform, material);
}

void ModelSphere::Initialize()
{
	ModelInitialize();

	maxIndex = sphereMaxIndex;
	maxVert = sphereMaxVert;

	Create();
}

#pragma region 削除関数

void ModelSphere::Create(const char* filePath, bool smoothing)
{
	static_cast<void>(filePath);
	static_cast<void>(smoothing);
}

#pragma endregion