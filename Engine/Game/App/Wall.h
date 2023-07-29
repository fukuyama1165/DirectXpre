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

	

	//当たり判定の処理
	MobCollision collision_;

	//当たり判定の本体
	SphereCollider collider_;

};

