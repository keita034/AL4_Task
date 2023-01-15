#include "EnemyBullet.h"
#include"Player.h"

EnemyBullet::EnemyBullet()
{
}

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::Initialize(std::shared_ptr<objModel> model, std::shared_ptr<Player> player, const AliceMathF::Vector3& position, const AliceMathF::Vector3& velocity)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	bullet_ = model;
	velocity_ = velocity;
	player_ = player;

	transform_ = std::make_unique<Transform>();
	transform_->Initialize();
	//�����Ŏ󂯎�����������W���Z�b�g
	transform_->translation = position;
	transform_->scale = { 100.0f,100.0f,100.0f };

	CreateOBB(bullet_->GetVertices(), *transform_);

	//�Փˑ�����ݒ�
	SetCollisionAttribute(CollisionAttributeEnemy);
	//�ՓˑΏۂ������ȊO�ɐݒ�
	SetCollisionMask(~CollisionAttributeEnemy);
	collsionName = "EnemyBullet";
}

void EnemyBullet::Update(Camera* camera)
{
	//�G�e���玩�L�����ւ̃x�N�g�����v�Z
	AliceMathF::Vector3 toPlayer = player_->GetWorldPosition() - GetWorldPosition();

	if (deathTimer_ >= 60 * 4)
	{
		//�x�N�g���𐳋K������
		toPlayer = toPlayer.normalization();
		velocity_ = velocity_.normalization();
		//���ʐ��`��Ԃɂ��A���̑��x�Ǝ��L�����ւ̃x�N�g������}���A�V���ȑ��x�Ƃ���
		velocity_ = AliceMathF::Lerp(velocity_, toPlayer, 0.3f) * 18.0f;
	}

	//���W���ړ�������(1�t���[�����̈ړ�����������)
	transform_->translation += velocity_;

	transform_->rotation.y = AliceMathF::Atan2(velocity_.x, velocity_.z);
	AliceMathF::Vector3 temp = velocity_;
	temp.y = 0.0f;
	transform_->rotation.x = AliceMathF::Atan2(-velocity_.y, temp.length());

	//���[���h�g�����X�t�H�[���̍X�V
	transform_->TransUpdate(camera);

	UpdateOBB(*transform_);

	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}

void EnemyBullet::Draw()
{
	//���f���̕`��
	bullet_->Draw(transform_.get());
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}

bool EnemyBullet::IsDead() const
{
	return isDead_;
}

AliceMathF::Vector3 EnemyBullet::GetWorldPosition()
{
	return AliceMathF::GetWorldPosition(*transform_);
}

Transform& EnemyBullet::GetTransform()
{
	return *transform_;
}
