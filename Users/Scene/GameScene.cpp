#include "GameScene.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

void GameScene::Initialize()
{

	input = Input::GetInstance();

	audioManager = audioManager->GetInstance();

	skydome.reset(ModelCreate::CreateOBJModel("Resources/model/skydome"));
	ground.reset(ModelCreate::CreateOBJModel("Resources/model/ground"));

	//BGM
	titleScene = audioManager->LoadAudio("Resources/sound/title.mp3");
	AudioManager::GetInstance()->PlayWave(titleScene, true);
	AudioManager::GetInstance()->ChangeVolume(titleScene, 0.5f);

	gameScene = audioManager->LoadAudio("Resources/sound/game.mp3");
	gameClearScene = audioManager->LoadAudio("Resources/sound/gameClear.mp3");

	groundTransform.Initialize();
	groundTransform.scale = { 245.0f,1.0f, 245.0f };
	groundTransform.translation = { 0.0f,-1000.0f, 0.0f };

	skydomeTransform.Initialize();
	skydomeTransform.scale = { 250.0f,250.0f, 250.0f };
	//�X�v���C�g�ǂݍ���
	titleBack.reset(SpriteCreat::Create2DSprite());
	titleTex = TextureManager::GetInstance()->LoadTexture(L"Resources/img/Title.png");
	titleTrans.translation.x = 640.0f;
	titleTrans.translation.y = 360.0f;

	gameClear.reset(SpriteCreat::Create2DSprite());
	gameClearTex = TextureManager::GetInstance()->LoadTexture(L"Resources/img/GameClesr.png");
	gameClearTrans.translation.x = 640.0f;
	gameClearTrans.translation.y = 360.0f;

	gameOver.reset(SpriteCreat::Create2DSprite());
	gameOverTex = TextureManager::GetInstance()->LoadTexture(L"Resources/img/GameOver.png");
	gameOverTrans.translation.x = 640.0f;
	gameOverTrans.translation.y = 360.0f;

	//���C�g������
	light.reset(Light::Create());
	light->SetLightColor(lightColor);
	//���f���Ƀ��C�g���Z�b�g
	Model::SetLight(light.get());

	gameCamera = std::make_unique<GameCamera>();
	gameCamera->Initialize();

	playerData.player.reset(ModelCreate::CreateOBJModel("Resources/model/F-15"));
	playerData.missileModel.reset(ModelCreate::CreateOBJModel("Resources/model/Missile"));
	playerData.camera = gameCamera;
	playerData.reticleTex = TextureManager::GetInstance()->LoadTexture(L"Resources/img/reticle.png");
	playerData.RockOnReticleTex = TextureManager::GetInstance()->LoadTexture(L"Resources/img/RokcReticle.png");
	playerData.sceneChangeTex = TextureManager::GetInstance()->LoadTexture(L"Resources/img/black.png");
	playerData.gameOverScene = AudioManager::GetInstance()->LoadAudio("Resources/sound/gameOver.mp3");
	playerData.silenPullUpTex = TextureManager::GetInstance()->LoadTexture(L"Resources/img/UP.png");
	playerData.silenReturnTex = TextureManager::GetInstance()->LoadTexture(L"Resources/img/Down.png");
	playerData.damageEffectTex[0] = TextureManager::GetInstance()->LoadTexture(L"Resources/img/crack3.png");
	playerData.damageEffectTex[1] = TextureManager::GetInstance()->LoadTexture(L"Resources/img/crack2.png");
	playerData.damageEffectTex[2] = TextureManager::GetInstance()->LoadTexture(L"Resources/img/crack1.png");
	playerData.missileSE = AudioManager::GetInstance()->LoadAudio("Resources/sound/missile.mp3");
	playerData.fighterSE = AudioManager::GetInstance()->LoadAudio("Resources/sound/fighter.mp3", 0.05f);
	playerData.destroySE = AudioManager::GetInstance()->LoadAudio("Resources/sound/destroy.mp3");

	player_ = std::make_unique<Player>();
	AliceMathF::Vector3 pos = { 0.0f, -0.0f, -25.0f };
	player_->Initialize(&pos, playerData);

	//���[�_�[
	radarData.radarTex = TextureManager::GetInstance()->LoadTexture(L"Resources/img/radar.png");
	radarData.radarPlayerTex = TextureManager::GetInstance()->LoadTexture(L"Resources/img/radarPlayer.png");
	radarData.radarEnemyTex = TextureManager::GetInstance()->LoadTexture(L"Resources/img/radarEnemy.png");

	//���[�_�[������
	radar_ = Radar::GetInstance();
	radar_->Initialize(radarData);

	//�R���C�_�[�}�l�W���[�̐���
	collisionManager_ = std::make_unique<GameCollisionManager>();

	//�G�̃f�[�^
	enemyData.enemymodel_.reset(ModelCreate::CreateOBJModel("Resources/model/enemyF-15"));
	enemyData.camera_ = gameCamera;
	enemyData.destroyTex = TextureManager::GetInstance()->LoadTexture(L"Resources/img/destoryReticle.png");
	enemyData.bullet_ = playerData.missileModel;
	enemyData.player_ = player_;
	enemyData.destroySE = AudioManager::GetInstance()->LoadAudio("Resources/sound/destroy.mp3");

	//���L�����̍��W���R�s�[
	AliceMathF::Vector3 position = { 9000.0f,4000.0f,0.0f };

	// �e�𐶐����A������
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(enemyData, position);
	newEnemy->SetControlPoint(enemy0);
	//�e��o�^����
	enemys_.push_back(std::move(newEnemy));

}

void GameScene::Update()
{
	//���C�g�̌��������ݒ�
	AliceMathF::Vector3 rightDir = { 10.0f,3.0f,5.0f };
	light->SetLightDir(rightDir);
	light->Update();

	switch (sceneNum)
	{
	case 0:
		//�^�C�g���V�[��
		titleBack->Update(titleTrans);
		if (input->ButtonTrigger(A) || input->KeepPush(DIK_SPACE))
		{
			AudioManager::GetInstance()->StopWave(titleScene);
			AudioManager::GetInstance()->PlayWave(gameScene, true);
			sceneNum = 1;//A�{�^���������ƃ`���[�g���A���V�[����
		}

		break;

	case 1:
		//�Q�[���V�[��
			//�f�X�t���O�̗������e���폜
		enemys_.remove_if([](std::shared_ptr<Enemy>& enemy)
			{
				return enemy->IsDead();
			});

		//�f�X�t���O�̗������e���폜
		enemyBullets_.remove_if([](std::shared_ptr<EnemyBullet>& enemyBullet)
			{
				return enemyBullet->IsDead();
			});

		if (enemys_.empty())
		{
			if (destroyFlag == 0)
			{
			//���L�����̍��W���R�s�[
				AliceMathF::Vector3 position = { -12000.0f,4000.0f,12000.0f };//���ォ��

				// �e�𐶐����A������
				std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
				newEnemy->Initialize(enemyData, position);
				newEnemy->SetControlPoint(enemy1);
				//�e��o�^����
				enemys_.push_back(std::move(newEnemy));

				//���L�����̍��W���R�s�[
				position = { 12000.0f,2000.0f,12000.0f };//�E�ォ��

				// �e�𐶐����A������
				newEnemy = std::make_unique<Enemy>();
				newEnemy->Initialize(enemyData, position);
				newEnemy->SetControlPoint(enemy2);
				//�e��o�^����
				enemys_.push_back(std::move(newEnemy));

				//���L�����̍��W���R�s�[
				position = { 12000.0f,4000.0f,-12000.0f };//�E������

				// �e�𐶐����A������
				newEnemy = std::make_unique<Enemy>();
				newEnemy->Initialize(enemyData, position);
				newEnemy->SetControlPoint(enemy3);
				//�e��o�^����
				enemys_.push_back(std::move(newEnemy));

				destroyFlag++;
			}
			else if (destroyFlag == 1)
			{
				Reset();
				AudioManager::GetInstance()->StopWave(gameScene);
				AudioManager::GetInstance()->PlayWave(gameClearScene, true);
				sceneNum = 3;//�Q�[���N���A�V�[����
			}

		}

		gameCamera->Update();

		for (std::shared_ptr<Enemy>& enemy : enemys_)
		{
			enemy->Update();
		}

		player_->LockOn(enemys_);

		for (std::shared_ptr<EnemyBullet>& enemyBullet : enemyBullets_)
		{
			enemyBullet->Update(gameCamera->GetThirdPersonCamera());
		}

		player_->Update(sceneNum);

		Collision();

		skydomeTransform.TransUpdate(gameCamera->GetThirdPersonCamera());
		groundTransform.TransUpdate(gameCamera->GetThirdPersonCamera());
		radar_->Update(player_->GetWorldPosition().x, player_->GetWorldPosition().z, gameCamera->GetFirstPersonTransform()->rotation.y, enemys_, enemyBullets_);

		break;

	case 2:
		//�Q�[���I�[�o�[�V�[��
		gameOver->Update(gameOverTrans);
		if (input->ButtonTrigger(A)||input->KeepPush(DIK_SPACE))
		{
			Reset();
			uint32_t gameOverScene = AudioManager::GetInstance()->LoadAudio("Resources/sound/gameOver.mp3");
			AudioManager::GetInstance()->StopWave(gameOverScene);
			AudioManager::GetInstance()->PlayWave(titleScene, true);
			sceneNum = 0;//A�{�^���������ƃ^�C�g���V�[����
		}
		break;

	case 3:
		//�Q�[���N���A�V�[��
		gameClear->Update(gameClearTrans);
		if (input->ButtonTrigger(A)||input->KeepPush(DIK_SPACE))
		{
			Reset();
			AudioManager::GetInstance()->StopWave(gameClearScene);
			AudioManager::GetInstance()->PlayWave(titleScene, true);
			sceneNum = 0;//A�{�^���������ƃ^�C�g���V�[����
		}
		break;
	}
}


void GameScene::Draw()
{
	switch (sceneNum)
	{
	case 0:
		//�^�C�g���V�[��
		titleBack->Draw(titleTex);//�^�C�g���w�i�`��

		break;

	case 1:
		//�Q�[���V�[��
		//�V��
		skydome->Draw(&skydomeTransform);

		//�n��
		ground->Draw(&groundTransform);

		//�G
		for (std::shared_ptr<EnemyBullet>& enemyBullet : enemyBullets_)
		{
			enemyBullet->Draw();
		}

		for (std::shared_ptr<Enemy>& enemy : enemys_)
		{
			enemy->Draw();
		}

		//�v���C���[
		player_->Draw();

		//���[�_�[
		radar_->Draw();

		break;

	case 2:
		//�Q�[���I�[�o�[�V�[��
		gameOver->Draw(gameOverTex);
		break;

	case 3:
		//�Q�[���N���A�V�[��
		gameClear->Draw(gameClearTex);
		break;
	}
}

GameScene* GameScene::GetInstance()
{
	static GameScene instance;
	return &instance;
}

int GameScene::GetSceneNum()
{
	return sceneNum;
}

void GameScene::SetSceneNum(int sceneNumber)
{
	sceneNumber = sceneNum;
}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet>& enemyBullet)
{
	enemyBullets_.push_back(std::move(enemyBullet));
}

void GameScene::Collision()
{
		//�R���C�_�[���X�g���N���A
	collisionManager_->ListClear();

	//�R���C�_�[�����X�g�ɓo�^
	//���L����
	collisionManager_->ColliderSet(player_.get());

	const std::list<std::unique_ptr<Missile>>& playerMissiles = player_->GetMissileList();
	for (const std::unique_ptr<Missile>& missile_ : playerMissiles)
	{
		collisionManager_->ColliderSet(missile_.get());
	}

	//�G�L�����̂��ׂ�
	for (const std::shared_ptr<Enemy>& enemy : enemys_)
	{
		collisionManager_->ColliderSet(enemy.get());
	}

	//�G�e�S�Ăɂ���
	for (const std::shared_ptr<EnemyBullet>& bullet : enemyBullets_)
	{
		collisionManager_->ColliderSet(bullet.get());
	}

	//�Փ˔���
	collisionManager_->CheckCollisions();
}

void GameScene::Reset()
{
	enemyBullets_.clear();
	enemys_.clear();
	destroyFlag = 0;

	gameCamera->SetPos({ 0.0f,3000.0f,0.0f });
	player_->StopAudio();
	player_.reset();
	player_ = std::make_unique<Player>();
	AliceMathF::Vector3 pos = { 0.0f, -0.0f, -25.0f };
	player_->Initialize(&pos, playerData);
	enemyData.player_ = player_;

	// �e�𐶐����A������
	AliceMathF::Vector3 position = { 0.0f,4000.0f,8000.0f };
	std::shared_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(enemyData, position);
	newEnemy->SetControlPoint(enemy1);
	//�e��o�^����
	enemys_.push_back(std::move(newEnemy));
}
