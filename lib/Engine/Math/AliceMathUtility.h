#pragma once
#include"AliceMathF.h"
#include"AliceMath.h"

namespace AliceMathUtility
{
	/// <summary>
	/// ディレクトリからファイル名一覧を所得
	/// </summary>
	/// <param name="folderPath">ディレクトリパス</param>
	/// <returns>ファイル名一覧</returns>
	std::vector<std::string> getFileNames(std::string folderPath);

#pragma region 絶対値の計算

	//絶対値の計算
	int Abs(int abs);

	//絶対値の計算
	long AbsL(long abs);

	//絶対値の計算
	long long AbsLL(long long abs);

#pragma endregion 絶対値の計算

#pragma region 比較関数

	/// <summary>
	/// 値が大きい方を返す
	/// </summary>
	template<typename T>
	T Max(T a, T b)
	{
		return (a > b) ? a : b;
	}

	/// <summary>
	/// 値が大きい奴を返す(vector)
	/// </summary>
	template<typename T>
	T Max(std::vector<T> values)
	{
		int num = values.size() - 1;

		if (num == 0)
		{
			return static_cast<T>(0);
		}

		float num2 = values[0];

		for (int i = 1; i < num; i++)
		{
			if (values[i] > num2)
			{
				num2 = values[i];
			}
		}

		return num2;
	}

	/// <summary>
	/// 値が小さい方を返す
	/// </summary>
	template<typename T>
	T Min(T a, T b)
	{
		return (a < b) ? a : b;
	}

	/// <summary>
	/// 値が小さい奴を返す(vector)
	/// </summary>
	template<typename T>
	T Min(std::vector<T> values)
	{
		int num = values.size() - 1;

		if (num == 0)
		{
			return static_cast<T>(0);
		}

		float num2 = values[0];

		for (int i = 1; i < num; i++)
		{
			if (values[i] < num2)
			{
				num2 = values[i];
			}
		}

		return num2;
	}

#pragma endregion 比較関数
};

//座標変換行列
struct ConstBufferDataTransform
{
	AliceMathF::Matrix4 mat;//3D変換行列
};


struct PosColor
{
	AliceMathF::Vector3 pos;//座標
	AliceMathF::Vector4 color;//カラー
};

struct PosNormalUv
{
	AliceMathF::Vector3 pos;//座標
	AliceMathF::Vector3 normal;//法線ベクトル
	AliceMathF::Vector2 uv;//uv座標
};

//座標・UV・カラー
struct PosUvColor
{
	AliceMathF::Vector3 pos;//座標
	AliceMathF::Vector2 uv;//uv座標
	AliceMathF::Vector4 color;//カラー
};

typedef struct
{
	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	//デスクプリタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;
	//デスクプリタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange;
	//スプライト数
	UINT nextIndex;
}ModelShareVaria;

//パイプライン・ルートシグネチャセット
struct PipelineSet
{
	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
};

struct worldViewpojCamera
{
	//ワールド行列
	AliceMathF::Matrix4 world;
	//ワールド座標
	AliceMathF::Matrix4 matWorld;
	//カメラ座標(ワールド座標)
	AliceMathF::Vector3 cameraPos;
};

//マテリアル
struct ModelMaterial
{
	//マテリアル名
	std::string name;
	//アンビエント影響度
	AliceMathF::Vector3 ambient = { 0.06f, 0.06f, 0.06f };
	//ディフューズ影響度
	AliceMathF::Vector3 diffuse = { 0.0f,0.0f,0.0f };
	//スペキュラー影響度
	AliceMathF::Vector3 specular = { 0.0f,0.0f,0.0f };
	//アルファ
	float alpha = 1.0f;
	//テクスチャファイル名
	std::string textureFiename;
};

//マテリアルよう定数バッファ構造体
struct ConstBuffDataMaterial
{
	//アンビエント係数
	AliceMathF::Vector3 ambient;
	//パディング
	float pad1;
	//ディフューズ係数
	AliceMathF::Vector3 diffuse;
	//パディング
	float pad2;
	//スペキュラー係数
	AliceMathF::Vector3 specular;
	//アルファ
	float alpha;
};

//バッファ関連
struct Buff
{
	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	//頂点マップ
	PosColor* vertMap;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//インデックスバッファの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
	////インデックスバッファをマッピング
	uint16_t* indexMap;
	//インデックスバッファビューの作成
	D3D12_INDEX_BUFFER_VIEW ibView{};
};

//ブレンドモード
enum class BlendMode
{
	AX_BLENDMODE_NOBLEND,//ノーブレンド（デフォルト）
	AX_BLENDMODE_ALPHA,//αブレンド
	AX_BLENDMODE_ADD,//加算ブレンド
	AX_BLENDMODE_SUB,//減算ブレンド
	AX_BLENDMODE_MULA,//乗算ブレンド
	AX_BLENDMODE_INVSRC,//反転ブレンド

	AX_BLENDMODE_MAX//ブレンド種類数
};

struct TextureData
{
	//テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;

	//デスクプリタヒープ
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> srvHeap;

	//GPUデスクプリタハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};

	//デスクプリタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	char PADDING[4]{};
	//横幅
	size_t width = 0;
	//縦幅
	size_t height = 0;
};