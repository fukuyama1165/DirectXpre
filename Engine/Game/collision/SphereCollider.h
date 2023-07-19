#pragma once
#include "BaseCollider.h"
#include "CollisionPrimirive.h"

#include "Vector3.h"

class SphereCollider:public BaseCollider,public Sphere
{

public:
	SphereCollider(const Vector3& Offset = { 0,0,0 },float Radius = 1.0f) :offset_(Offset), radius_(Radius)
	{

		//���`����Z�b�g
		shapeType_ = COLLISIONSHAPE_SPHERE;

	}

	~SphereCollider()
	{
		collisionObject_->Finalize();
	}

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(const Camera& camera, const Vector3 pos)override;

	void SetRadius(float Radius) { radius_ = Radius; };

public:

	//�I�u�W�F�N�g���S����̃I�t�Z�b�g
	Vector3 offset_;

	//���a
	float radius_;
};

