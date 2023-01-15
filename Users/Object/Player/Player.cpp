#include "Player.h"
#include"DefaultMaterial.h"
#include"Enemy.h"

Player::Player()
{

}

Player::~Player()
{

}

void Player::Initialize(AliceMathF::Vector3* pos, PlayerData& data)
{
	//�V���O���g�����
	input_ = Input::GetInstance();
	player_ = data.player;
	missileModel_ = data.missileModel;
	camera_ = data.camera;
	transform_ = std::make_unique<Transform>();
	transform_->Initialize();
	transform_->parent = data.camera->GetFirstPersonTransform();
	transform_->translation = *pos;
	transform_->scale = { 10.0f,10.0f,10.0f };

	reticle.reset(SpriteCreat::Create2DSprite());
	reticleTex = data.reticleTex;
	reticleTrans.Initialize();
	reticleTrans.translation.x = 640;
	reticleTrans.translation.y = 360;

	RockOnReticle.reset(SpriteCreat::Create2DSprite());
	RockOnReticleTex = data.RockOnReticleTex;
	rockOnReticleTrans.Initialize();

	sceneChange.reset(SpriteCreat::Create2DSprite());
	sceneChangeTex_ = data.sceneChangeTex;
	sceneChangeTransform.translation.x = 640.0f;
	sceneChangeTransform.translation.y = 360.0f;
	gameOverScene = data.gameOverScene;

	silenPullUp.reset(SpriteCreat::Create2DSprite());
	silenPullUpTex_ = data.silenPullUpTex;
	silenPullUpTransform.translation.x = 640.0f;
	silenPullUpTransform.translation.y = 560.0f;

	silenReturn.reset(SpriteCreat::Create2DSprite());
	silenReturnTex_ = data.silenReturnTex;
	silenReturnTransform.translation.x = 640.0f;
	silenReturnTransform.translation.y = 560.0f;

	damageEffect.reset(SpriteCreat::Create2DSprite());
	damageEffectTex_[0] = data.damageEffectTex[0];
	damageEffectTex_[1] = data.damageEffectTex[1];
	damageEffectTex_[2] = data.damageEffectTex[2];
	damageEffectTransform.translation.x = 640.0f;
	damageEffectTransform.translation.y = 360.0f;

	CreateOBB(player_->GetVertices(), *transform_);

	//�Փˑ�����ݒ�
	SetCollisionAttribute(CollisionAttributePlayer);
	//�ՓˑΏۂ������ȊO�ɐݒ�
	SetCollisionMask(~CollisionAttributePlayer);
	collsionName = "Player";

	missileSE = data.missileSE;
	fighterSE = data.fighterSE;
	destroySE = data.destroySE;

	tmpTrasn.Initialize();
}

void Player::Complement(float& x1, float x2, float flame)//�ړ��⊮
{
	float distanceX = x2 - x1;//�������o��
	float dividedDistanceX = distanceX / flame;//������flame�Ŋ������l

	x1 += dividedDistanceX;//������flame�Ŋ������l�𑫂�
}

void Player::Rotate()
{
	//���񏈗�
	Complement(transform_->rotation.z, -input_->GetLeftStickVec({ 1.0f,1.0f }).x * 40.0f * AliceMathF::Deg2Rad, 40.0f);//��
	Complement(transform_->translation.x, static_cast<float>(input_->GetKeyTime(DIK_A)) / 254.0f * 30.0f, 40.0f);
	Complement(transform_->translation.x, static_cast<float>(-input_->GetKeyTime(DIK_D)) / 254.0f * 30.0f, 40.0f);
}

void Player::MissileAttack()
{
	if (missileInterval <= 0)
	{
		if (input_->ButtonInput(A)||input_->KeepPush(DIK_SPACE))
		{
			// �e�𐶐����A������
			std::unique_ptr<Missile> newMissile = std::make_unique<Missile>();

			//���L�����̍��W���R�s�[
			AliceMathF::Vector3 position = GetWorldPosition();
			//�e�̑��x
			const float kBulletSpeed = camera_->GetSpeed() + 1.0f;
			AliceMathF::Vector3 velocity(0, 0, kBulletSpeed);

			//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
			velocity = AliceMathF::Vec3Mat4Mul(velocity, transform_->matWorld);

			newMissile->Initialize(camera_, missileModel_, &enemy_->GetTransform(), velocity, position, RockOnFlag);

			//�e��o�^����
			missiles_.push_back(std::move(newMissile));

			AudioManager::GetInstance()->PlayWave(missileSE);
			missileInterval = 60 * 5;
		}
	}
	else
	{
		missileInterval--;
	}
}

void Player::Update(INT32& sceneNum)
{
	if (initflag)
	{
		AudioManager::GetInstance()->PlayWave(fighterSE, true);
		initflag = false;
	}

	oldTranslation_ = GetWorldPosition();

	//�f�X�t���O�̗������e���폜
	missiles_.remove_if([](std::unique_ptr<Missile>& missile)
		{
			return missile->IsDead();
		});

	if (animationFlag == false)
	{
		Rotate();
		transform_->TransUpdate(camera_->GetThirdPersonCamera());

		MissileAttack();

		//�e�X�V
		for (std::unique_ptr<Missile>& missile : missiles_)
		{
			missile->Update();
		}
	}

	reticle->Update(reticleTrans);
	RockOnReticleUpdate();
	UpdateOBB(*transform_);

	//�̗͂�0�ȉ��ɂȂ�����
	if (hp <= 0)
	{
		//�Q�[���I�[�o�[�V�[��
		if (animationFlag == false)
		{
			AudioManager::GetInstance()->PlayWave(destroySE);
			AudioManager::GetInstance()->StopWave(fighterSE);
		}
		animationFlag = true;
		if (animationFlag == true)
		{
			alpha += 1.0f / 60;
		}

		if (alpha >= 1.0f)
		{
			animationFlag = false;
			AudioManager::GetInstance()->PlayWave(gameOverScene, true);
			uint32_t gameScene = AudioManager::GetInstance()->LoadAudio("Resources/sound/game.mp3");
			AudioManager::GetInstance()->StopWave(gameScene);
			sceneNum = 2;
		}
	}

	silenPullUp->Update(silenPullUpTransform);
	silenReturn->Update(silenReturnTransform);

	WorldPos = camera_->GetWorldPosition();
	velocity2 = { 0, 0, 5000.0f };
	tmpVec = velocity2 = AliceMathF::Vec3Mat4Mul(velocity2, camera_->GetFirstPersonTransform()->matWorld);

	tmpTrasn.translation = WorldPos;
	tmpTrasn.TransUpdate(camera_->GetThirdPersonCamera());

	//�I������
	if (GetWorldPosition().x <= -11600 || GetWorldPosition().x >= 11600 || GetWorldPosition().z <= -11600 || GetWorldPosition().z >= 11600)
	{
		animationFlag = true;
		if (animationFlag == true)
		{
			alpha += 1.0f / 100;
		}

		if (alpha >= 1.0f)
		{
			animationFlag = false;
			AudioManager::GetInstance()->StopWave(fighterSE);
			AudioManager::GetInstance()->PlayWave(gameOverScene, true);
			uint32_t gameScene = AudioManager::GetInstance()->LoadAudio("Resources/sound/game.mp3");
			AudioManager::GetInstance()->StopWave(gameScene);
			sceneNum = 2;
		}
	}

	if (GetWorldPosition().y >= 11600 || GetWorldPosition().y <= 0)
	{
		animationFlag = true;
		if (animationFlag == true)
		{
			alpha += 1.0f / 60;
		}

		if (alpha >= 1.0f)
		{
			animationFlag = false;
			AudioManager::GetInstance()->StopWave(fighterSE);
			AudioManager::GetInstance()->PlayWave(gameOverScene, true);
			uint32_t gameScene = AudioManager::GetInstance()->LoadAudio("Resources/sound/game.mp3");
			AudioManager::GetInstance()->StopWave(gameScene);
			sceneNum = 2;
		}
	}

	if (damageFlag == true)
	{
		damageAlpha -= 0.5f / 60;
		if (damageAlpha <= 0)
		{
			damageFlag = false;
		}
	}
	else
	{
		damageAlpha = 0.5f;
	}

	sceneChange->Update(sceneChangeTransform, { 1.0,1.0,1.0,alpha });
	damageEffect->Update(damageEffectTransform, { 1.0,1.0,1.0,damageAlpha });
}

void Player::Draw()
{
	if (hp > 0)
	{
		player_->Draw(transform_.get());
	}

	if (animationFlag == true)
	{
		sceneChange->Draw(sceneChangeTex_);
	}

	for (std::unique_ptr<Missile>& missile : missiles_)
	{
		missile->Draw();
	}

	reticle->Draw(reticleTex);

	if (hp > 0)
	{
		if (RockOnFlag)
		{
			RockOnReticle->Draw(RockOnReticleTex);
		}
	}

	WorldPos = WorldPos + tmpVec;


	if (WorldPos.y <= 1500 && GetWorldPosition().y <= 1500 || WorldPos.y >= 8500 && GetWorldPosition().y >= 8500)
	{
		warningInterval += 0.05f;

		AliceMathF::Vector4 col = { 1.0f,1.0f,1.0f,AliceMathF::Sin(warningInterval) };

		if (WorldPos.y <= 1500 && GetWorldPosition().y <= 1500)
		{
			silenPullUp->Update(silenPullUpTransform,col);
			silenPullUp->Draw(silenPullUpTex_);
		}

		if (WorldPos.y >= 8500 && GetWorldPosition().y >= 8500)
		{
			silenReturn->Update(silenReturnTransform, col);
			silenReturn->Draw(silenReturnTex_);
		}
	}
	else
	{
		warningInterval = 0.0f;
	}
	
	if (hp < 4 && hp >= 1)
	{
		damageEffect->Draw(damageEffectTex_[hp-1]);
	}

}

AliceMathF::Vector3 Player::GetWorldPosition()
{
	//���[���h���W������ϐ�
	AliceMathF::Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = transform_->matWorld.m[3][0];
	worldPos.y = transform_->matWorld.m[3][1];
	worldPos.z = transform_->matWorld.m[3][2];

	return worldPos;
}

void Player::StopAudio()
{
	AudioManager::GetInstance()->StopWave(bulletSE);
	AudioManager::GetInstance()->StopWave(destroySE);
	AudioManager::GetInstance()->StopWave(fighterSE);
	AudioManager::GetInstance()->StopWave(missileSE);
	//AudioManager::GetInstance()->StopWave(gameScene);
}

int32_t Player::GetHp()
{
	return hp;
}

void Player::LockOn(std::list<std::shared_ptr<Enemy>>& enemys)
{
	tmpEnemys.clear();

	for (std::shared_ptr<Enemy>& enemy : enemys)
	{
		enemy->SetRockOn(false);
		enemy->ReticleUpdate();

		if (LockOnFlag(enemy.get()))
		{
			tmpEnemys.push_back(enemy);
		}
	}

	if (tmpEnemys.empty())
	{
		RockOnFlag = false;

	}
	else
	{
		float len = 9999999.0f;
		for (std::shared_ptr<Enemy>& enemy : tmpEnemys)
		{
			AliceMathF::Vector3 tmp = enemy->GetWorldPosition() - GetWorldPosition();
			float len2 = tmp.length();

			if (len2 < len)
			{
				len = len2;
				enemy_ = enemy.get();
				enemy->SetRockOn(true);
				RockOnFlag = true;

			}
		}
	}

}

bool Player::LockOnFlag(Enemy* enemy)
{
	velocity_ = GetWorldPosition() - oldTranslation_;

	targetVec_ = enemy->GetWorldPosition() - GetWorldPosition();//�^�[�Q�b�g�x�N�g��
	targetSize_ = targetVec_.length();//�^�[�Q�b�g�x�N�g���̑傫�����擾
	targetVec_ = targetVec_.normalization();//�^�[�Q�b�g�x�N�g���̐��K��
	velocity_ = velocity_.normalization();
	float vecAngle = velocity_.dot(targetVec_);//���ʃx�N�g���ƃ^�[�Q�b�g�x�N�g���̓���
	float tmp = AliceMathF::Acos(vecAngle);
	float deg = AliceMathF::Rad2Deg * tmp;//�����`

	if (deg <= 20.0f && targetSize_ <= 5000.0f)
	{
		return true;
	}

	return false;
}

bool Player::ReticleFlag()
{
	velocity_ = oldTranslation_ - GetWorldPosition();

	targetVec_ = enemy_->GetWorldPosition() - GetWorldPosition();//�^�[�Q�b�g�x�N�g��
	targetVec_ = targetVec_.normalization();//�^�[�Q�b�g�x�N�g���̐��K��
	velocity_ = velocity_.normalization();
	float vecAngle = velocity_.dot(targetVec_);//���ʃx�N�g���ƃ^�[�Q�b�g�x�N�g���̓���
	float tmp = AliceMathF::Acos(vecAngle);
	float deg = AliceMathF::Rad2Deg * tmp;//�����`

	if (deg >= 70.0f)
	{
		return true;
	}

	return false;
}

Transform* Player::GetTransform()
{
	return transform_.get();
}

bool Player::ReticleFlag(Enemy* enemy)
{
	velocity_ = camera_->GetFront();

	targetVec_ = enemy->GetWorldPosition() - GetWorldPosition();//�^�[�Q�b�g�x�N�g��
	targetVec_ = targetVec_.normalization();//�^�[�Q�b�g�x�N�g���̐��K��
	velocity_ = velocity_.normalization();
	float vecAngle = velocity_.dot(targetVec_);//���ʃx�N�g���ƃ^�[�Q�b�g�x�N�g���̓���
	float tmp = AliceMathF::Acos(vecAngle);
	float deg = AliceMathF::Rad2Deg * tmp;//�����`

	if (deg <= 120.0f)
	{
		return true;
	}

	return false;
}

std::list<std::unique_ptr<Missile>>& Player::GetMissileList()
{
	return missiles_;
}

void Player::RockOnReticleUpdate()
{
	if (enemy_ == nullptr)
	{
		RockOnFlag = false;
	}

	if (RockOnFlag)
	{
		AliceMathF::Vector3 positionReticle = AliceMathF::GetWorldPosition(enemy_->GetTransform());

		float winH = static_cast<float>(WindowsApp::GetInstance()->GetWindowHeight() / 2);
		float winW = static_cast<float>(WindowsApp::GetInstance()->GetWindowWidth() / 2);

		//�r���[�|�[�g�s��
		AliceMathF::Matrix4 matViewport = { winW, 0, 0, 0, 0, -winH, 0, 0, 0, 0, 1, 0, winW, winH, 0, 1 };

		//�r���[�s��ƃv���W�F�N�V�����s��A�r���[�|�[�g�s�����������
		AliceMathF::Matrix4 matViewProjectionViewport =
			camera_->GetThirdPersonCamera()->GetViewMatrixInv() * camera_->GetThirdPersonCamera()->GetProjectionMatrix() * matViewport;

		//���[���h���X�N���[�����W�ϊ�(������3D����2D�ɂȂ�)
		positionReticle = AliceMathF::Vec3Mat4MulWdiv(positionReticle, matViewProjectionViewport);

		rockOnReticleTrans.translation = positionReticle;

		RockOnReticle->Update(rockOnReticleTrans);
	}
}

void Player::OnCollision()
{
	if (GetOpponentCollsionName() == "EnemyBullet")
	{
		hp--;
		damageFlag = true;

		input_->ShakeController(1.0f, 50.0f);
	}
	else if (GetOpponentCollsionName() == "Enemy")
	{
		hp = 0;
		damageFlag = true;

		input_->ShakeController(1.0f, 50.0f);
	}
}
