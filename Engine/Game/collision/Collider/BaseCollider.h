#pragma once

/**
 * @file BaseCollider.h
 * @brief コライダー基底クラス
 * @author フクヤマ
 * @date 2023_12/29
 */

#include "CollisionTypes.h"
#include "CollisionInfo.h"
#include "ICollisionObj.h"

/// <summary>
/// コライダー基底クラス
/// </summary>
class BaseCollider
{

public:
	BaseCollider() = default;

	//仮想デストラクタ
	virtual ~BaseCollider() = default;

	void SetObject(ICollisionObj* object)
	{
		collisionObject_ = object;
	}

	ICollisionObj* GetCollisionObj() { return collisionObject_; };

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update(const Vector3 pos) = 0;

	//形状タイプ取得
	CollisionShapeType GetShapeType() { return shapeType_; };

	void OnCollision(const CollisionInfo& Info) { collisionObject_->OnCollision(Info); };

	//消すためのフラグ
	bool isEnd_ = false;

	//当たり判定処理を行わないか
	bool isActive_ = true;

protected:

	ICollisionObj* collisionObject_ = nullptr;

	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;

	

};

