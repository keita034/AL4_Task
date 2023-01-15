#pragma once
#include"ErrorException.h"
#include"AliceMathUtility.h"
#include"Transform.h"
#include"Camera.h"
#include"Material.h"

enum BillboardFlag
{
	NonBillboard,//�r���{�[�h�����Ȃ�
	AllBillboard,//���ׂđł�����
	XBillboard,//X����ł�����
	YBillboard,//Y����ł�����
	XYBillboard//Z����ł�����
};


class Sprite
{
protected:
	HRESULT result;
	char PADDING[4];
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr <ID3D12GraphicsCommandList> cmdList = nullptr;

	//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	//���_�}�b�v
	PosUvColor* vertMap;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�C���f�b�N�X�o�b�t�@�̐���
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
	////�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap;
	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//�萔�o�b�t�@��GPU���\�[�X�̃|�C���^
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	//�萔�o�b�t�@�̃}�b�s���O�p�|�C���^
	ConstBufferDataTransform* constMapTransform = nullptr;
	//���[���h���W
	AliceMathF::Matrix4 matWorld;
	//�v���W�F�N�V�����s��
	AliceMathF::Matrix4 matProjection;

	bool InitializeFlag = true;
	char PADDING2[7]{};
public:

	Sprite()= default;
	virtual ~Sprite() = default;

	//������
	virtual void Initialize() = 0;

	///<summary>
	///�`��
	///</summary>
	virtual void Draw(TextureData& textureData, Material* material)=0;

protected:

	//������
	virtual void SpriteInitialize();

	//�X�V
	virtual void SpriteUpdate(Transform& transform, const AliceMathF::Vector4& color = { 1.0f,1.0f, 1.0f, 1.0f });

	//�`��
	virtual void SpriteDraw(TextureData& textureData, Material* material);

	//���_�o�b�t�@�E�C���f�b�N�X����
	virtual void CreatVertexIndexBuffer();

	//�R�s�[�R���X�g���N�^�E������Z�q�폜
	Sprite& operator=(const Sprite&) = delete;
	Sprite(const Sprite&) = delete;
};