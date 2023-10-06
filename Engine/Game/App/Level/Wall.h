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

	

	//当たり判定の処理
	MobCollision collision_;

	//当たり判定の本体
	CubeCollider collider_;

};

