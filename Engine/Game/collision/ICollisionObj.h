#pragma once

#include "Object3D.h"
#include "CollisionInfo.h"

class ICollisionObj
{
public:
	virtual ~ICollisionObj() = default;

	//������
	virtual void Initialize() {};

	//�I������
	virtual void Finalize() {};

	//���t���[���X�V
	virtual void Update(const Camera& camera, const Vector3 pos) { camera; pos; };

	//�`��
	virtual void Draw() {};

	virtual void OnCollision(const CollisionInfo& info) { info; };

	Object3D collisionObj_;

	std::string tag_ = "";

	bool isDraw_ = false;

};

