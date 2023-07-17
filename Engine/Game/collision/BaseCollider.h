#pragma once

#include "CollisionTypes.h"
#include "CollisionInfo.h"
#include "Object3D.h"

/// <summary>
/// �R���C�_�[���N���X
/// </summary>
class BaseCollider
{

public:
	BaseCollider() = default;

	//���z�f�X�g���N�^
	virtual ~BaseCollider() = default;

	void SetObject(Object3D* object)
	{
		object3d_ = object;
	}

	Object3D* GetObject3D() { return object3d_; };

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	//�`��^�C�v�擾
	CollisionShapeType GetShapeType() { return shapeType_; };

	void OnCollision(const CollisionInfo& Info) { object3d_->OnCollision(Info); };

protected:

	 Object3D* object3d_ = nullptr;

	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;

};

