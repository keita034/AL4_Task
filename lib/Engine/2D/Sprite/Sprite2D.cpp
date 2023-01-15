#include "Sprite2D.h"

void Sprite2D::Initialize()
{
	SpriteInitialize();

	//�萔�o�b�t�@����
	DirectX12Core::GetInstance()->CreateConstBuff(constMapTransform, constBuffTransform);

	float width = static_cast<float>(WindowsApp::GetInstance()->GetWindowWidth());
	float height = static_cast<float>(WindowsApp::GetInstance()->GetWindowHeight());

	AliceMathF::MakeOrthogonalL(0.0f, width, height, 0.0f, 0.0f, 1.0f, matProjection);

	CreatVertexIndexBuffer();
}


void Sprite2D::Update(Transform& transform, const AliceMathF::Vector4& color)
{
	SpriteUpdate(transform, color);
	constMapTransform->mat = matWorld * matProjection;
}

//�`��
void Sprite2D::Draw(TextureData& textureData, Material* material)
{
	if (InitializeFlag)
	{

		//�摜�̔����̃T�C�Y
		float halfW = static_cast<float>(textureData.width) * 0.5f;
		float halfH = static_cast<float>(textureData.height) * 0.5f;

		// ���_�f�[�^
		PosUvColor vertices[] =
		{//		x		y		z		u	v
			{{-halfW, -halfH, 0.0f},{0.0f,0.0f},vertMap[0].color},//����C���f�b�N�X0
			{{-halfW, halfH, 0.0f},{0.0f,1.0f},vertMap[1].color},//�����C���f�b�N�X1
			{{halfW, -halfH, 0.0f},{1.0f,0.0f},vertMap[2].color},//�E��C���f�b�N�X2
			{{halfW, halfH, 0.0f},{1.0f,1.0f},vertMap[3].color},//�E���C���f�b�N�X3
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
