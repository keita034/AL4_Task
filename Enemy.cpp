#include "Enemy.h"
#include"Player.h"
#include"EnemyBullet.h"
#include"GameScene.h"

void Enemy::Initialize(EnemyData& data, AliceMathF::Vector3& position)
{
	gameScene_ = GameScene::GetInstance();

	enemymodel_ = data.enemymodel_;
	camera_ = data.camera_;
	hitSprite.reset(SpriteCreat::Create2DSprite());
	destroyTex = data.destroyTex;
	bullet_ = data.bullet_;
	player_ = data.player_;
	destroySE = data.destroySE;

	transform_.Initialize();
	transform_.translation = position;
	initpos = position;
	transform_.scale = { 50.0f,50.0f, 50.0f };

	CreateOBB(enemymodel_->GetVertices(), transform_);

	//衝突属性を設定
	SetCollisionAttribute(CollisionAttributeEnemy);
	//衝突対象を自分以外に設定
	SetCollisionMask(~CollisionAttributeEnemy);
	collsionName = "Enemy";
}

void Enemy::Update()
{
	switch (phase)
	{
	case Phase::Approach://接近フェーズ
		ApproachMove();
		break;
	case Phase::Around://周回フェーズ
		AroundMove();
		break;
	default:
		break;
	}

	transform_.TransUpdate(camera_->GetThirdPersonCamera());

	if (attackInterval > 0)
	{
		attackInterval--;
	}

	if (AttackFlag() && attackInterval == 0)
	{
		attackInterval = 60 * 5;
		Attack();
	}

	if (!isRockOnFlag)
	{
		ReticleUpdate();
	}

	if (hitTimer > 0)
	{
		hitTrans.translation = { 640,360,0 };
		AliceMathF::Vector4 col = { 1.0f,1.0f,1.0f,1.0f };
		col.w = (1.0f / 30.0f) * static_cast<float>(hitTimer);
		hitSprite->Update(hitTrans, col);
		hitTimer--;
	}

	UpdateOBB(transform_);

	if (hp <= 0 && hitTimer == 29)
	{
		AudioManager::GetInstance()->PlayWave(destroySE);

	}
	if (hp <= 0 && hitTimer == 0)
	{
		isDead = true;
	}
}

void Enemy::Draw()
{
	//描画
	enemymodel_->Draw(&transform_);

	if (hitTimer > 0)
	{
		hitSprite->Draw(destroyTex);
	}
}

void Enemy::SetControlPoint(std::vector<AliceMathF::Vector3>& controlPoints)
{
	controlPoints_ = controlPoints;
}

AliceMathF::Vector3 Enemy::GetWorldPosition()
{
	return AliceMathF::GetWorldPosition(transform_);
}

void Enemy::SetRockOn(bool flag)
{
	isRockOnFlag = flag;
}

bool Enemy::GetRockOn()
{
	return isRockOnFlag;
}

Transform& Enemy::GetTransform()
{
	return transform_;
}

void Enemy::OnCollision()
{
	if (GetOpponentCollsionName() == "PlayerBullet")
	{
		if (hp > 0)
		{
			hitTimer = 30;
		}

		hp--;
	}
	else if (GetOpponentCollsionName() == "Missile")
	{
		if (hp > 0)
		{
			hitTimer = 30;
		}

		hp -= 3;
	}



}

bool Enemy::IsDead() const
{
	return isDead;
}

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::ApproachMove()
{
	time += 0.001f;
	time = AliceMathF::Clamp01(time);

	AliceMathF::Vector3 tmpvec = transform_.translation;

	//初期値から制御点の最初の所へ移動
	transform_.translation = AliceMathF::Lerp(initpos, controlPoints_[0], time);

	if (time >= 1.0f)
	{
		//周回フェーズに移動
		phase = Phase::Around;
		time = 0.0f;
	}

	//方向ベクトル計算
	velocity_ = transform_.translation - tmpvec;

	AliceMathF::Vector3 temp = velocity_;
	temp.y = 0.0f;

	//ヨー計算
	transform_.rotation.y = AliceMathF::Atan2(velocity_.x, velocity_.z);
	//ピッチ計算
	transform_.rotation.x = AliceMathF::Atan2(-velocity_.y, temp.length());


}

void Enemy::AroundMove()
{
	time += 0.0005f;
	time = AliceMathF::Clamp01(time);

	//キャットムルスプライン曲線
	transform_.translation = AliceMathF::CatmullRomSpline(controlPoints_, time);

	if (time >= 1.0f)
	{
		time = 0.0f;
	}

	float tmpTime = time + 0.001f;
	tmpTime = AliceMathF::Clamp01(tmpTime);

	//方向ベクトル計算
	velocity_ = AliceMathF::CatmullRomSpline(controlPoints_, tmpTime) - AliceMathF::CatmullRomSpline(controlPoints_, time);
	AliceMathF::Vector3 temp = velocity_;
	temp.y = 0.0f;

	//ヨー計算
	transform_.rotation.y = AliceMathF::Atan2(velocity_.x, velocity_.z);
	//ピッチ計算
	transform_.rotation.x = AliceMathF::Atan2(-velocity_.y, temp.length());
}

void Enemy::Attack()
{
	//弾の速度
	const float kBulletSpeed = 1.0f;

	//プレイヤーのワールド座標を取得
	AliceMathF::Vector3 playerPos = player_->GetWorldPosition();
	//敵キャラのワールド座標を取得
	AliceMathF::Vector3 enemyPos = GetWorldPosition();
	//敵キャラ->自キャラの差分ベクトルを求める
	AliceMathF::Vector3 velocity = playerPos - enemyPos;
	//ベクトルの正規化
	velocity = velocity.normalization();
	//ベクトルの長さを、早さに合わせる
	velocity *= kBulletSpeed;

	// 弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(bullet_, player_, transform_.translation, velocity);
	//弾を登録する
	gameScene_->AddEnemyBullet(newBullet);
}

bool Enemy::AttackFlag()
{
	targetVec_ = player_->GetWorldPosition() - transform_.translation;//ターゲットベクトル
	targetSize_ = targetVec_.length();//ターゲットベクトルの大きさを取得
	targetVec_ = targetVec_.normalization();//ターゲットベクトルの正規化

	float vecAngle = velocity_.dot(targetVec_);//正面ベクトルとターゲットベクトルの内積
	float deg = acosf(vecAngle) * AliceMathF::Rad2Deg;//視野定義

	if (deg <= 90.0f && targetSize_ <= 5000.0f)
	{
		return true;
	}

	return false;
}

void Enemy::ReticleUpdate()
{
	AliceMathF::Vector3 positionReticle = GetWorldPosition();

	float winH = static_cast<float>(WindowsApp::GetInstance()->GetWindowHeight() / 2);
	float winW = static_cast<float>(WindowsApp::GetInstance()->GetWindowWidth() / 2);

	//ビューポート行列
	AliceMathF::Matrix4 matViewport = { winW, 0, 0, 0, 0, -winH, 0, 0, 0, 0, 1, 0, winW, winH, 0, 1 };

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	AliceMathF::Matrix4 matViewProjectionViewport =
		camera_->GetThirdPersonCamera()->GetViewMatrixInv() * camera_->GetThirdPersonCamera()->GetProjectionMatrix() * matViewport;

	//ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = AliceMathF::Vec3Mat4MulWdiv(positionReticle, matViewProjectionViewport);

	ReticleTrans.translation = positionReticle;
}
