#pragma once
#include "Object3D.h"
#include "MobCollision.h"
#include "SphereCollider.h"

class WallObj
{
public:
	WallObj();
	~WallObj();

	void Init();
	void Update(const Camera& camera);
	void Draw(AnimationModel* model);

	Object3D obj_;

private:

	

	//�����蔻��̏���
	MobCollision collision_;

	//�����蔻��̖{��
	SphereCollider collider_;

};

