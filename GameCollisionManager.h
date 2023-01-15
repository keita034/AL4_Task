#pragma once
#include"ErrorException.h"
#include"Collision.h"
#include"CollisionPrimitive.h"

class GameCollisionManager
{
private:
//コライダーリスト
	std::list<OBB*> colliders_;

public:
  /// <summary>
  /// コライダーをリストに登録
  /// </summary>
	void ColliderSet(OBB* collider);

	/// <summary>
	/// コライダーリストをクリア
	/// </summary>
	void ListClear();

	/// <summary>
	/// 衝突判定と応答
	/// </summary
	void CheckCollisions();

private:
	void CheckOBBToOBB(OBB* colliderA, OBB* colliderB);

};

