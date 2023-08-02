#pragma once

#include "ICollisionObj.h"
#include "FBXLoader.h"

class MobCollision :public ICollisionObj
{
public:
	MobCollision();
	MobCollision(std::string tag);
	~MobCollision();

	//初期化
	void Initialize()override;

	//終了処理
	void Finalize()override;

	//毎フレーム更新
	void Update(const Vector3 pos)override;

	//描画
	void Draw(AnimationModel* model);

	void OnCollision(const CollisionInfo& info);

	void SetIsHit(bool flag) { isHit = flag; };

	bool isHit = false;

private:

};