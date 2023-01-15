#include "Radar.h"

Radar::Radar()
{
}

Radar::~Radar()
{
}

void Radar::Initialize(RadarData& radarData)
{
	radar.reset(SpriteCreat::Create2DSprite());
	radarTex = radarData.radarTex;
	radarTrans.translation.x = 140;
	radarTrans.translation.y = 580;

	radarPlayer.reset(SpriteCreat::Create2DSprite());
	radarPlayerTex = radarData.radarPlayerTex;
	radarPlayerTrans.translation.x = 140;
	radarPlayerTrans.translation.y = 580;
	radarPlayerTrans.rotation.y = 0.0f;

	radarEnemyTex = radarData.radarEnemyTex;

	radarBulletTex = TextureManager::GetInstance()->LoadTexture(L"Resources/img/radarBullet.png");

}

void Radar::Update(float playerX, float playerZ, float playerAngle, std::list<std::shared_ptr<Enemy>>enemys, std::list<std::shared_ptr<EnemyBullet>>enemyBullets)
{
	radarPlayerTrans.translation.x = 140 + playerX / 120;
	radarPlayerTrans.translation.y = 580 - playerZ / 120;
	radarPlayerTrans.rotation.z = playerAngle;

	radarPlayer->Update(radarPlayerTrans);
	radar->Update(radarTrans);
	
	//Enemy
	radarEnemys.clear();
	radarBullet.clear();

	for (std::shared_ptr<Enemy>& enemy : enemys)
	{
		Transform radarTmp;
		std::unique_ptr<Sprite2D> tmp;

		tmp.reset(SpriteCreat::Create2DSprite());

		radarTmp.translation.x = 140;
		radarTmp.translation.y = 580;
		radarTmp.rotation.y = 0.0f;

		radarTmp.translation.x = 140 + enemy->GetWorldPosition().x / 120;
		radarTmp.translation.y = 580 - enemy->GetWorldPosition().z / 120;
		radarTmp.rotation.z = enemy->GetTransform().rotation.y;

		tmp->Update(radarTmp);
		radarEnemys.push_back(move(tmp));
	}

	for (std::shared_ptr<EnemyBullet>& enemyBullet : enemyBullets)
	{
		Transform radarTmp;
		std::unique_ptr<Sprite2D> tmp;

		tmp.reset(SpriteCreat::Create2DSprite());

		radarTmp.translation.x = 140;
		radarTmp.translation.y = 580;
		radarTmp.rotation.y = 0.0f;

		radarTmp.translation.x = 140 + enemyBullet->GetWorldPosition().x / 120;
		radarTmp.translation.y = 580 - enemyBullet->GetWorldPosition().z / 120;
		radarTmp.rotation.z = enemyBullet->GetTransform().rotation.y;

		tmp->Update(radarTmp);
		radarBullet.push_back(move(tmp));
	}

}

void Radar::Draw()
{

	radar->Draw(radarTex);

	for (std::unique_ptr<Sprite2D>& enemyBullet : radarBullet)
	{
		enemyBullet->Draw(radarBulletTex);
	}

	for (std::unique_ptr<Sprite2D>& enemy : radarEnemys)
	{
		enemy->Draw(radarEnemyTex);
	}

	radarPlayer->Draw(radarPlayerTex);
}

Radar* Radar::GetInstance()
{
	static Radar instance;
	return &instance;
}
