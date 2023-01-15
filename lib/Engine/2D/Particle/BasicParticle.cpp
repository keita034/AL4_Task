#include "BasicParticle.h"

//定数バッファの生成
void BasicParticle::CreateShaderResourceView()
{
	//SRVヒープの先頭ハンドルを取得
	cpuHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
	gpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

	UINT incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//ハンドルのポインタずらし
	cpuHandle.ptr += static_cast<unsigned long long>(nextIndex) * incrementSize;
	gpuHandle.ptr += static_cast<unsigned long long>(nextIndex) * incrementSize;

	// シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = static_cast<UINT>(metadata.mipLevels);

	// ハンドルの指す位置にシェーダーリソースビュー作成
	device->CreateShaderResourceView(texBuff.Get(), &srvDesc, cpuHandle);

	nextIndex++;
}

//定数バッファ生成
void BasicParticle::CreatConstBuff()
{
	//定数バッファ生成
	DirectX12Core::GetInstance()->CreateConstBuff(constMapTransform, constBuffTransform);
}

//頂点バッファ・インデックス生成
void BasicParticle::CreatVertexIndexBuffer()
{
	// ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(VerPosColScaRot) * vertexCount);

	// 頂点バッファの設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	// リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	result = device->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(vertBuff.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点１つ分のデータサイズ
	vbView.StrideInBytes = sizeof(VerPosColScaRot);
}

//テクスチャバッファ生成
void BasicParticle::CreatTextureBuffer()
{
	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;//幅
	textureResourceDesc.Height = (UINT)metadata.height;//高さ
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	// テクスチャバッファの生成
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(texBuff.ReleaseAndGetAddressOf()));

	//テクスチャバッファにデータ転送
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		// ミップマップレベルを指定してイメージを取得
		const DirectX::Image* img = scratchImg.GetImage(i, 0, 0);
		// テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,              // 全領域へコピー
			img->pixels,          // 元データアドレス
			(UINT)img->rowPitch,  // 1ラインサイズ
			(UINT)img->slicePitch // 1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

}

void BasicParticle::Initialize(ModelShareVaria& modelShareVaria)
{
	device = DirectX12Core::GetInstance()->GetDevice();
	cmdList = DirectX12Core::GetInstance()->GetCommandList();

	descriptorRange = modelShareVaria.descriptorRange;
	nextIndex = modelShareVaria.nextIndex;
	pipelineState = modelShareVaria.pipelineState;
	rootSignature = modelShareVaria.rootSignature;
	srvHeap = modelShareVaria.srvHeap;
}

//コンストラクタ
void BasicParticle::Load(const wchar_t* filePath)
{
	device = DirectX12Core::GetInstance()->GetDevice();
	cmdList = DirectX12Core::GetInstance()->GetCommandList();

	// WICテクスチャのロード
	result = LoadFromWICFile(
		filePath,
		DirectX::WIC_FLAGS_NONE,
		&metadata, scratchImg);
	assert(SUCCEEDED(result));

	//ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = DirectX::MakeSRGB(metadata.format);

	//テクスチャバッファ生成
	CreatTextureBuffer();

	//シェーダ－リソースビュー生成
	CreateShaderResourceView();

	//頂点バッファ・インデックス生成
	CreatVertexIndexBuffer();

	//定数バッファ生成(2D座標変換行列)
	CreatConstBuff();
}

void BasicParticle::Update()
{

	//寿命が尽きたパーティクルを全削除
	particleDatas.remove_if([](ParticleData& x)
		{
			return x.frame >= x.numFrame;
		});

	//全パーティクル更新
	for (std::forward_list<ParticleData>::iterator it = particleDatas.begin();
		it != particleDatas.end();
		it++)
	{
		//経過フレーム数をカウント
		it->frame++;
		//速度に加速度を加算
		it->velocity = it->velocity + it->accel;
		//速度による移動
		it->position = it->position + it->velocity;
		//進行度を0～1の範囲に換算
		float f = static_cast<float>(it->numFrame / it->frame);

		//スケールの線形補間
		it->scale = (it->eScale - it->sScale) / f;
		it->scale += it->sScale;

		//回転角の線形補間
		it->rotation = (it->eRotation - it->sRotation) / f;
		it->rotation += it->sRotation;

		//カラーの線形補間
		it->color = (it->eColor - it->sColor) / f;
		it->color += it->sColor;
	}

	//頂点バッファへデータ転送
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	uint16_t count = 0;
	for (std::forward_list<ParticleData>::iterator it = particleDatas.begin();
		it != particleDatas.end();
		it++)
	{
		if (count == vertexCount)
		{
			break;
		}

		//座標
		vertMap->pos = it->position;
		vertMap->scale = it->scale;
		vertMap->rotation = it->rotation;
		vertMap->color = it->color;
		//次の頂点へ
		vertMap++;
		count++;
	}

	vertBuff->Unmap(0, nullptr);
}

void BasicParticle::Add(UINT life, AliceMathF::Vector3& position, AliceMathF::Vector3& velocity,
	AliceMathF::Vector3& accel, AliceMathF::Vector2& scale, AliceMathF::Vector2& rotation
	, AliceMathF::Vector4& sColor, AliceMathF::Vector4& eColor)
{
	if (std::distance(particleDatas.begin(), particleDatas.end()) < vertexCount)
	{
		//リストに要素を追加
		particleDatas.emplace_front();
		//追加した要素の参照
		ParticleData& p = particleDatas.front();
		//値のセット
		p.position = position;
		p.velocity = velocity;
		p.accel = accel;
		p.numFrame = life;
		p.sScale = scale.x;
		p.eScale = scale.y;
		p.scale = scale.x;
		p.sRotation = rotation.x;
		p.eRotation = rotation.y;
		p.rotation = rotation.x;
		p.sColor = sColor;
		p.eColor = eColor;
		p.color = sColor;
	}
}

void BasicParticle::Draw(Camera* camera)
{
	assert(camera);

	AliceMathF::Matrix4 mat = camera->GetViewMatrix();
	mat.m[3][0] = 0;
	mat.m[3][1] = 0;
	mat.m[3][2] = 0;
	mat.m[3][3] = 1;

	constMapTransform->matBillboard = mat;
	constMapTransform->mat = camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
	// パイプラインステートとルートシグネチャの設定コマンド
	cmdList->SetPipelineState(pipelineState.Get());
	cmdList->SetGraphicsRootSignature(rootSignature.Get());

	// プリミティブ形状の設定コマンド
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // 三角形リスト

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	// 定数バッファビュー(CBV)の設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());

	// SRVヒープの設定コマンド
	cmdList->SetDescriptorHeaps(1, srvHeap.GetAddressOf());

	// SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	cmdList->SetGraphicsRootDescriptorTable(1, gpuHandle);

	// 描画コマンド
	if (std::distance(particleDatas.begin(), particleDatas.end()) < vertexCount)
	{
		cmdList->DrawInstanced(static_cast<UINT> (std::distance(particleDatas.begin(), particleDatas.end())), 1, 0, 0);
	}
	else
	{
		cmdList->DrawInstanced(vertexCount, 1, 0, 0);

	}

}




