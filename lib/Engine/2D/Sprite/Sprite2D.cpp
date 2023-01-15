#include "Sprite2D.h"

void Sprite2D::Initialize()
{
	SpriteInitialize();

	//定数バッファ生成
	DirectX12Core::GetInstance()->CreateConstBuff(constMapTransform, constBuffTransform);

	float width = static_cast<float>(WindowsApp::GetInstance()->GetWindowWidth());
	float height = static_cast<float>(WindowsApp::GetInstance()->GetWindowHeight());

	AliceMathF::MakeOrthogonalL(0.0f, width, height, 0.0f, 0.0f, 1.0f, matProjection);

	CreatVertexIndexBuffer();
}


void Sprite2D::Update(Transform& transform, const AliceMathF::Vector4& color)
{
	SpriteUpdate(transform, color);
	constMapTransform->mat = matWorld * matProjection;
}

//描画
void Sprite2D::Draw(TextureData& textureData, Material* material)
{
	if (InitializeFlag)
	{

		//画像の半分のサイズ
		float halfW = static_cast<float>(textureData.width) * 0.5f;
		float halfH = static_cast<float>(textureData.height) * 0.5f;

		// 頂点データ
		PosUvColor vertices[] =
		{//		x		y		z		u	v
			{{-halfW, -halfH, 0.0f},{0.0f,0.0f},vertMap[0].color},//左上インデックス0
			{{-halfW, halfH, 0.0f},{0.0f,1.0f},vertMap[1].color},//左下インデックス1
			{{halfW, -halfH, 0.0f},{1.0f,0.0f},vertMap[2].color},//右上インデックス2
			{{halfW, halfH, 0.0f},{1.0f,1.0f},vertMap[3].color},//右下インデックス3
		};

		// インデックスデータ
		uint16_t indices[] =
		{
			1, 0, 3, // 三角形1つ目
			2, 3, 0, // 三角形2つ目
		};

		//頂点バッファへのデータ転送
		for (int i = 0; i < _countof(vertices); i++)
		{
			vertMap[i] = vertices[i]; // 座標をコピー
		}

		//インデックスバッファへのデータ転送
		for (int i = 0; i < _countof(indices); i++)
		{
			indexMap[i] = indices[i];   // インデックスをコピー
		}
	}

	SpriteDraw(textureData, material);
}
