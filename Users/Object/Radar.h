#pragma once
#include"SpriteCreat.h"
#include"Transform.h"
#include"Enemy.h"
#include"EnemyBullet.h"

struct RadarData
{
	TextureData radarTex;
	TextureData radarPlayerTex;
	TextureData radarEnemyTex;
	TextureData radarbulletTex;
};

class Radar
{
private:

	//宣言
	//背景絵
	std::unique_ptr<Sprite2D>radar;
	TextureData radarTex;
	Transform radarTrans;

	//プレイヤー
	std::unique_ptr<Sprite2D>radarPlayer;
	TextureData radarPlayerTex;
	Transform radarPlayerTrans;

	//敵
	std::list<std::unique_ptr<Sprite2D>>radarEnemys;
	TextureData radarEnemyTex;

	//弾
	std::list<std::unique_ptr<Sprite2D>>radarBullet;
	TextureData radarBulletTex;

	

public:
	Radar();
	~Radar();
	void Initialize(RadarData& radarData);
	void Update(float playerX, float playerZ, float playerAngle,std::list<std::shared_ptr<Enemy>>enemys, std::list<std::shared_ptr<EnemyBullet>>enemyBullets);
	void Draw();

	//ゲッター
	static Radar* GetInstance();

private:
	//コピーコンストラクタ・代入演算子削除
	Radar& operator=(const Radar&) = delete;
	Radar(const Radar&) = delete;
};