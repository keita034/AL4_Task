#pragma once
#include"ErrorException.h"
#include"Collision.h"
#include"CollisionPrimitive.h"

class GameCollisionManager
{
private:
//�R���C�_�[���X�g
	std::list<OBB*> colliders_;

public:
  /// <summary>
  /// �R���C�_�[�����X�g�ɓo�^
  /// </summary>
	void ColliderSet(OBB* collider);

	/// <summary>
	/// �R���C�_�[���X�g���N���A
	/// </summary>
	void ListClear();

	/// <summary>
	/// �Փ˔���Ɖ���
	/// </summary
	void CheckCollisions();

private:
	void CheckOBBToOBB(OBB* colliderA, OBB* colliderB);

};

