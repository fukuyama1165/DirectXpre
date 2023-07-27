#pragma once

#include "Object3D.h"
#include "CollisionInfo.h"

class ICollisionObj
{
public:
	virtual ~ICollisionObj() = default;

	//初期化
	virtual void Initialize() {};

	//終了処理
	virtual void Finalize() {};

	//毎フレーム更新
	virtual void Update(const Camera& camera, const Vector3 pos) { camera; pos; };

	//描画
	virtual void Draw() {};

	virtual void OnCollision(const CollisionInfo& info) { info; };

	Object3D collisionObj_;

	std::string tag_ = "";

	bool isDraw_ = false;

};

