#pragma once
#include"ErrorException.h"
#include"ModelCreate.h"
#include"Transform.h"
#include"GameCamera.h"
#include"CollisionPrimitive.h"
#include"Sprite2D.h"
#include"GameCollisionConflg.h"

//�s���t�F�[�Y
enum class Phase
{
	Approach, //�ڋ߂���
	Around, //���񂷂�

};

class GameScene;
class Player;

struct EnemyData
{
	//���f��
	std::shared_ptr<objModel> enemymodel_;
	//�J����
	std::shared_ptr<GameCamera> camera_;
	//�f�X�g���C�摜�e�N�X�`��
	TextureData destroyTex;
	//�e
	std::shared_ptr<objModel> bullet_;
	//�v���C���[
	std::shared_ptr<Player>player_;
	//���S��
	uint32_t destroySE = 0;

	char PADING[4]{};
};


class Enemy :public OBB
{

private:

	GameScene* gameScene_ = nullptr;

	//���f��
	std::shared_ptr<objModel> enemymodel_;
	//�J����
	std::shared_ptr<GameCamera> camera_;
	//�q�b�g�摜
	std::shared_ptr<Sprite2D> hitSprite;
	//�f�X�g���C�摜�e�N�X�`��
	TextureData destroyTex;
	//�e
	std::shared_ptr<objModel> bullet_;
	//�v���C���[
	std::shared_ptr<Player>player_;
	//���S��
	uint32_t destroySE = 0;
	char PADING[4]{};

	//���W�ϊ�
	Transform transform_;
	Transform hitTrans;
	Transform ReticleTrans;

	//����_
	std::vector<AliceMathF::Vector3> controlPoints_;
	AliceMathF::Vector3 initpos;
	char PADING4[4]{};

	float time = 0.0f;

	Phase phase = Phase::Approach;

	int32_t hitTimer= 0;
	AliceMathF::Vector3 targetVec_;
	float targetSize_ = 0.0f;
	AliceMathF::Vector3 velocity_;
	int32_t attackInterval = 60 * 5;
	char PADING2[4]{};
	bool isRockOnFlag = false;
	char PADING3[3]{};
	int32_t hp = 6;
	bool isDead = false;
	char PADING5[7]{};
public:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(EnemyData& data, AliceMathF::Vector3& position);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	void SetControlPoint(std::vector<AliceMathF::Vector3>& controlPoints);

	AliceMathF::Vector3 GetWorldPosition();

	void SetRockOn(bool flag);

	bool GetRockOn();

	Transform& GetTransform();

	virtual void OnCollision();

	bool IsDead() const;//�f�X�t���O�Q�b�^�[

	void ReticleUpdate();

	Enemy();
	~Enemy();
private:

	//�ڋ߃t�F�[�Y
	void ApproachMove();

	//����t�F�[�Y
	void AroundMove();

	void Attack();

	bool AttackFlag();

	//�R�s�[�R���X�g���N�^�E������Z�q�폜
	Enemy& operator=(const Enemy&) = delete;
	Enemy(const Enemy&) = delete;
};

