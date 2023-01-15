#pragma once
#include"ErrorException.h"
#include"ModelCreate.h"
#include"Transform.h"
#include"GameCamera.h"
#include"CollisionPrimitive.h"
#include"Sprite2D.h"
#include"GameCollisionConflg.h"

//行動フェーズ
enum class Phase
{
	Approach, //接近する
	Around, //周回する

};

class GameScene;
class Player;

struct EnemyData
{
	//モデル
	std::shared_ptr<objModel> enemymodel_;
	//カメラ
	std::shared_ptr<GameCamera> camera_;
	//デストロイ画像テクスチャ
	TextureData destroyTex;
	//弾
	std::shared_ptr<objModel> bullet_;
	//プレイヤー
	std::shared_ptr<Player>player_;
	//死亡音
	uint32_t destroySE = 0;

	char PADING[4]{};
};


class Enemy :public OBB
{

private:

	GameScene* gameScene_ = nullptr;

	//モデル
	std::shared_ptr<objModel> enemymodel_;
	//カメラ
	std::shared_ptr<GameCamera> camera_;
	//ヒット画像
	std::shared_ptr<Sprite2D> hitSprite;
	//デストロイ画像テクスチャ
	TextureData destroyTex;
	//弾
	std::shared_ptr<objModel> bullet_;
	//プレイヤー
	std::shared_ptr<Player>player_;
	//死亡音
	uint32_t destroySE = 0;
	char PADING[4]{};

	//座標変換
	Transform transform_;
	Transform hitTrans;
	Transform ReticleTrans;

	//制御点
	std::vector<AliceMathF::Vector3> controlPoints_;
	AliceMathF::Vector3 initpos;
	char PADING4[4]{};

	float time = 0.0f;

	Phase phase = Phase::Approach;

	int32_t hitTimer= 0;
	AliceMathF::Vector3 targetVec_;
	float targetSize_ = 0.0f;
	AliceMathF::Vector3 velocity_;
	int32_t attackInterval = 60 * 5;
	char PADING2[4]{};
	bool isRockOnFlag = false;
	char PADING3[3]{};
	int32_t hp = 6;
	bool isDead = false;
	char PADING5[7]{};
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(EnemyData& data, AliceMathF::Vector3& position);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void SetControlPoint(std::vector<AliceMathF::Vector3>& controlPoints);

	AliceMathF::Vector3 GetWorldPosition();

	void SetRockOn(bool flag);

	bool GetRockOn();

	Transform& GetTransform();

	virtual void OnCollision();

	bool IsDead() const;//デスフラグゲッター

	void ReticleUpdate();

	Enemy();
	~Enemy();
private:

	//接近フェーズ
	void ApproachMove();

	//周回フェーズ
	void AroundMove();

	void Attack();

	bool AttackFlag();

	//コピーコンストラクタ・代入演算子削除
	Enemy& operator=(const Enemy&) = delete;
	Enemy(const Enemy&) = delete;
};

