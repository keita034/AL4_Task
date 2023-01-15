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
		//���X�g���̃y�A�𑍓�����
	std::list<OBB*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		OBB* colliderA = *itrA;

		//�C�e���[�^B�C�e���[�^A�̎��̗v�f�����(�d����������)
		std::list<OBB*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB)
		{
			OBB* colliderB = *itrB;

			//�y�A�̓����蔻��
			CheckOBBToOBB(colliderA, colliderB);
		}
	}
}

void GameCollisionManager::CheckOBBToOBB(OBB* colliderA, OBB* colliderB)
{
	//�Փ˃t�B���^�����O
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
