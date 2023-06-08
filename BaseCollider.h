#pragma once

#include "CollisionTypes.h"
#include "CollisionInfo.h"
#include "Object3D.h"

/// <summary>
/// コライダー基底クラス
/// </summary>
class BaseCollider
{

public:
	BaseCollider() = default;

	//仮想デストラクタ
	virtual ~BaseCollider() = default;

	inline void SetObject(Object3D* object)
	{
		object3d_ = object;
	}

	inline Object3D* GetObject3D() { return object3d_; };

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	//形状タイプ取得
	inline CollisionShapeType GetShapeType() { return shapeType_; };

	inline void OnCollision(const CollisionInfo& Info) { object3d_->OnCollision(Info); };

protected:

	 Object3D* object3d_ = nullptr;

	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;

};

