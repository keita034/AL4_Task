#pragma once
#include"ErrorException.h"
#include"AliceMathUtility.h"
#include"ModelCreate.h"
#include"GameCamera.h"
#include"Transform.h"
#include"CollisionPrimitive.h"
#include"GameCollisionConflg.h"

class Missile :public OBB
{
private:

	//���f��
	std::shared_ptr<objModel> missileModel_;
	//�J����
	std::shared_ptr<GameCamera>camera_;
	//���W�ϊ�
	Transform missileTransform_{};
	//���x
	AliceMathF::Vector3 velocity_;
	char PADING[4]{};
	//�G�̍��W
	Transform* enemyTransform_ = nullptr;
	//������ł̎���
	float period_ = 0.0f;
	bool isDead_ = false;//�f�X�t���O
	bool lockonFlag_ = false;
	char PADING2[2]{};
public:
	Missile();
	~Missile();
	void Initialize(std::shared_ptr<GameCamera> camera, std::shared_ptr<objModel> missileModel, Transform* enemyTransform, AliceMathF::Vector3& velocity, AliceMathF::Vector3& pos, bool lockonFlag);
	void Update();
	void Draw();
	bool IsDead() const;//�f�X�t���O�Q�b�^�[
	virtual void OnCollision()override;
private:

};

