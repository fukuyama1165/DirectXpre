#pragma once
#include "BaseCollider.h"
#include "CollisionPrimirive.h"

#include "Vector3.h"

class SphereCollider:public BaseCollider,public Sphere
{

public:
	SphereCollider(Vector3 Offset = { 0,0,0 }, float Radius = 1.0f) :offset_(Offset), radius_(Radius)
	{

		//球形状をセット
		shapeType_ = COLLISIONSHAPE_SPHERE;

	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	inline void SetRadius(float Radius) { radius_ = Radius; };

public:

	//オブジェクト中心からのオフセット
	Vector3 offset_;

	//半径
	float radius_;
};

