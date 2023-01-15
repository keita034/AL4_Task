#include "Sprite3D.h"


void Sprite3D::Initialize()
{
	SpriteInitialize();

	//�萔�o�b�t�@����
	DirectX12Core::GetInstance()->CreateConstBuff(constMapTransform, constBuffTransform);
	
	CreatVertexIndexBuffer();
}

void Sprite3D::Update(Transform& transform, Camera* camera, BillboardFlag flag, const AliceMathF::Vector4& color)
{
		//�J���[
	for (int i = 0; i < 4; i++)
	{
		vertMap[i].color = color;
	}

	//���s�ړ��s��
	mTrans.MakeTranslation(transform.translation);
	//��]�s��
	mRot.MakeRotation(transform.rotation);
	//�X�P�[���s��
	mScale.MakeScaling(transform.scale);

	switch (flag)
	{
	case NonBillboard:
		constMapTransform->mat = mScale * mRot * mTrans * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
		break;
	case AllBillboard:
	{
		AliceMathF::Matrix4 mat = camera->GetViewMatrix();

		mat.m[3][0] = 0;
		mat.m[3][1] = 0;
		mat.m[3][2] = 0;
		mat.m[3][3] = 1;

		constMapTransform->mat = mScale * mRot * mat * mTrans * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
		break;
	}
	case XBillboard:
	{
		AliceMathF::Matrix4 mat = camera->GetViewMatrix();

		mat.m[1][0] = 0;
		mat.m[1][1] = 1;
		mat.m[1][2] = 0;
		mat.m[1][3] = 0;

		mat.m[3][0] = 0;
		mat.m[3][1] = 0;
		mat.m[3][2] = 0;
		mat.m[3][3] = 1;

		constMapTransform->mat = mScale * mRot * mat * mTrans * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
		break;
	}
	case YBillboard:
	{
		AliceMathF::Matrix4 mat = camera->GetViewMatrix();

		mat.m[0][0] = 1;
		mat.m[0][1] = 0;
		mat.m[0][2] = 0;
		mat.m[0][3] = 0;

		mat.m[3][0] = 0;
		mat.m[3][1] = 0;
		mat.m[3][2] = 0;
		mat.m[3][3] = 1;


		constMapTransform->mat = mScale * mRot * mat * mTrans * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
		break;
	}
	case XYBillboard:
	{
		AliceMathF::Matrix4 mat = camera->GetViewMatrix();

		mat.m[2][0] = 0;
		mat.m[2][1] = 0;
		mat.m[2][2] = 1;
		mat.m[2][3] = 0;

		mat.m[3][0] = 0;
		mat.m[3][1] = 0;
		mat.m[3][2] = 0;
		mat.m[3][3] = 1;

		constMapTransform->mat = mScale * mRot * mat * mTrans * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
		break;
	}
	default:
		assert(0);
		break;
	}
}

//�`��
void Sprite3D::Draw(TextureData& textureData, Material* material)
{
	if (InitializeFlag)
	{

		//�摜�̔����̃T�C�Y
		float halfW = static_cast<float>(textureData.width) * 0.0625f;
		float halfH = static_cast<float>(textureData.height) * 0.0625f;

		// ���_�f�[�^
		PosUvColor vertices[] =
		{//		x		y		z		u	v
			{{-halfW, halfH, 0.0f},{0.0f,0.0f},{1.0f,1.0f,1.0f,1.0f}},//����C���f�b�N�X0
			{{-halfW, -halfH, 0.0f},{0.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},//�����C���f�b�N�X1
			{{halfW, halfH, 0.0f},{1.0f,0.0f},{1.0f,1.0f,1.0f,1.0f}},//�E��C���f�b�N�X2
			{{halfW, -halfH, 0.0f},{1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},//�E���C���f�b�N�X3
		};

		// �C���f�b�N�X�f�[�^
		uint16_t indices[] =
		{
			1, 0, 3, // �O�p�`1��
			2, 3, 0, // �O�p�`2��
		};


		//���_�o�b�t�@�ւ̃f�[�^�]��
		for (int i = 0; i < _countof(vertices); i++)
		{
			vertMap[i] = vertices[i]; // ���W���R�s�[
		}

		//�C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
		for (int i = 0; i < _countof(indices); i++)
		{
			indexMap[i] = indices[i];   // �C���f�b�N�X���R�s�[
		}
	}

	SpriteDraw(textureData, material);
}

