#pragma once
#include"ErrorException.h"
#include"AliceMathUtility.h"
#include"Transform.h"
#include"Camera.h"
#include"Material.h"

enum BillboardFlag
{
	NonBillboard,//ビルボードをしない
	AllBillboard,//すべて打ち消す
	XBillboard,//X軸を打ち消す
	YBillboard,//Y軸を打ち消す
	XYBillboard//Z軸を打ち消す
};


class Sprite
{
protected:
	HRESULT result;
	char PADDING[4];
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr <ID3D12GraphicsCommandList> cmdList = nullptr;

	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	//頂点マップ
	PosUvColor* vertMap;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//インデックスバッファの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
	////インデックスバッファをマッピング
	uint16_t* indexMap;
	//インデックスバッファビューの作成
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//定数バッファのGPUリソースのポインタ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	//定数バッファのマッピング用ポインタ
	ConstBufferDataTransform* constMapTransform = nullptr;
	//ワールド座標
	AliceMathF::Matrix4 matWorld;
	//プロジェクション行列
	AliceMathF::Matrix4 matProjection;

	bool InitializeFlag = true;
	char PADDING2[7]{};
public:

	Sprite()= default;
	virtual ~Sprite() = default;

	//初期化
	virtual void Initialize() = 0;

	///<summary>
	///描画
	///</summary>
	virtual void Draw(TextureData& textureData, Material* material)=0;

protected:

	//初期化
	virtual void SpriteInitialize();

	//更新
	virtual void SpriteUpdate(Transform& transform, const AliceMathF::Vector4& color = { 1.0f,1.0f, 1.0f, 1.0f });

	//描画
	virtual void SpriteDraw(TextureData& textureData, Material* material);

	//頂点バッファ・インデックス生成
	virtual void CreatVertexIndexBuffer();

	//コピーコンストラクタ・代入演算子削除
	Sprite& operator=(const Sprite&) = delete;
	Sprite(const Sprite&) = delete;
};