#include "Light.h"

Light* Light::Create()
{
	//3Dオブジェクトのインスタンス生成
	Light* instance = new Light();
	//初期化
	instance->Initialize();
	//生成したインスタンスを返す
	return instance;
}

void Light::Initialize()
{
	device = DirectX12Core::GetInstance()->GetDevice();

	//定数バッファ
	DirectX12Core::GetInstance()->CreateConstBuff(constMap, constBuff);
	//定数バッファへデータ転送
	TransferConstBuffer();
}

void Light::TransferConstBuffer(){
	constMap->lightv = -lightdir;
	constMap->lightcolor = lightcolor;
}

void Light::SetLightDir(AliceMathF::Vector3& lightdir_)
{
	//正規化してセット
	lightdir = lightdir_.normal();
	dirty = true;
}

void Light::SetLightColor(AliceMathF::Vector4& lightcolor_)
{
	lightcolor = lightcolor_;
	dirty = true;
}

void Light::Update()
{
	//値の更新があった時だけ定数バッファに転送する
	if (dirty)
	{
		TransferConstBuffer();
		dirty = false;
	}
}

void Light::SetConstBufferView(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff->GetGPUVirtualAddress());
}
