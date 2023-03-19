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

	itA = colliders.begin();

	//総当たりで全てをチェック
	for (itA = colliders.begin(); itA != colliders.end(); ++itA)
	{

		itB = itA;
		
		for (++itB; itB != colliders.end(); ++itB)
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
				if(Collision)

			}

		}

	}

}
