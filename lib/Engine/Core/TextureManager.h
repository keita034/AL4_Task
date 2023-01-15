#pragma once
#include"ErrorException.h"
#include"AliceMathUtility.h"

class TextureManager
{
private:

	//デスクプリタヒープ
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> srvHeap;
	//デスクプリタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange;
	char PADDING[4];

	Microsoft::WRL::ComPtr<ID3D12Device> device;

	static TextureManager* textureManager;

	//テクスチャ数
	UINT nextTexture;

	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};


public:

	/// <summary>
	/// テクスチャをロードします
	/// </summary>
	/// <param name="filepath">テクスチャのファイルパス</param>
	/// <returns>テクスチャハンドル</returns>
	TextureData LoadTexture(const wchar_t* filepath);

	/// <summary>
	/// テクスチャをロードします(ポインタ)
	/// </summary>
	/// <param name="filepath">テクスチャのファイルパス</param>
	/// <returns>テクスチャハンドル</returns>
	void LoadTexturePtr(const wchar_t* filepath, TextureData* tex);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// インスタンスを所得
	/// </summary>
	/// <returns>インスタンス</returns>
	static TextureManager* GetInstance();

	/// <summary>
	/// インスタンスを解放
	/// </summary>
	void Destroy();

private:

	TextureManager() = default;
	~TextureManager() = default;


	Microsoft::WRL::ComPtr<ID3D12Resource>CreateTexBuff(DirectX::TexMetadata& metadata, DirectX::ScratchImage& scratchImg);

	D3D12_GPU_DESCRIPTOR_HANDLE CreateShaderResourceView(ID3D12Resource* texBuff, DirectX::TexMetadata& metadata);
};

