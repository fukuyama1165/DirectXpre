#pragma once

/**
 * @file CollisionTypes.h
 * @brief 当たり判定の為の構造体がおいてあるだけ
 * @author フクヤマ
 * @date 2023_12/29
 */

//衝突判定の為の判断用enum
enum CollisionShapeType
{

	SHAPE_UNKNOWN = -1,//未設定

	COLLISIONSHAPE_SPHERE,//球

	COLLISIONSHAPE_CUBE,//立方体

	COLLISIONSHAPE_BOX,//直方体

};

class CollisionTypes
{
};

