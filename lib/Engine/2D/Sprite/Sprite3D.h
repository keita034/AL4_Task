#pragma once
//#DirectXのやつ
#include"ErrorException.h"

//自作.h
#include"AliceMathUtility.h"
#include"WindowsApp.h"
#include"DirectX12Core.h"
#include "Camera.h"
#include"Sprite.h"
#include"DefaultMaterial.h"

//pragma comment



//using namespace


class Sprite3D : public Sprite
{
private:

	AliceMathF::Matrix4 mTrans, mRot, mScale;

public:

	Sprite3D() = default;
	~Sprite3D() = default;

	//初期化
	virtual void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update(Transform& transform, Camera* camera, BillboardFlag flag = NonBillboard, const AliceMathF::Vector4& color = { 1.0f,1.0f,1.0f,1.0f });

	///<summary>
	///描画
	///</summary>
	virtual void Draw(TextureData& textureData, Material* material = &DEFAULT_SPRITE3D_MATERIAL)override;

private:



	//コピーコンストラクタ・代入演算子削除
	Sprite3D& operator=(const Sprite3D&) = delete;
	Sprite3D(const Sprite3D&) = delete;
};

