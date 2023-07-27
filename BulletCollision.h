#pragma once
#include "ICollisionObj.h"
#include "Vector3.h"
#include "FBXLoader.h"

class BulletCollision:public ICollisionObj
{
public:
	BulletCollision(std::string tag="");
	~BulletCollision();

	//初期化
	void Initialize();

	//終了処理
	void Finalize();

	//毎フレーム更新
	void Update(const Camera& camera,const Vector3 pos);

	//描画
	void Draw(AnimationModel* model);

	void OnCollision(const CollisionInfo& info);

	Object3D collisionObj;

	bool isDraw = false;

	bool isHit = false;

private:

};

