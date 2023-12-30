#pragma once

/**
 * @file CollisionInfo.h
 * @brief 当たり判定で情報を入れる為のクラス
 * @author フクヤマ
 * @date 2023_12/29
 */

#include "Vector3.h"

class ICollisionObj;
class BaseCollider;


/// <summary>
/// 衝突情報
/// </summary>
struct CollisionInfo
{
	
public:
	CollisionInfo(ICollisionObj* Object, BaseCollider* Collider, const Vector3& Inter)
	{

		object_ = Object;

		collider_ = Collider;

		inter_ = Inter;


	}

	//衝突相手のオブジェクト(コライダーから引っ張ってこれるのでめんどくさくなったら消してもいい)
	ICollisionObj* object_ = nullptr;

	//衝突相手のコライダー
	BaseCollider* collider_ = nullptr;

	//衝突点
	Vector3 inter_;

};

