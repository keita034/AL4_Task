#pragma once
#include"ErrorException.h"
#include"Transform.h"
#include"ModelCreate.h"
#include"CollisionPrimitive.h"
#include"GameCollisionConflg.h"

class Player;

class EnemyBullet :public OBB
{

private:

	std::unique_ptr<Transform> transform_;//ワールド変換データ
	std::shared_ptr<objModel> bullet_;//モデル
	std::shared_ptr<Player> player_;
	AliceMathF::Vector3 velocity_;//速度
	static const int32_t kLifeTime = 60 * 5;//寿命<frm>
	int32_t deathTimer_ = kLifeTime;//デスタイマー
	bool isDead_ = false;//デスフラグ
	char PADING[7]{};

public:
	EnemyBullet();//コンストラクタ
	~EnemyBullet();;//デストラクタ
	void Initialize(std::shared_ptr<objModel> model, std::shared_ptr<Player> player, const AliceMathF::Vector3& position, const AliceMathF::Vector3& velocity);//初期化処理
	void Update(Camera* camera);//更新処理
	void Draw();//描画処理
	virtual void OnCollision()override;
	bool IsDead() const;//デスフラグゲッター
	AliceMathF::Vector3 GetWorldPosition();//ワールド座標を所得
	Transform& GetTransform();
private:
	//コピーコンストラクタ・代入演算子削除
	EnemyBullet& operator=(const EnemyBullet&) = delete;
	EnemyBullet(const EnemyBullet&) = delete;

};

