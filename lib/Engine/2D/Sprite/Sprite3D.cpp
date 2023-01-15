#include "Sprite3D.h"


void Sprite3D::Initialize()
{
	SpriteInitialize();

	//定数バッファ生成
	DirectX12Core::GetInstance()->CreateConstBuff(constMapTransform, constBuffTransform);
	
	CreatVertexIndexBuffer();
}

void Sprite3D::Update(Transform& transform, Camera* camera, BillboardFlag flag, const AliceMathF::Vector4& color)
{
		//カラー
	for (int i = 0; i < 4; i++)
	{
		vertMap[i].color = color;
	}

	//平行移動行列
	mTrans.MakeTranslation(transform.translation);
	//回転行列
	mRot.MakeRotation(transform.rotation);
	//スケール行列
	mScale.MakeScaling(transform.scale);

	switch (flag)
	{
	case NonBillboard:
		constMapTransform->mat = mScale * mRot * mTrans * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
		break;
	case AllBillboard:
	{
		AliceMathF::Matrix4 mat = camera->GetViewMatrix();

		mat.m[3][0] = 0;
		mat.m[3][1] = 0;
		mat.m[3][2] = 0;
		mat.m[3][3] = 1;

		constMapTransform->mat = mScale * mRot * mat * mTrans * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
		break;
	}
	case XBillboard:
	{
		AliceMathF::Matrix4 mat = camera->GetViewMatrix();

		mat.m[1][0] = 0;
		mat.m[1][1] = 1;
		mat.m[1][2] = 0;
		mat.m[1][3] = 0;

		mat.m[3][0] = 0;
		mat.m[3][1] = 0;
		mat.m[3][2] = 0;
		mat.m[3][3] = 1;

		constMapTransform->mat = mScale * mRot * mat * mTrans * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
		break;
	}
	case YBillboard:
	{
		AliceMathF::Matrix4 mat = camera->GetViewMatrix();

		mat.m[0][0] = 1;
		mat.m[0][1] = 0;
		mat.m[0][2] = 0;
		mat.m[0][3] = 0;

		mat.m[3][0] = 0;
		mat.m[3][1] = 0;
		mat.m[3][2] = 0;
		mat.m[3][3] = 1;


		constMapTransform->mat = mScale * mRot * mat * mTrans * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
		break;
	}
	case XYBillboard:
	{
		AliceMathF::Matrix4 mat = camera->GetViewMatrix();

		mat.m[2][0] = 0;
		mat.m[2][1] = 0;
		mat.m[2][2] = 1;
		mat.m[2][3] = 0;

		mat.m[3][0] = 0;
		mat.m[3][1] = 0;
		mat.m[3][2] = 0;
		mat.m[3][3] = 1;

		constMapTransform->mat = mScale * mRot * mat * mTrans * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
		break;
	}
	default:
		assert(0);
		break;
	}
}

//描画
void Sprite3D::Draw(TextureData& textureData, Material* material)
{
	if (InitializeFlag)
	{

		//画像の半分のサイズ
		float halfW = static_cast<float>(textureData.width) * 0.0625f;
		float halfH = static_cast<float>(textureData.height) * 0.0625f;

		// 頂点データ
		PosUvColor vertices[] =
		{//		x		y		z		u	v
			{{-halfW, halfH, 0.0f},{0.0f,0.0f},{1.0f,1.0f,1.0f,1.0f}},//左上インデックス0
			{{-halfW, -halfH, 0.0f},{0.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},//左下インデックス1
			{{halfW, halfH, 0.0f},{1.0f,0.0f},{1.0f,1.0f,1.0f,1.0f}},//右上インデックス2
			{{halfW, -halfH, 0.0f},{1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},//右下インデックス3
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

