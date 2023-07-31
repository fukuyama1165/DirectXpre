#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Collision.h"

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}


void CollisionManager::CheckAllCollisions()
{

	std::forward_list<BaseCollider*>::iterator itA;
	std::forward_list<BaseCollider*>::iterator itB;

	itA = colliders_.begin();

	//総当たりで全てをチェック
	for (itA = colliders_.begin(); itA != colliders_.end(); ++itA)
	{

		itB = itA;
		
		for (++itB; itB != colliders_.end(); ++itB)
		{

			BaseCollider* colA = *itA;
			BaseCollider* colB = *itB;

			//ここからタイプによっての処理を書く

			//ともに球
			if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE and colB->GetShapeType() == COLLISIONSHAPE_SPHERE)
			{

				Sphere* SphereA = dynamic_cast<Sphere*>(colA);
				Sphere* SphereB = dynamic_cast<Sphere*>(colB);
				Vector3 inter;
				if (Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter))
				{
					colA->OnCollision(CollisionInfo(colB->GetCollisionObj(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetCollisionObj(), colA, inter));

				}
				else if(!Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter))
				{
					//colA->GetCollisionObj()->SetIsHit(false);
					//colB->GetCollisionObj()->SetIsHit(false);
				}

			}

		}

	}

}
