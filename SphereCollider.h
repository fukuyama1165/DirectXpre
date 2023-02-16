#pragma once
#include "BaseCollider.h"
#include "CollisionPrimirive.h"

#include "Vector3.h"

class SphereCollider:public BaseCollider,public Sphere
{

public:
	SphereCollider(Vector3 Offset = { 0,0,0 }, float Radius = 1.0f) :offset(Offset), radius(Radius)
	{

		//���`����Z�b�g
		shapeType = COLLISIONSHAPE_SPHERE;

	}

	/// <summary>
	/// �X�V
	/// </summary>
	void Update()override;

	inline void SetRadius(float Radius) { radius = Radius; };

public:

	//�I�u�W�F�N�g���S����̃I�t�Z�b�g
	Vector3 offset;

	//���a
	float radius;
};

