#pragma once

/**
 * @file WallObj.h
 * @brief ゲーム内のプレイヤーや敵の弾を防ぐ壁の処理をを行うクラス
 * @author フクヤマ
 * @date 2023_11/10
 */

#include "Object3D.h"
#include "MobCollision.h"
#include "SphereCollider.h"
#include "CubeCollider.h"

class WallObj
{
public:
	WallObj();
	~WallObj();

	void Init(std::string tag = "Wall");
	void Update();
	void Draw(AnimationModel* model);

	Object3D obj_;

private:

	

	//当たり判定の処理
	MobCollision collision_;

	//当たり判定の本体
	CubeCollider collider_;

};

