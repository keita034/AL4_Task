#pragma once
#include"ErrorException.h"
#include"Transform.h"
#include"ModelCreate.h"
#include"CollisionPrimitive.h"
#include"GameCollisionConflg.h"

class Player;

class EnemyBullet :public OBB
{

private:

	std::unique_ptr<Transform> transform_;//���[���h�ϊ��f�[�^
	std::shared_ptr<objModel> bullet_;//���f��
	std::shared_ptr<Player> player_;
	AliceMathF::Vector3 velocity_;//���x
	static const int32_t kLifeTime = 60 * 5;//����<frm>
	int32_t deathTimer_ = kLifeTime;//�f�X�^�C�}�[
	bool isDead_ = false;//�f�X�t���O
	char PADING[7]{};

public:
	EnemyBullet();//�R���X�g���N�^
	~EnemyBullet();;//�f�X�g���N�^
	void Initialize(std::shared_ptr<objModel> model, std::shared_ptr<Player> player, const AliceMathF::Vector3& position, const AliceMathF::Vector3& velocity);//����������
	void Update(Camera* camera);//�X�V����
	void Draw();//�`�揈��
	virtual void OnCollision()override;
	bool IsDead() const;//�f�X�t���O�Q�b�^�[
	AliceMathF::Vector3 GetWorldPosition();//���[���h���W������
	Transform& GetTransform();
private:
	//�R�s�[�R���X�g���N�^�E������Z�q�폜
	EnemyBullet& operator=(const EnemyBullet&) = delete;
	EnemyBullet(const EnemyBullet&) = delete;

};

