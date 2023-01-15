#pragma once
//#DirectXのやつ
#include"ErrorException.h"

//自作.h
#include"AliceMathUtility.h"
#include"WindowsApp.h"
#include"DirectX12Core.h"
#include"Sprite.h"
#include"DefaultMaterial.h"
//pragma comment



//using namespace

class Sprite2D : public Sprite
{
private:


public:

	Sprite2D() = default;
	~Sprite2D() = default;

	//初期化
	virtual void Initialize()override;

	///<summary>
	///更新
	///</summary>
	///<param name="pos">座標</param>
	///<param name="rot">回転</param>
	///<param name="scale">拡大率</param>
	/// ///<param name="color"> カラー</param>
	virtual void Update(Transform& transform, const AliceMathF::Vector4& color = {1.0f,1.0f,1.0f,1.0f});

	///<summary>
	///描画
	///</summary>
	virtual void Draw(TextureData& textureData, Material* material = &DEFAULT_SPRITE2D_MATERIAL)override;

private:


	//コピーコンストラクタ・代入演算子削除
	Sprite2D& operator=(const Sprite2D&) = delete;
	Sprite2D(const Sprite2D&) = delete;

};