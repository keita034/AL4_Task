#pragma once
#include"ErrorException.h"
#include"AliceMathUtility.h"
#include"ModelCreate.h"
#include"GameCamera.h"
#include"Transform.h"
#include"CollisionPrimitive.h"
#include"GameCollisionConflg.h"

class Missile :public OBB
{
private:

	//モデル
	std::shared_ptr<objModel> missileModel_;
	//カメラ
	std::shared_ptr<GameCamera>camera_;
	//座標変換
	Transform missileTransform_{};
	//速度
	AliceMathF::Vector3 velocity_;
	char PADING[4]{};
	//敵の座標
	Transform* enemyTransform_ = nullptr;
	//当たるでの時間
	float period_ = 0.0f;
	bool isDead_ = false;//デスフラグ
	bool lockonFlag_ = false;
	char PADING2[2]{};
public:
	Missile();
	~Missile();
	void Initialize(std::shared_ptr<GameCamera> camera, std::shared_ptr<objModel> missileModel, Transform* enemyTransform, AliceMathF::Vector3& velocity, AliceMathF::Vector3& pos, bool lockonFlag);
	void Update();
	void Draw();
	bool IsDead() const;//デスフラグゲッター
	virtual void OnCollision()override;
private:

};

