#include "Material.h"

void Material::Initialize()
{
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectX12Core::GetInstance()->GetDevice();

	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob; // ピクセルシェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> dsBlob; // ドメインシェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> hsBlob; // ハルシェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> gsBlob; // ジオメトリシェーダオブジェクト

	bool vsflag = false;
	bool psflag = false;
	bool dsflag = false;
	bool hsflag = false;
	bool gsflag = false;

	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

#pragma region 頂点シェーダー

	//頂点シェーダー
	if (vsFiledata.pFileName.size() > 4)
	{
		// 頂点シェーダの読み込みとコンパイル
		result = D3DCompileFromFile(
			vsFiledata.pFileName.c_str(), // シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
			vsFiledata.pEntrypoint.c_str(), vsFiledata.pTarget.c_str(), // エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
			0,
			&vsBlob, &errorBlob);

		// エラーなら
		if (FAILED(result))
		{
			// errorBlobからエラー内容をstring型にコピー
			std::string error;
			error.resize(errorBlob->GetBufferSize());
			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				error.begin());
			error += "\n";
			// エラー内容を出力ウィンドウに表示
			OutputDebugStringA(error.c_str());
			assert(0);
		}

		vsflag = true;
	}

#pragma endregion

#pragma region ピクセルシェーダー

	//ピクセルシェーダ－
	if (psFiledata.pFileName.size() > 4)
	{
		// ピクセルシェーダの読み込みとコンパイル
		result = D3DCompileFromFile(
			psFiledata.pFileName.c_str(), // シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
			psFiledata.pEntrypoint.c_str(), psFiledata.pTarget.c_str(), // エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
			0,
			&psBlob, &errorBlob);

		// エラーなら
		if (FAILED(result))
		{
			// errorBlobからエラー内容をstring型にコピー
			std::string error;
			error.resize(errorBlob->GetBufferSize());
			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				error.begin());
			error += "\n";
			// エラー内容を出力ウィンドウに表示
			OutputDebugStringA(error.c_str());
			assert(0);
		};

		psflag = true;
	}

#pragma endregion

#pragma region ドメインシェーダー

	//ドメインシェーダー
	if (dsFiledata.pFileName.size() > 4)
	{
		// ピクセルシェーダの読み込みとコンパイル
		result = D3DCompileFromFile(
			dsFiledata.pFileName.c_str(), // シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
			dsFiledata.pEntrypoint.c_str(), dsFiledata.pTarget.c_str(), // エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
			0,
			&dsBlob, &errorBlob);

		// エラーなら
		if (FAILED(result))
		{
			// errorBlobからエラー内容をstring型にコピー
			std::string error;
			error.resize(errorBlob->GetBufferSize());
			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				error.begin());
			error += "\n";
			// エラー内容を出力ウィンドウに表示
			OutputDebugStringA(error.c_str());
			assert(0);
		};

		dsflag = true;
	}

#pragma endregion

#pragma region ハルシェーダー

	//ハルシェーダー
	if (hsFiledata.pFileName.size() > 4)
	{
		// ピクセルシェーダの読み込みとコンパイル
		result = D3DCompileFromFile(
			hsFiledata.pFileName.c_str(), // シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
			hsFiledata.pEntrypoint.c_str(), hsFiledata.pTarget.c_str(), // エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
			0,
			&hsBlob, &errorBlob);

		// エラーなら
		if (FAILED(result))
		{
			// errorBlobからエラー内容をstring型にコピー
			std::string error;
			error.resize(errorBlob->GetBufferSize());
			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				error.begin());
			error += "\n";
			// エラー内容を出力ウィンドウに表示
			OutputDebugStringA(error.c_str());
			assert(0);
		};

		hsflag = true;
	}

#pragma endregion

#pragma region ジオメトリシェーダー

	if (gsFiledata.pFileName.size() > 4)
	{
		// ピクセルシェーダの読み込みとコンパイル
		result = D3DCompileFromFile(
			gsFiledata.pFileName.c_str(), // シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
			gsFiledata.pEntrypoint.c_str(), gsFiledata.pTarget.c_str(), // エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
			0,
			&gsBlob, &errorBlob);

		// エラーなら
		if (FAILED(result))
		{
			// errorBlobからエラー内容をstring型にコピー
			std::string error;
			error.resize(errorBlob->GetBufferSize());
			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				error.begin());
			error += "\n";
			// エラー内容を出力ウィンドウに表示
			OutputDebugStringA(error.c_str());
			assert(0);
		};

		gsflag = true;
	}

#pragma endregion
	
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// シェーダーの設定
	if (vsflag)
	{
		pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
		pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	}
	if (psflag)
	{
		pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
		pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
	}
	if (dsflag)
	{
		pipelineDesc.DS.pShaderBytecode = dsBlob->GetBufferPointer();
		pipelineDesc.DS.BytecodeLength = dsBlob->GetBufferSize();
	}
	if (hsflag)
	{
		pipelineDesc.HS.pShaderBytecode = hsBlob->GetBufferPointer();
		pipelineDesc.HS.BytecodeLength = hsBlob->GetBufferSize();
	}
	if (gsflag)
	{
		pipelineDesc.GS.pShaderBytecode = gsBlob->GetBufferPointer();
		pipelineDesc.GS.BytecodeLength = gsBlob->GetBufferSize();
	}

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // カリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	pipelineDesc.BlendState.RenderTarget[0] = blenddesc;

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.NumElements = static_cast<UINT>(inputLayouts.size());
	pipelineDesc.InputLayout.pInputElementDescs = inputLayouts.data();

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
	// //D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE
	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = depthFlag;//深度テストを行う
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;//深度フォーマット
	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	//テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//横繰り返し（タイリング）
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//縦繰り返し（タイリング）
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//奥行繰り返し（タイリング）
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//全てリニア補間
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//ピクセルシェーダからのみ使用可能

	// ルートシグネチャ
	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams.data(); //ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = static_cast<UINT>(rootParams.size());        //ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ルートシグネチャのシリアライズ
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, rootSigBlob.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf());
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(rootSignature.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();

	// パイプランステートの生成
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
}

//void Material::copy(const Material* material)
//{
//	memcpy(this, material, sizeof(material));
//}
//
//void Material::copy(const Material& material)
//{
//	memcpy(this, &material, sizeof(material));
//}

Material* Material::copy()
{
	Material* tmp = new Material;
	*tmp = *this;
	return tmp;
}

