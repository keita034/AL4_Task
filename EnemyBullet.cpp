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
	// NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	bullet_ = model;
	velocity_ = velocity;
	player_ = player;

	transform_ = std::make_unique<Transform>();
	transform_->Initialize();
	//引数で受け取った初期座標をセット
	transform_->translation = position;
	transform_->scale = { 100.0f,100.0f,100.0f };

	CreateOBB(bullet_->GetVertices(), *transform_);

	//衝突属性を設定
	SetCollisionAttribute(CollisionAttributeEnemy);
	//衝突対象を自分以外に設定
	SetCollisionMask(~CollisionAttributeEnemy);
	collsionName = "EnemyBullet";
}

void EnemyBullet::Update(Camera* camera)
{
	//敵弾から自キャラへのベクトルを計算
	AliceMathF::Vector3 toPlayer = player_->GetWorldPosition() - GetWorldPosition();

	if (deathTimer_ >= 60 * 4)
	{
		//ベクトルを正規化する
		toPlayer = toPlayer.normalization();
		velocity_ = velocity_.normalization();
		//球面線形補間により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
		velocity_ = AliceMathF::Lerp(velocity_, toPlayer, 0.3f) * 18.0f;
	}

	//座標を移動させる(1フレーム分の移動を差し込む)
	transform_->translation += velocity_;

	transform_->rotation.y = AliceMathF::Atan2(velocity_.x, velocity_.z);
	AliceMathF::Vector3 temp = velocity_;
	temp.y = 0.0f;
	transform_->rotation.x = AliceMathF::Atan2(-velocity_.y, temp.length());

	//ワールドトランスフォームの更新
	transform_->TransUpdate(camera);

	UpdateOBB(*transform_);

	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}

void EnemyBullet::Draw()
{
	//モデルの描画
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
