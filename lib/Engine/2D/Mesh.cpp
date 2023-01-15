#include"Mesh.h"

Mesh* Mesh::mesh = nullptr;

#pragma region パブリック

//線を描画する
void Mesh::DrawLine(float x1, float y1, float x2, float y2, AliceMathF::Vector4 color)
{
	assert(lineCount < lineMaxCount);

	// 頂点データ
	std::vector <PosColor > vertices = {
	  {{x1, y1, 0.0f}, {color.x, color.y, color.z, color.w}},
	  {{x2, y2, 0.0f}, {color.x, color.y, color.z, color.w}},
	};

	UINT indxcount = lineCount * lineVertexCount;

	//頂点バッファへのデータ転送
	copy(vertices.begin(), vertices.end(), &lineBuff->vertMap[indxcount]);

	// パイプラインステートの設定
	directX12Core->GetCommandList()->SetPipelineState(linePipelineSet[blendMode]->pipelineState.Get());

	// ルートシグネチャの設定
	directX12Core->GetCommandList()->SetGraphicsRootSignature(linePipelineSet[blendMode]->rootSignature.Get());

	// プリミティブ形状の設定コマンド
	directX12Core->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST); //三角形リスト

	// 頂点バッファビューの設定コマンド
	directX12Core->GetCommandList()->IASetVertexBuffers(0, 1, &lineBuff->vbView);

	//定数バッファビュー
	directX12Core->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());

	// 描画コマンド
	directX12Core->GetCommandList()->DrawInstanced(lineVertexCount, 1, indxcount, 0);

	lineCount++;
}

//三角形を描画する
void Mesh::DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, AliceMathF::Vector4 color, int fillFlag)
{
	if (fillFlag)
	{
		DrawTriangleFill(x1, y1, x2, y2, x3, y3, color);
	}
	else
	{
		DrawLine(x1, y1, x2, y2, color);
		DrawLine(x2, y2, x3, y3, color);
		DrawLine(x3, y3, x1, y1, color);

	}
}

//四角形を描画する
void Mesh::DrawBox(float x, float y, float radiusX, float radiusY, float angle, AliceMathF::Vector4 color, int fillFlag)
{
	if (fillFlag)
	{
		DrawBoxFill(x, y, radiusX, radiusY, angle, color);

	}
	else
	{

		DrawLine(x - radiusX, y - radiusY , x + radiusX, y - radiusY, color);//左上から右上
		DrawLine(x - radiusX, y - radiusY , x - radiusX, y + radiusY, color);//左上から左下
		DrawLine(x + radiusX, y - radiusY , x + radiusX, y + radiusY, color);//右上から右下
		DrawLine(x - radiusX, y + radiusY , x + radiusX, y + radiusY, color);//左下から右下
	}
}

//楕円を描画する
void Mesh::DrawEllipse(float x, float y, float radiusX, float radiusY, float angle, AliceMathF::Vector4 color, int fillMode)
{
	constexpr UINT NumDiv = 64; // 分割数
	float RadianPerDivision = AliceMathF::AX_2PI / float(NumDiv); // 分割1単位の角度

	//分割点分割数より1つ多くしておいて1周回るようにする
	std::array<float, NumDiv + 1> pointsX{};
	std::array<float, NumDiv + 1> pointsY{};
	float angleCos = cos(angle);
	float angleSin = sin(angle);
	for (size_t i = 0; i < NumDiv; ++i)
	{
		float baseX = cos(RadianPerDivision * static_cast<float>(i)) * radiusX;
		float baseY = sin(RadianPerDivision * static_cast<float>(i)) * radiusY;
		pointsX[i] = float(baseX * angleCos - baseY * angleSin + x);
		pointsY[i] = float(baseY * angleCos + baseX * angleSin + y);
	}

	//最後と最初は一緒
	(*pointsX.rbegin()) = (*pointsX.begin());
	(*pointsY.rbegin()) = (*pointsY.begin());

	if (fillMode)
	{
		for (size_t index = 0; index < NumDiv; ++index)
		{
			DrawTriangleFill(x, y, pointsX[index], pointsY[index], pointsX[index + 1], pointsY[index + 1], color);
		}
	}
	else
	{
		for (size_t index = 0; index < NumDiv; ++index)
		{
			DrawLine(pointsX[index], pointsY[index], pointsX[index + 1], pointsY[index + 1], color);
		}
	}
}

//カウント初期化
void Mesh::DrawReset()
{
	triangleCount = 0;
	lineCount = 0;
	boxCount = 0;
}

//ブレンドモードを設定する
void Mesh::SetBlendMode(BlendMode mode)
{
	blendMode = (UINT)mode;
}

//色コードを取得する
AliceMathF::Vector4 Mesh::GetColor(int red, int blue, int green, int alpha)
{
	AliceMathF::Vector4 color{};

	color.x = static_cast<float>(red) / 255.0f;
	color.y = static_cast<float>(blue) / 255.0f;
	color.z = static_cast<float>(green) / 255.0f;
	color.w = static_cast<float>(alpha) / 255.0f;

	return color;
}

AliceMathF::Vector4 Mesh::GetColor(AliceMathF::Vector4 color)
{
	AliceMathF::Vector4 result{};

	result.x = static_cast<float>(color.x) / 255.0f;
	result.y = static_cast<float>(color.y) / 255.0f;
	result.z = static_cast<float>(color.z) / 255.0f;
	result.w = static_cast<float>(color.w) / 255.0f;

	return result;
}


//コンストラクタ
Mesh::Mesh()
{
	directX12Core = DirectX12Core::GetInstance();

	//各種パイプライン生成
	CreatArryPipeline();

	//三角形バッファ
	UINT vertexCount = triangleVertexCount * triangleMaxCount;
	UINT indexCount = triangleIndexCount * triangleMaxCount;
	triangleBuff = CreateBuff(vertexCount, indexCount);

	//線バッファ
	vertexCount = lineVertexCount * lineMaxCount;
	indexCount = lineIndexCount * lineMaxCount;
	lineBuff = CreateBuff(vertexCount, indexCount);

	//四角形バッファ
	vertexCount = boxVertexCount * boxMaxCount;
	indexCount = boxIndexCount * boxMaxCount;
	boxBuff = CreateBuff(vertexCount, indexCount);

	CreatConstBuff();

}

#pragma endregion

#pragma region プライベート

Mesh* Mesh::GetInstance()
{
	if (mesh == nullptr)
	{
		mesh = new Mesh();
	}

	return mesh;
}

void Mesh::Destroy()
{
	delete mesh;
}

//三角形を描画する(中身塗りつぶし)
void Mesh::DrawTriangleFill(float x1, float y1, float x2, float y2, float x3, float y3, AliceMathF::Vector4 color)
{
	assert(triangleCount < triangleMaxCount);

	// 頂点データ
	std::vector <PosColor > vertices = {
	  {{x1, y1, 0.0f}, {color.x, color.y, color.z, color.w}},
	  {{x2, y2, 0.0f}, {color.x, color.y, color.z, color.w}},
	  {{x3, y3, 0.0f}, {color.x, color.y, color.z, color.w}},
	};

	UINT vertexCount = triangleCount * triangleVertexCount;

	//頂点バッファへのデータ転送
	copy(vertices.begin(), vertices.end(), &triangleBuff->vertMap[vertexCount]);

	// パイプラインステートの設定
	directX12Core->GetCommandList()->SetPipelineState(trianglePipelineSet[blendMode]->pipelineState.Get());

	// ルートシグネチャの設定
	directX12Core->GetCommandList()->SetGraphicsRootSignature(trianglePipelineSet[blendMode]->rootSignature.Get());

	// プリミティブ形状の設定コマンド
	directX12Core->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //三角形リスト

	// 頂点バッファビューの設定コマンド
	directX12Core->GetCommandList()->IASetVertexBuffers(0, 1, &triangleBuff->vbView);

	//定数バッファビュー
	directX12Core->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());

	// 描画コマンド
	directX12Core->GetCommandList()->DrawInstanced(triangleVertexCount, 1, vertexCount, 0);

	// 使用カウント上昇
	triangleCount++;
}

//四角形を描画する(中身塗りつぶし)
void Mesh::DrawBoxFill(float x, float y, float radiusX, float radiusY, float angle, AliceMathF::Vector4 color)
{
	assert(boxCount < boxMaxCount);

	float left = -radiusX;
	float top = -radiusY;
	float right = radiusX;
	float bottom = radiusY;

	// 頂点データ
	std::vector <PosColor > vertices = {
	  {{left, bottom, 0.0f},{color.x, color.y, color.z, color.w}},//左下
	  {{left, top, 0.0f},{color.x, color.y, color.z, color.w}},//左上
	  {{right, top, 0.0f},{color.x, color.y, color.z, color.w}},//右上
	  {{right, bottom, 0.0f},{color.x, color.y, color.z, color.w}},//右下
	};

	//インデックス
	std::vector<uint16_t> indices = { 0, 1, 2, 2, 3, 0 };

	//回転
	for (auto& vertex : vertices)
	{
		//回転
		vertex.pos = {
		  vertex.pos.x * cosf(angle) + vertex.pos.y * -sinf(angle),
		  vertex.pos.x * sinf(angle) + vertex.pos.y * cosf(angle), vertex.pos.z };

		//平行移動
		vertex.pos.x += x;
		vertex.pos.y += y;
	}

	UINT vertexCount = boxCount * boxVertexCount;
	UINT indxCount = boxCount * boxIndexCount;

	//頂点バッファへのデータ転送
	std::copy(vertices.begin(), vertices.end(), &boxBuff->vertMap[vertexCount]);

	//インデックスバッファへのデータ転送
	std::copy(indices.begin(), indices.end(), &boxBuff->indexMap[indxCount]);

	//パイプラインステートの設定
	directX12Core->GetCommandList()->SetPipelineState(trianglePipelineSet[blendMode]->pipelineState.Get());

	//ルートシグネチャの設定
	directX12Core->GetCommandList()->SetGraphicsRootSignature(trianglePipelineSet[blendMode]->rootSignature.Get());

	//プリミティブ形状の設定コマンド
	directX12Core->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //三角形リスト

	//頂点バッファの設定
	directX12Core->GetCommandList()->IASetVertexBuffers(0, 1, &boxBuff->vbView);

	//インデックスバッファの設定
	directX12Core->GetCommandList()->IASetIndexBuffer(&boxBuff->ibView);

	//定数バッファビュー
	directX12Core->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());

	//描画コマンド
	directX12Core->GetCommandList()->DrawIndexedInstanced(boxIndexCount, 1, static_cast<UINT>(indxCount), static_cast<INT>(vertexCount), 0);

	//使用カウント上昇
	boxCount++;
}

//各種パイプラインセット生成
void Mesh::CreatArryPipeline()
{
	//三角形形状用パイプラインセット
	trianglePipelineSet[(UINT)BlendMode::AX_BLENDMODE_NOBLEND] = CreatPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, BlendMode::AX_BLENDMODE_NOBLEND);
	trianglePipelineSet[(UINT)BlendMode::AX_BLENDMODE_ALPHA] = CreatPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, BlendMode::AX_BLENDMODE_ALPHA);
	trianglePipelineSet[(UINT)BlendMode::AX_BLENDMODE_ADD] = CreatPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, BlendMode::AX_BLENDMODE_ADD);
	trianglePipelineSet[(UINT)BlendMode::AX_BLENDMODE_SUB] = CreatPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, BlendMode::AX_BLENDMODE_SUB);
	trianglePipelineSet[(UINT)BlendMode::AX_BLENDMODE_MULA] = CreatPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, BlendMode::AX_BLENDMODE_MULA);
	trianglePipelineSet[(UINT)BlendMode::AX_BLENDMODE_INVSRC] = CreatPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, BlendMode::AX_BLENDMODE_INVSRC);

	//ライン形状用パイプラインセット
	linePipelineSet[(UINT)BlendMode::AX_BLENDMODE_NOBLEND] = CreatPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::AX_BLENDMODE_NOBLEND);
	linePipelineSet[(UINT)BlendMode::AX_BLENDMODE_ALPHA] = CreatPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::AX_BLENDMODE_ALPHA);
	linePipelineSet[(UINT)BlendMode::AX_BLENDMODE_ADD] = CreatPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::AX_BLENDMODE_ADD);
	linePipelineSet[(UINT)BlendMode::AX_BLENDMODE_SUB] = CreatPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::AX_BLENDMODE_SUB);
	linePipelineSet[(UINT)BlendMode::AX_BLENDMODE_MULA] = CreatPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::AX_BLENDMODE_MULA);
	linePipelineSet[(UINT)BlendMode::AX_BLENDMODE_INVSRC] = CreatPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::AX_BLENDMODE_INVSRC);
}

//定数バッファ生成(2D座標変換行列)
void Mesh::CreatConstBuff()
{
	//定数バッファ生成
	directX12Core->CreateConstBuff(constMapTransform, constBuffTransform);

	AliceMathF::MakeOrthogonalL(
		0.0f, static_cast<float>(WindowsApp::GetInstance()->GetWindowWidth()),
		static_cast<float>(WindowsApp::GetInstance()->GetWindowHeight()), 0.0f,
		0.0f, 1.0f, constMapTransform->mat);

}

///<summary>
///パイプライン作成
///</summary>
///<param name="type">プリミティブ形状</param>
///<param name="mode">ブレンドモード数</param>
///<returns>パイプライン</returns>
std::unique_ptr<PipelineSet> Mesh::CreatPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE type, BlendMode mode)
{
	HRESULT result;

	std::unique_ptr< PipelineSet>set = std::make_unique<PipelineSet>();

	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};


	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/2D/MeshVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
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

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/2D/MeshPS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
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
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		//座標
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//uv座標 
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};


	// シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // カリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	// ブレンドステート
	//共通設定(アルファ値)
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//テストの値を0%使う

	switch (mode)
	{
	case BlendMode::AX_BLENDMODE_NOBLEND:
		blenddesc.BlendEnable = false;// ブレンドを有効

		break;

	case BlendMode::AX_BLENDMODE_ALPHA:

		blenddesc.BlendEnable = true;// ブレンドを有効
		// 半透明合成
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;// 加算
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;;// 1.0f-ソースのアルファ値

		break;

	case BlendMode::AX_BLENDMODE_ADD:

		blenddesc.BlendEnable = true;// ブレンドを有効
		// 加算合成
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;// 加算
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースの値を100% 使う
		blenddesc.DestBlend = D3D12_BLEND_ONE;// デストの値を100% 使う

		break;

	case BlendMode::AX_BLENDMODE_SUB:

		//共通設定(アルファ値)
		blenddesc.BlendEnable = true;// ブレンドを有効
		// 減算合成
		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;// デストからソースを減算
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースの値を100% 使う
		blenddesc.DestBlend = D3D12_BLEND_ONE;// デストの値を100% 使う

		break;

	case BlendMode::AX_BLENDMODE_MULA:

		//共通設定(アルファ値)
		blenddesc.BlendEnable = true;// ブレンドを有効
		//乗算
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_ZERO;
		blenddesc.DestBlend = D3D12_BLEND_SRC_COLOR;

		break;

	case BlendMode::AX_BLENDMODE_INVSRC:

		//共通設定(アルファ値)
		blenddesc.BlendEnable = true;// ブレンドを有効
		// 色反転
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;// 加算
		blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;// 1.0f-デストカラーの値
		blenddesc.DestBlend = D3D12_BLEND_ZERO;// 使わない

		break;

	case BlendMode::AX_BLENDMODE_MAX:
		assert(0);
		break;
	}

	pipelineDesc.BlendState.RenderTarget[0] = blenddesc;

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = type;
	//D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE
	//D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE

	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams = {};
	//定数バッファ0番
	rootParams.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//種類
	rootParams.Descriptor.ShaderRegister = 0;//定数バッファバッファ番号
	rootParams.Descriptor.RegisterSpace = 0;//デフォルト値
	rootParams.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams; //ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = 1;        //ルートパラメータ数

	// ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = directX12Core->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(set->rootSignature.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = set->rootSignature.Get();

	// パイプランステートの生成
	result = directX12Core->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(set->pipelineState.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	return set;
}

///<summary>
///バッファ作成
///</summary>
///<param name="vertexCount">頂点数</param>
///<param name="indexCount">インデックス数</param>
///<returns>バッファ</returns>
std::unique_ptr <Buff> Mesh::CreateBuff(UINT vertexCount, UINT indexCount)
{
	HRESULT result;
	std::unique_ptr <Buff> buff = std::make_unique<Buff>();

	if (vertexCount > 0)
	{
		// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
		UINT sizeVB = static_cast<UINT>(sizeof(PosColor) * vertexCount);

		// 頂点バッファの設定
		D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
		// リソース設定
		D3D12_RESOURCE_DESC resDesc{};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeVB; // 頂点データ全体のサイズ
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 頂点バッファの生成
		result = directX12Core->GetDevice()->CreateCommittedResource(
			&heapProp, // ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&resDesc, // リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(buff->vertBuff.ReleaseAndGetAddressOf()));
		assert(SUCCEEDED(result));

		// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
		result = buff->vertBuff->Map(0, nullptr, (void**)&buff->vertMap);
		assert(SUCCEEDED(result));

		// 頂点バッファビューの作成
		// GPU仮想アドレス
		buff->vbView.BufferLocation = buff->vertBuff->GetGPUVirtualAddress();
		// 頂点バッファのサイズ
		buff->vbView.SizeInBytes = sizeVB;
		// 頂点１つ分のデータサイズ
		buff->vbView.StrideInBytes = sizeof(PosColor);
	}

	if (indexCount > 0)
	{
		// インデックスデータのサイズ
		UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexCount);
		// 頂点バッファの設定
		D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
		//リソース設定
		D3D12_RESOURCE_DESC resDesc{};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeIB;//インデックス情報が入る分のサイズ
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		//インデックスバッファの生成
		result = directX12Core->GetDevice()->CreateCommittedResource(
			&heapProp,//ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&resDesc,//リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(buff->indexBuff.ReleaseAndGetAddressOf()));

		//インデックスバッファをマッピング
		result = buff->indexBuff->Map(0, nullptr, (void**)&buff->indexMap);
		//マッピング解除
		buff->indexBuff->Unmap(0, nullptr);

		//インデックスバッファビューの作成
		buff->ibView.BufferLocation = buff->indexBuff->GetGPUVirtualAddress();
		buff->ibView.Format = DXGI_FORMAT_R16_UINT;
		buff->ibView.SizeInBytes = sizeIB;
		assert(SUCCEEDED(result));
	}

	return buff;
}

#pragma endregion
