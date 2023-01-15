#pragma once
#include"ErrorException.h"
#include"AliceMathUtility.h"
#include"DirectX12Core.h"

struct ShaderFile
{
	//シェーダーファイルパス
	std::wstring pFileName;
	//エントリーポイント
	std::string pEntrypoint;
	//バージョン
	std::string pTarget;
};

class Material
{
public:
	//テクスチャデータ
	TextureData textureData;

	//頂点シェーダー
	ShaderFile vsFiledata{ {},{"main"},{"vs_5_0"} };
	//ピクセルシェーダー
	ShaderFile psFiledata{ {},{"main"},{"ps_5_0"} };
	//ドメインシェーダー
	ShaderFile dsFiledata{ {},{"main"},{"ds_5_0"} };
	//ハルシェーダ－
	ShaderFile hsFiledata{ {},{"main"},{"hs_5_0"} };
	//ジオメトリシェーダー
	ShaderFile gsFiledata{ {},{"main"},{"gs_5_0"} };

	//頂点レイアウト
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayouts;

	//ルートパラメータ
	std::vector<D3D12_ROOT_PARAMETER> rootParams;

	//ブレンドステート
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};

	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;

	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

	bool depthFlag = true;

	char PADING[7]{};
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	//void copy(const Material* material);
	//void copy(const Material& material);
	Material* copy();

	Material() = default;
	~Material() = default;

private:
	//コピーコンストラクタ・代入演算子削除
	Material(const Material&) = delete;
};

