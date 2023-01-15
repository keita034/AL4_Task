#include "GameCollisionManager.h"

void GameCollisionManager::ColliderSet(OBB* collider)
{
	colliders_.push_back(collider);
}

void GameCollisionManager::ListClear()
{
	if (!colliders_.empty())
	{
		colliders_.clear();
	}
}

void GameCollisionManager::CheckCollisions()
{
		//リスト内のペアを総当たり
	std::list<OBB*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		OBB* colliderA = *itrA;

		//イテレータBイテレータAの次の要素から回す(重複判定を回避)
		std::list<OBB*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB)
		{
			OBB* colliderB = *itrB;

			//ペアの当たり判定
			CheckOBBToOBB(colliderA, colliderB);
		}
	}
}

void GameCollisionManager::CheckOBBToOBB(OBB* colliderA, OBB* colliderB)
{
	//衝突フィルタリング
	if (
		colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask() ||
		colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask())
	{

	}
	else
	{
		return;
	}

	if (Collision::CheckOBB2OBB(colliderA, colliderB))
	{
		colliderA->SetOpponentCollsionName(colliderB->GetCollsionName());
		colliderB->SetOpponentCollsionName(colliderA->GetCollsionName());

		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}
