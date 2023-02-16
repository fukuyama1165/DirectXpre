#pragma once

#include "Vector3.h"

class Object3D;
class BaseCollider;


/// <summary>
/// 衝突情報
/// </summary>
struct CollisionInfo
{
	
public:
	CollisionInfo(Object3D* Object, BaseCollider* Collider, const Vector3& Inter)
	{

		object = Object;

		collider = Collider;

		inter = Inter;


	}

	//衝突相手のオブジェクト(コライダーから引っ張ってこれるのでめんどくさくなったら消してもいい)
	Object3D* object = nullptr;

	//衝突相手のコライダー
	BaseCollider* collider = nullptr;

	//衝突点
	Vector3 inter;

};

