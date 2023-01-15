#include "ModelCylinder.h"
#include"TextureManager.h"

ModelCylinder::ModelCylinder()
{
}

ModelCylinder::~ModelCylinder()
{
}

void ModelCylinder::Create(bool smoothing)
{
	static_cast<void>(smoothing);

	maxIndex = cylinderMaxIndex;
	maxVert = cylinderMaxVert;

	//頂点バッファ・インデックス生成
	CreatVertexIndexBuffer();

	//定数バッファ生成(マテリアル)
	DirectX12Core::GetInstance()->CreateConstBuff(constMapMaterial, constBuffMaterial);
	constMapMaterial->ambient = modelMaterial.ambient;
	constMapMaterial->diffuse = modelMaterial.diffuse;
	constMapMaterial->specular = modelMaterial.specular;
	constMapMaterial->alpha = modelMaterial.alpha;

	// 頂点データ
	constexpr UINT NumDiv = 32; // 分割数
	float RadianPerDivision = AliceMathF::AX_2PI / float(NumDiv); // 分割1単位の角度

	//分割点分割数より1つ多くしておいて1周回るようにする
	std::array<float, NumDiv + 1> pointsZ{};
	std::array<float, NumDiv + 1> pointsX{};
	std::array<float, NumDiv + 1> pointsU{};
	std::array<float, NumDiv + 1> pointsV{};

	//上側の計算
	//頂点座標の計算
	for (size_t i = 0; i < NumDiv; ++i)
	{
		float baseZ = cos(RadianPerDivision * static_cast<float>(i)) * 1.0f;
		float baseX = sin(RadianPerDivision * static_cast<float>(i)) * 1.0f;
		pointsZ[i] = baseZ * -1.0f;
		pointsX[i] = baseX;
		//UV座標の計算
		float baseU = cos(RadianPerDivision * static_cast<float>(i)) * 0.25f;
		float baseV = sin(RadianPerDivision * static_cast<float>(i)) * 0.25f;
		pointsU[i] = baseU + 0.75f;
		pointsV[i] = baseV + 0.75f;
	}

	//最後と最初は一緒
	(*pointsZ.rbegin()) = (*pointsZ.begin());
	(*pointsX.rbegin()) = (*pointsX.begin());
	(*pointsU.rbegin()) = (*pointsU.begin());
	(*pointsV.rbegin()) = (*pointsV.begin());

	PosNormalUv tmp;

	//計算した値を代入
	for (size_t i = 0; i < NumDiv; i++)
	{
		tmp = {{ pointsX[i],1.0f,pointsZ[i] },{},{ pointsV[i],pointsU[i] } };
		vertices.push_back(tmp);
	}

	tmp = { { 0.0f,1.0f,0.0f },{},{ 0.75f,0.75f } };
	vertices.push_back(tmp);

	//下側の計算
	//UV座標の計算
	for (size_t i = 0; i < NumDiv; ++i)
	{
		float baseU = cos(RadianPerDivision * static_cast<float>(i)) * 0.25f;
		float baseV = sin(RadianPerDivision * static_cast<float>(i)) * 0.25f;
		pointsU[i] = baseU + 0.25f;
		pointsV[i] = baseV + 0.75f;
	}

	(*pointsU.rbegin()) = (*pointsU.begin());
	(*pointsV.rbegin()) = (*pointsV.begin());

	//計算した値を代入
	for (size_t i = 0; i < static_cast<size_t>(NumDiv); i++)
	{
		tmp = { { pointsX[i],-1.0f,pointsZ[i] },{},{ pointsU[i],pointsV[i] }, };
		vertices.push_back(tmp);
	}

	tmp = { { 0.0f,-1.0f,0.0f },{},{ 0.25f,0.75f } };
	vertices.push_back(tmp);

	//UV座標の計算
	for (size_t i = 0; i < static_cast<size_t>(NumDiv + 1); ++i)
	{
		pointsU[i] = static_cast<float>(i) * (1.0f / 31.0f);
		pointsV[i] = 0.5f;
	}
	(*pointsU.rbegin()) = 1.0f;

	//計算した値を代入
	for (size_t i = 0; i < static_cast<size_t>(NumDiv + 1); i++)
	{
		tmp = { { pointsX[i],-1.0f,pointsZ[i] },{},{ pointsU[i],pointsV[i] } };
		vertices.push_back(tmp);
	}

	//UV座標の計算
	for (size_t i = 0; i < static_cast<size_t>(NumDiv + 1); ++i)
	{
		pointsU[i] = static_cast<float>(i) * (1.0f / 31.0f);
		pointsV[i] = 0.0f;
	}

	(*pointsU.rbegin()) = 1.0f;

	//計算した値を代入
	for (size_t i = 0; i < static_cast<size_t>(NumDiv + 1); i++)
	{
		tmp = { { pointsX[i],1.0f,pointsZ[i] },{},{ pointsU[i],pointsV[i] } };
		vertices.push_back(tmp);
	}

	// インデックスデータ
	uint8_t count = 0;
	uint16_t indexData = 0;

	//インデックスデータの計算上側
	for (uint16_t i = 0; i < 93; i++)
	{
		if (count == 3)
		{
			count = 0;
		}

		if (i == 92)
		{
			indices.push_back(0);
			count++;
		}

		switch (count)
		{
		case 0:
			indices.push_back(indexData);
			break;
		case 1:
			indices.push_back(32);
			break;
		case 2:
			indexData++;
			indices.push_back(indexData);

			break;
		}

		count++;
	}

	count = 0;
	indexData = 34;
	//インデックスデータの計側
	for (size_t i = 0; i < 95; i++)
	{
		if (count == 3)
		{
			count = 0;
		}

		if (i == 93)
		{
			indices.push_back(33);
			count++;
		}

		switch (count)
		{
		case 0:
			indices.push_back(indexData);
			break;
		case 1:
			indices.push_back(65);
			break;
		case 2:
			indices.push_back(indexData - static_cast<unsigned>(1));
			indexData++;
			break;
		}
		count++;
	}

	count = 0;
	indexData = 66;
	//インデックスデータの計側
	for (size_t i = 0; i < 96; i++)
	{
		if (count == 3)
		{
			count = 0;
		}

		if (i == 95)
		{
			indices.push_back(98);
			count++;
		}

		switch (count)
		{
		case 0:
			indices.push_back(indexData);
			break;
		case 1:
			indices.push_back(indexData + static_cast<unsigned>(33));
			break;
		case 2:
			indices.push_back(indexData + static_cast<unsigned>(1));
			indexData += 1;
			break;
		}
		count++;

	}

	count = 0;
	indexData = 98;
	//インデックスデータの計側
	for (size_t i = 0; i < 99; i++)
	{
		if (count == 3)
		{
			count = 0;
		}

		if (i == 97)
		{
			indices.push_back(98);
			count++;
		}

		switch (count)
		{
		case 0:
			indices.push_back(indexData + static_cast<unsigned>(1));
			break;
		case 1:
			indices.push_back(indexData - static_cast<unsigned>(32));
			break;
		case 2:
			indices.push_back(indexData);
			indexData += 1;
			break;
		}
		count++;
	}

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

void ModelCylinder::Draw(Transform* transform, Material* material)
{

	ModelDraw(transform,material);
}

void ModelCylinder::Create(const char* filePath,bool smoothing)
{
	static_cast<void>(filePath);
	static_cast<void>(smoothing);
};

void ModelCylinder::Initialize()
{
	ModelInitialize();

	maxIndex = cylinderMaxIndex;
	maxVert = cylinderMaxVert;

	Create();

}