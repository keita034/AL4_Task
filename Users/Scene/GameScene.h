#pragma once

#include"ErrorException.h"
#include"Model.h"
#include"ModelCreate.h"
#include"Camera.h"
#include"Collision.h"
#include"Input.h"
#include"SpriteCreat.h"
#include"ParticleCreat.h"
#include"Light.h"
#include"Mesh.h"
#include"3DMesh.h"
#include"Transform.h"
#include"DefaultMaterial.h"
#include"AudioManager.h"

#include"Player.h"
#include"GameCamera.h"
#include"Radar.h"
#include"Enemy.h"
#include"EnemyBullet.h"
#include"GameCollisionManager.h"

class GameScene
{
private:

	Mesh3D* mesh3D = nullptr;
	Input* input = nullptr;
	AudioManager* audioManager = nullptr;
	uint32_t gameScene = 0;
	uint32_t titleScene = 0;
	uint32_t gameClearScene = 0;
	char PADING2[4]{};
	std::unique_ptr<Sprite2D> titleBack;
	TextureData titleTex{};
	Transform titleTrans{};

	std::unique_ptr<Sprite2D> gameClear;
	TextureData gameClearTex{};
	Transform gameClearTrans{};

	std::unique_ptr<Sprite2D> gameOver;
	TextureData gameOverTex{};
	Transform gameOverTrans{};

	std::unique_ptr<Light> light;
	AliceMathF::Vector4 lightColor = { 1, 1, 1, 1 };

	//モデル
	EnemyData enemyData;
	RadarData radarData;
	PlayerData playerData;

	std::unique_ptr<objModel>skydome;
	Transform skydomeTransform;
	std::unique_ptr<objModel>ground;
	Transform groundTransform;

	std::shared_ptr<Player>player_;

	std::unique_ptr<GameCollisionManager> collisionManager_;

	std::vector<AliceMathF::Vector3> enemy0 = {
		{8000.0f,4000.0f,0.0f},
		{6128.0f,4000.0f,5142.0f},
		{1389.0f,4000.0f,7878.0f},
		{-4000.0f,4000.0f,6928.0f},
		{-7517.0f,4000.0f,2736.0f},
		{-7517.0f ,4000.0f,-2736.0f},
		{-4000.0f,4000.0f,-6928.0f},
		{1389.0f,4000.0f,-7878.0f},
		{6128.0f,4000.0f,-5142.0f},
	};

	std::vector<AliceMathF::Vector3> enemy1 = {
	{1545.0f,4000.0f,4755.0f   },
	{1545.0f,4000.0f,-4755.0f },
	{-4045.0f,4000.0f,-2938.0f  },
	{5000.0f,4000.0f,0.0f   },
	{-4045.0f,4000.0f,2938.0f  },
	{1545.0f,4000.0f,4755.0f   },
	};

	std::vector<AliceMathF::Vector3>enemy2 = {
		{4000.0f,4000.0f,6928.0f},
		{-2000.0f,4000.0f,-6928.0f},
		{8000.0f,4000.0f,0.0f},
		{8000.0f,4000.0f,0.0f},
		{-4000.0f,4000.0f,0.0f},
		{4000.0f,4000.0f,-6928.0f},
		{-4000.0f,4000.0f,-6928.0f},
		{4000.0f,4000.0f,6928.0f },
	};

	std::vector<AliceMathF::Vector3>enemy3 = {
		{-5656.0f,4000.0f,-5656.0f},
		{8000.0f,4000.0f,0.0f},
		{-5656.0f,4000.0f,5656.0f},
		{-8000.0f,4000.0f,0.0f},
		{0.0f,4000.0f,8000.0f},	
		{5656.0f,4000.0f,-5656.0f},
		{0.0f,4000.0f,8000.0f},
		{5656.0f,4000.0f,5656.0f},
		{-5656.0f,4000.0f,-5656.0f},
	};
	

	std::list<std::shared_ptr<Enemy>>enemys_;
	std::list<std::shared_ptr<EnemyBullet>>enemyBullets_;

	std::shared_ptr<GameCamera> gameCamera;

	Radar* radar_ = nullptr;
	//宣言
	INT32 sceneNum = 0;
	char PADING[4]{};

	INT32 destroyFlag = 0;
	char PADING3[4]{};
public:

	GameScene();
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//ゲッター
	static GameScene* GetInstance();
	int GetSceneNum();
	void SetSceneNum(int sceneNum);

	void AddEnemyBullet(std::unique_ptr<EnemyBullet>& enemyBullet);

	void Collision();

	void Reset();

private:

	//コピーコンストラクタ・代入演算子削除
	GameScene& operator=(const GameScene&) = delete;
	GameScene(const GameScene&) = delete;
};