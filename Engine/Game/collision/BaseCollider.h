#pragma once

#include "CollisionTypes.h"
#include "CollisionInfo.h"
#include "ICollisionObj.h"

/// <summary>
/// �R���C�_�[���N���X
/// </summary>
class BaseCollider
{

public:
	BaseCollider() = default;

	//���z�f�X�g���N�^
	virtual ~BaseCollider() = default;

	void SetObject(ICollisionObj* object)
	{
		collisionObject_ = object;
	}

	ICollisionObj* GetCollisionObj() { return collisionObject_; };

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update(const Camera& camera, const Vector3 pos) = 0;

	//�`��^�C�v�擾
	CollisionShapeType GetShapeType() { return shapeType_; };

	void OnCollision(const CollisionInfo& Info) { collisionObject_->OnCollision(Info); };

protected:

	ICollisionObj* collisionObject_ = nullptr;

	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;

};

