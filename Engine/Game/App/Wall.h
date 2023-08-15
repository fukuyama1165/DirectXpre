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

	

	//“–‚½‚è”»’è‚Ìˆ—
	MobCollision collision_;

	//“–‚½‚è”»’è‚Ì–{‘Ì
	CubeCollider collider_;

};

