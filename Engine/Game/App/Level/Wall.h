#pragma once
#include "Object3D.h"
#include "MobCollision.h"
#include "SphereCollider.h"
#include "CubeCollider.h"

class WallObj
{
public:
	WallObj();
	~WallObj();

	void Init();
	void Update();
	void Draw(AnimationModel* model);

	Object3D obj_;

private:

	

	//�����蔻��̏���
	MobCollision collision_;

	//�����蔻��̖{��
	CubeCollider collider_;

};

