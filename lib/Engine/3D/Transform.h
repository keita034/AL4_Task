#pragma once
#include"ErrorException.h"
#include"AliceMathUtility.h"
#include"Camera.h"

class Transform
{
private:
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
	//�萔�o�b�t�@�̃}�b�s���O�p�|�C���^
	worldViewpojCamera* constBuffMap = nullptr;

public:
	// ���[�J�����W
	AliceMathF::Vector3 translation = { 0, 0, 0 };
	// X,Y,Z�����̃��[�J����]�p
	AliceMathF::Vector3 rotation = { 0, 0, 0 };
	// ���[�J���X�P�[��
	AliceMathF::Vector3 scale = { 1, 1, 1 };

	AliceMathF::Matrix4 matWorld;
private:
	char PADDING[4]{};
public:
	// �e�ƂȂ郏�[���h�ϊ��ւ̃|�C���^
	const Transform* parent = nullptr;

public:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	/// �s���]������
	/// </summary>
	void TransUpdate(Camera* camera);

	void MakeWorldMatrix();

	ID3D12Resource* GetconstBuff();
};

namespace AliceMathF
{
	Matrix4 MakeWorldMatrix4(Transform& transform);

	/// <summary>
	/// ���[���h���W�擾
	/// </summary>
	Vector3 GetWorldPosition(Transform& transform);
}