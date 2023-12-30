#pragma once

/**
 * @file BulletCollision.h
 * @brief コライダー弾丸
 * @author フクヤマ
 * @date 2023_12/29
 */

#include "ICollisionObj.h"
#include "FBXLoader.h"

class BulletCollision:public ICollisionObj
{
public:
	BulletCollision();
	BulletCollision(std::string tag);
	~BulletCollision();

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

