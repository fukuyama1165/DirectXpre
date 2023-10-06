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

			}

			//ともに立方体
			if (colA->GetShapeType() == COLLISIONSHAPE_CUBE and colB->GetShapeType() == COLLISIONSHAPE_CUBE)
			{

				Cube* CubeA = dynamic_cast<Cube*>(colA);
				Cube* CubeB = dynamic_cast<Cube*>(colB);
				Vector3 inter;
				if (Collision::CheckCube2CubeAABB(*CubeA, *CubeB))
				{
					colA->OnCollision(CollisionInfo(colB->GetCollisionObj(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetCollisionObj(), colA, inter));

				}

			}

			//片方が直方体でもう片方が立方体
			if (colA->GetShapeType() == COLLISIONSHAPE_BOX and colB->GetShapeType() == COLLISIONSHAPE_CUBE)
			{

				Rectangular* CubeA = dynamic_cast<Rectangular*>(colA);
				Cube* CubeB = dynamic_cast<Cube*>(colB);
				Vector3 inter;
				if (Collision::CheckCube2BoxAABB(*CubeA, *CubeB))
				{
					colA->OnCollision(CollisionInfo(colB->GetCollisionObj(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetCollisionObj(), colA, inter));

				}

			}
			else if (colA->GetShapeType() == COLLISIONSHAPE_CUBE and colB->GetShapeType() == COLLISIONSHAPE_BOX)
			{
				Cube* CubeA = dynamic_cast<Cube*>(colA);
				Rectangular* CubeB = dynamic_cast<Rectangular*>(colB);
				Vector3 inter;
				if (Collision::CheckCube2BoxAABB(*CubeA, *CubeB))
				{
					colA->OnCollision(CollisionInfo(colB->GetCollisionObj(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetCollisionObj(), colA, inter));

				}
			}

			//ともに直方体
			if (colA->GetShapeType() == COLLISIONSHAPE_BOX and colB->GetShapeType() == COLLISIONSHAPE_BOX)
			{
				Rectangular* CubeA = dynamic_cast<Rectangular*>(colA);
				Rectangular* CubeB = dynamic_cast<Rectangular*>(colB);
				Vector3 inter;
				if (Collision::CheckBox2BoxAABB(*CubeA, *CubeB))
				{
					colA->OnCollision(CollisionInfo(colB->GetCollisionObj(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetCollisionObj(), colA, inter));

				}
			}

		}

	}

}
