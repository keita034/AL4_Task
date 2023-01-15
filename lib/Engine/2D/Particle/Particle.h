#pragma once
#include"ErrorException.h"
#include"AliceMathUtility.h"
#include"Camera.h"

struct VerPosColScaRot
{
	AliceMathF::Vector3 pos;//座標
	AliceMathF::Vector4 color;//カラー
	float scale;//スケール
	float rotation;//回転角
};

struct ParticleConstBuffData
{
	AliceMathF::Matrix4 mat;
	AliceMathF::Matrix4 matBillboard;
};

//パーティクルデータ
struct ParticleData
{
	//座標
	AliceMathF::Vector3 position = {};
	//速度
	AliceMathF::Vector3 velocity = {};
	//加速度
	AliceMathF::Vector3 accel = {};

	//現在フレーム
	UINT frame = 0;
	//終了フレーム
	UINT numFrame = 0;

	//スケール
	float scale = 1.0f;
	//初期値
	float sScale = 1.0f;
	//最終値
	float eScale = 0.0f;

	//回転角
	float rotation = 0.0f;
	//初期値
	float sRotation = 0.0f;
	//最終値
	float eRotation = 0.0f;

	//カラー
	AliceMathF::Vector4 color = {1.0f,1.0f,1.0f,1.0f};
	//初期値
	AliceMathF::Vector4 sColor = { 1.0f,1.0f,1.0f,1.0f };
	//最終値
	AliceMathF::Vector4 eColor = { 1.0f,1.0f,1.0f,1.0f };
};

class Particle
{
protected:
	HRESULT result;
	char PADDING[4];
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr <ID3D12GraphicsCommandList> cmdList = nullptr;
	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};
	DirectX::ScratchImage mipChain{};

	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	Microsoft::WRL::ComPtr <ID3D12RootSignature> rootSignature;
	//デスクプリタヒープ
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> srvHeap;
	//デスクプリタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange;
	//スプライト数
	UINT nextIndex;
	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	//頂点マップ
	VerPosColScaRot* vertMap;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//テクスチャバッファ
	Microsoft::WRL::ComPtr <ID3D12Resource> texBuff = nullptr;
	//GPUデスクプリタハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	//CPUデスクプリタハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	//定数バッファのGPUリソースのポインタ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	//定数バッファのマッピング用ポインタ
	ParticleConstBuffData* constMapTransform = nullptr;
	//プロジェクション行列
	AliceMathF::Matrix4 matProjection;
	//頂点数
	const uint32_t vertexCount = 1024;
	char PADING[4];
	//パーティクル配列
	std::forward_list<ParticleData>particleDatas;
public:

	Particle()= default;

	virtual ~Particle() = default;

	//初期化
	virtual void Initialize(ModelShareVaria& modelShareVaria) = 0;


	///<summary>
	///読み込み
	///</summary>
	///<param name="filePath">ファイルパス</param>
	virtual void Load(const wchar_t* filePath) = 0;

	///<summary>
	///更新
	///</summary>
	virtual void Update() = 0;

	///<summary>
	///描画
	///</summary>
	virtual void Draw(Camera* camera) = 0;

	/// <summary>
	/// パーティクルの追加
	/// </summary>
	/// <param name="life">生存時間</param>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param>
	/// <param name="accel">加速度</param>
	/// <param name="scale">{開始時スケール,終了時スケール}</param>
	/// <param name="rotation">{開始時回転角,終了時回転角}</param>
	/// <param name="sColor">開始カラー</param>
	/// <param name="eColor">終了カラー</param>
	virtual void Add(
		UINT life, AliceMathF::Vector3& position, AliceMathF::Vector3& velocity,
		AliceMathF::Vector3& accel,AliceMathF::Vector2& scale, AliceMathF::Vector2& rotation
		,AliceMathF::Vector4& sColor, AliceMathF::Vector4& eColor) = 0;

protected:
	//シェーダ－リソースビュー生成
	virtual void CreateShaderResourceView() = 0;
	//定数バッファ生成(2D座標変換行列)
	virtual void CreatConstBuff() = 0;
	//頂点バッファ・インデックス生成
	virtual void CreatVertexIndexBuffer() = 0;
	//テクスチャバッファ生成
	virtual void CreatTextureBuffer() = 0;

	//コピーコンストラクタ・代入演算子削除
	Particle& operator=(const Particle&) = delete;
	Particle(const Particle&) = delete;
};