#pragma once

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

protected:

	ICollisionObj* collisionObject_ = nullptr;

	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;

};

