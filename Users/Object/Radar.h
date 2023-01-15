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

	//�錾
	//�w�i�G
	std::unique_ptr<Sprite2D>radar;
	TextureData radarTex;
	Transform radarTrans;

	//�v���C���[
	std::unique_ptr<Sprite2D>radarPlayer;
	TextureData radarPlayerTex;
	Transform radarPlayerTrans;

	//�G
	std::list<std::unique_ptr<Sprite2D>>radarEnemys;
	TextureData radarEnemyTex;

	//�e
	std::list<std::unique_ptr<Sprite2D>>radarBullet;
	TextureData radarBulletTex;

	

public:
	Radar();
	~Radar();
	void Initialize(RadarData& radarData);
	void Update(float playerX, float playerZ, float playerAngle,std::list<std::shared_ptr<Enemy>>enemys, std::list<std::shared_ptr<EnemyBullet>>enemyBullets);
	void Draw();

	//�Q�b�^�[
	static Radar* GetInstance();

private:
	//�R�s�[�R���X�g���N�^�E������Z�q�폜
	Radar& operator=(const Radar&) = delete;
	Radar(const Radar&) = delete;
};