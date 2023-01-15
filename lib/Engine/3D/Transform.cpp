#include "Transform.h"
#include"DirectX12Core.h"

void Transform::Initialize()
{
	//定数バッファ生成(3D座標変換行列)
	DirectX12Core::GetInstance()->CreateConstBuff(constBuffMap, constBuff);
}

void Transform::TransUpdate(Camera* camera)
{
	AliceMathF::Matrix4 matScale, matRot, matTrans;

	//スケール、回転、平行移動行列の計算
	matScale.MakeScaling(scale);
	matRot.MakeRotation(rotation);
	matTrans.MakeTranslation(translation);

	//ワールド行列の合成
	//変形をリセット
	matWorld = AliceMathF::MakeIdentity();
	//ワールド行列にスケーリングを反映
	matWorld *= matScale;
	//ワールド行列に回転を反映
	matWorld *= matRot;
	//ワールド行列に平行移動を反映
	matWorld *= matTrans;
	//親行列の指定がある場合は、掛け算する
	if (parent)
	{
		matWorld *= parent->matWorld;
	}

	//定数バッファに書き込み
	constBuffMap->matWorld = matWorld * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
	constBuffMap->world = matWorld;
	constBuffMap->cameraPos = camera->eye;
}

void Transform::MakeWorldMatrix()
{
	AliceMathF::Matrix4 matScale, matRot, matTrans;

	//スケール、回転、平行移動行列の計算
	matScale.MakeScaling(scale);
	matRot.MakeRotation(rotation);
	matTrans.MakeTranslation(translation);

	//ワールド行列の合成
	//変形をリセット
	matWorld = AliceMathF::MakeIdentity();
	//ワールド行列にスケーリングを反映
	matWorld *= matScale;
	//ワールド行列に回転を反映
	matWorld *= matRot;
	//ワールド行列に平行移動を反映
	matWorld *= matTrans;
	//親行列の指定がある場合は、掛け算する
	if (parent)
	{
		matWorld *= parent->matWorld;
	}
}

ID3D12Resource* Transform::GetconstBuff()
{
	return constBuff.Get();
}

namespace AliceMathF
{
	Matrix4 MakeWorldMatrix4(Transform & transform)
	{

		Matrix4 matWorld = MakeIdentity();

		Matrix4 matScal, matRot, matTrans;

		//スケーリング倍率
		matScal.MakeScaling(transform.scale);

		//回転行列
		matRot.MakeRotation(transform.rotation);

		// matWorld_に移動量を掛け算
		matTrans.MakeTranslation(transform.translation);

		matWorld = matScal * matRot * matTrans;

		if (transform.parent)
		{
			matWorld *= transform.parent->matWorld;
		}

		return matWorld;
	}

	Vector3 GetWorldPosition(Transform & transform)
	{
		//ワールド座標を入れる変数
		Vector3 worldPos;
		//ワールド行列の平行移動成分を取得(ワールド座標)
		worldPos.x = transform.matWorld.m[3][0];
		worldPos.y = transform.matWorld.m[3][1];
		worldPos.z = transform.matWorld.m[3][2];

		return worldPos;
	}
}