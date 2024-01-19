#pragma once

/**
 * @file ICollisionObj.h
 * @brief 当たり判定の為継承用クラス
 * @author フクヤマ
 * @date 2023_12/29
 */

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
	virtual void Update(const Vector3 pos) { pos; };

	//描画
	virtual void Draw() {};

	virtual void OnCollision(const CollisionInfo& info) { info; };

	virtual void SetIsHit(bool flag) { isHit = flag; };

	Object3D collisionObj_;

	std::string tag_ = "";

	bool isDraw_ = false;

	bool isHit = false;

};

