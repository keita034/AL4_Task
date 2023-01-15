#pragma once
#include"Model.h"
#include"AliceMathUtility.h"
#include"DefaultMaterial.h"

class objModel : public Model
{
private:

public:

	objModel();
	~objModel();

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	virtual void Create(const char* filePath, bool smoothing = false)override;

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="camera">�J����</param>
	virtual void Draw(Transform* transform, Material* material = &DEFAULT_TEXTURE_MATERIAL)override;

	//������
	virtual void Initialize()override;

private:

	void LoadMaterial(const std::string& directoryPath,const std::string& filename);
	void LoadTexture(const std::string& directoryPath, const std::string& filename);

	virtual void Create(bool smoothing)override
	{
		static_cast<void>(smoothing);
	};

	void CalculateSmoothedVertexNormals();

	//�R�s�[�R���X�g���N�^�E������Z�q�폜
	objModel& operator=(const objModel&) = delete;
	objModel(const objModel&) = delete;
};