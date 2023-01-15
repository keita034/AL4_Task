#pragma once
//#DirectX�̂��
#include"ErrorException.h"

//����.h
#include"AliceMathUtility.h"
#include"WindowsApp.h"
#include"DirectX12Core.h"
#include"Sprite.h"
#include"DefaultMaterial.h"
//pragma comment



//using namespace

class Sprite2D : public Sprite
{
private:


public:

	Sprite2D() = default;
	~Sprite2D() = default;

	//������
	virtual void Initialize()override;

	///<summary>
	///�X�V
	///</summary>
	///<param name="pos">���W</param>
	///<param name="rot">��]</param>
	///<param name="scale">�g�嗦</param>
	/// ///<param name="color"> �J���[</param>
	virtual void Update(Transform& transform, const AliceMathF::Vector4& color = {1.0f,1.0f,1.0f,1.0f});

	///<summary>
	///�`��
	///</summary>
	virtual void Draw(TextureData& textureData, Material* material = &DEFAULT_SPRITE2D_MATERIAL)override;

private:


	//�R�s�[�R���X�g���N�^�E������Z�q�폜
	Sprite2D& operator=(const Sprite2D&) = delete;
	Sprite2D(const Sprite2D&) = delete;

};