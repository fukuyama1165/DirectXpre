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

	inline void SetObject(Object3D* object)
	{
		this->object3d = object;
	}

	inline Object3D* GetObject3D() { return object3d; };

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	//�`��^�C�v�擾
	inline CollisionShapeType GetShapeType() { return shapeType; };

	inline void OnCollision(const CollisionInfo& Info) { object3d->OnCollision(Info); };

protected:

	Object3D* object3d = nullptr;

	CollisionShapeType shapeType = SHAPE_UNKNOWN;

};

