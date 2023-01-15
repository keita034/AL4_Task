#pragma once
#include"ErrorException.h"


class DirectX12Core
{
private:

	HRESULT result;
	char PADDING[4];
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;


	//バックバッファ
	std::vector< Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;

	//バリアーデスク
	D3D12_RESOURCE_BARRIER barrierDesc{};

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	//フェンスの生成
	UINT64 fenceVal = 0;

	//スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

	//クリアーカラー
	FLOAT clearColor[4] = { 0.1f, 0.25f, 0.5f, 0.0f };

	//ビューポート
	D3D12_VIEWPORT viewport{};

	//シザー矩形
	D3D12_RECT scissorRect{};

	//深度バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;

	//深度ビュー用デスクプリタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;

	static DirectX12Core* DirectX12Core_;

public:

	//シングルトンインスタンスの取得
	static DirectX12Core* GetInstance();

	//初期化
	void DirectXInitialize();

	//描画準備
	void BeginDraw();

	//描画後始末
	void EndDraw();

	//コマンド後始末
	void ExecuteCommand();

	void Destroy();
	//セッター

	//背景の色変え(RGBA)
	void SetBackScreenColor(float red, float green, float blue, float alpha);

	//ゲッター
	Microsoft::WRL::ComPtr<ID3D12Device> GetDevice();
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList();
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetRtvHeap();
	Microsoft::WRL::ComPtr<IDXGISwapChain4> GetSwapChain();
	D3D12_CPU_DESCRIPTOR_HANDLE GetRtvHandle();
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetCmdAllocator();
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetCommandQueue();
	D3D12_DESCRIPTOR_HEAP_DESC GetRtvHeapDesc();
	Microsoft::WRL::ComPtr<IDXGIFactory7> GetDxgiFactory();
	Microsoft::WRL::ComPtr<ID3D12Fence> GetFence();
	UINT64 GetFenceVal();

#pragma region テンプレート関数

	//定数バッファ作成
	template<typename T>
	void CreateConstBuff(T*& buff, Microsoft::WRL::ComPtr<ID3D12Resource>& constBuff)
	{

		// 頂点バッファの設定
		D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
		// リソース設定
		D3D12_RESOURCE_DESC resDesc{};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = (sizeof(T) + 0xff) & ~0xff; // 頂点データ全体のサイズ
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		//定数バッファの生成
		result = device->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(constBuff.ReleaseAndGetAddressOf()));
		assert(SUCCEEDED(result));

		//定数バッファのマッピング
		result = constBuff->Map(0, nullptr, (void**)&buff);//マッピング
		assert(SUCCEEDED(result));

	}

#pragma endregion

private:

	//DXGIまわり初期化
	HRESULT InitializeDXGIDevice();

	//最終的なレンダーターゲットの生成
	HRESULT CreatFinalRenderTarget();

	//スワップチェインの生成
	HRESULT CreateSwapChain();

	//コマンドまわり初期化
	HRESULT InitializeCommand();

	//フェンス生成
	HRESULT CreateFence();

	//深度関係生成
	HRESULT CreatDepthBuffer();

	//デバッグレイヤーを有効にする
	void EnableDebugLayer();

	//コンストラクタ・デストラクタ
	DirectX12Core() = default;
	~DirectX12Core() = default;
};