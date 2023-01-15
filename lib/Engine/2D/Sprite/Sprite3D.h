#pragma once
//#DirectX�̂��
#include"ErrorException.h"

//����.h
#include"AliceMathUtility.h"
#include"WindowsApp.h"
#include"DirectX12Core.h"
#include "Camera.h"
#include"Sprite.h"
#include"DefaultMaterial.h"

//pragma comment



//using namespace


class Sprite3D : public Sprite
{
private:

	AliceMathF::Matrix4 mTrans, mRot, mScale;

public:

	Sprite3D() = default;
	~Sprite3D() = default;

	//������
	virtual void Initialize()override;

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update(Transform& transform, Camera* camera, BillboardFlag flag = NonBillboard, const AliceMathF::Vector4& color = { 1.0f,1.0f,1.0f,1.0f });

	///<summary>
	///�`��
	///</summary>
	virtual void Draw(TextureData& textureData, Material* material = &DEFAULT_SPRITE3D_MATERIAL)override;

private:



	//�R�s�[�R���X�g���N�^�E������Z�q�폜
	Sprite3D& operator=(const Sprite3D&) = delete;
	Sprite3D(const Sprite3D&) = delete;
};

