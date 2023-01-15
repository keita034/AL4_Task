#pragma once
#include"ErrorException.h"
#include"AliceMathUtility.h"
#include"Camera.h"
#include"ModelPipeLine.h"
#include"DirectX12Core.h"
#include"Light.h"
#include"Transform.h"
#include"Material.h"

enum class ModelShape
{
	Cube,//立方体
	Sphere,//球
	Capsule,//カプセル
	Cylinder,//円柱
	Cone,//円錐
};

enum ShaderType
{
	Default,
	Flat,
	Gouraud,
	Lambert,
	Phong,

};

class Model
{
protected:

	TextureData textureData;

	HRESULT result;
	char PADDING[4];
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList;

	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	//頂点マップ
	PosNormalUv* vertMap;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//インデックスバッファの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
	////インデックスバッファをマッピング
	uint16_t* indexMap;
	//インデックスバッファビューの作成
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
	char PADDING1[4];
	//インデックスの数
	UINT maxIndex;
	char PADDING2[4];
	//頂点の数
	UINT maxVert;
	//ワールド行列
	AliceMathF::Matrix4 matWorld;
	//頂点データ
	std::vector<PosNormalUv>vertices;
	//頂点インデックス
	std::vector<uint16_t> indices;
	//頂点法線スムージング用データ
	std::unordered_map<uint16_t, std::vector<uint16_t>>smoothData;
	//ライト
	static Light* light;
	//マテリアル
	ModelMaterial modelMaterial{};
	//定数バッファマテリアル
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial;
	//定数バッファマテリアルのマッピング用ポインタ
	ConstBuffDataMaterial* constMapMaterial = nullptr;


public:

	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	virtual AliceMathF::Matrix4& GetMatWorld();

	/// <summary>
	/// 頂点座標を取得
	/// </summary>
	/// <returns>頂点座標配列</returns>
	virtual const std::vector<PosNormalUv>GetVertices();

	/// <summary>
	/// インデックスを取得
	/// </summary>
	/// <returns>インデックス座標配列</returns>
	virtual const std::vector<uint16_t>GetIndices();

	/// <summary>
	/// ライトのセット
	/// </summary>
	/// <param name="light">ライト</param>
	static void SetLight(Light* light_);

	Model() = default;
	virtual ~Model()= default;

	virtual void Initialize() = 0;

	virtual void Draw(Transform* transform,Material* material) = 0;

protected:

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	virtual void Create(bool smoothing) = 0;

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <param name="filePath">オブジェクトまでのファイルパス</param>
	virtual void Create(const char* filePath, bool smoothing) = 0;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	virtual void ModelDraw(Transform* transform, Material* material);

	//初期化
	virtual void ModelInitialize();


	//頂点バッファ・インデックス生成
	virtual void CreatVertexIndexBuffer();

	//コピーコンストラクタ・代入演算子削除
	Model& operator=(const Model&) = delete;
	Model(const Model&) = delete;
};