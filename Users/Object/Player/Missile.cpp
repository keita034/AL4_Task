#include "Missile.h"
#include"DefaultMaterial.h"

Missile::Missile()
{
}

Missile::~Missile()
{
}

void Missile::Initialize(std::shared_ptr<GameCamera> camera, std::shared_ptr<objModel> missileModel, Transform* enemyTransform, AliceMathF::Vector3& velocity, AliceMathF::Vector3& pos,bool lockonFlag)
{
	camera_ = camera;
	missileModel_ = missileModel;
	enemyTransform_ = enemyTransform;

	velocity_ = velocity;
	period_ = 60.0f * 2.0f;
	missileTransform_.Initialize();
	missileTransform_.translation = pos;
	missileTransform_.scale = {50.5f,50.5f, 50.5f};
	lockonFlag_ = lockonFlag;
	if (lockonFlag_)
	{
		velocity_ = velocity/5.0f;
	}

	CreateOBB(missileModel_->GetVertices(), missileTransform_);

	//Õ“Ë‘®«‚ğİ’è
	SetCollisionAttribute(CollisionAttributePlayer);
	//Õ“Ë‘ÎÛ‚ğ©•ªˆÈŠO‚Éİ’è
	SetCollisionMask(~CollisionAttributePlayer);

	collsionName = "Missile";
}

void Missile::Update()
{
	if (lockonFlag_)
	{
		AliceMathF::Vector3 diff = AliceMathF::GetWorldPosition(*enemyTransform_) - missileTransform_.translation;
		//‰Á‘¬“x
		AliceMathF::Vector3 acceleration;

		acceleration += (diff - velocity_ * period_) * 2.0f / (period_ * period_);

		period_--;

		if (period_ < 0.0f)
		{
			isDead_ = true;
			return;
		}

		velocity_ += acceleration;
		missileTransform_.translation += velocity_;
	}
	else
	{
		period_--;

		if (period_ < 0.0f)
		{
			isDead_ = true;
			return;
		}

		missileTransform_.translation += velocity_;
	}
	
	missileTransform_.rotation.y = AliceMathF::Atan2(velocity_.x, velocity_.z);
	AliceMathF::Vector3 temp = velocity_;
	temp.y = 0.0f;
	missileTransform_.rotation.x = AliceMathF::Atan2(-velocity_.y, temp.length());

	missileTransform_.TransUpdate(camera_->GetThirdPersonCamera());

	UpdateOBB(missileTransform_);
}

void Missile::Draw()
{
	missileModel_->Draw(&missileTransform_);
}

bool Missile::IsDead() const
{
	return isDead_;
}

void Missile::OnCollision()
{
	isDead_ = true;
}
