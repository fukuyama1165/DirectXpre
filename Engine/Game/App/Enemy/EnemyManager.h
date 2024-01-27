#pragma once

/**
 * @file EnemyManager.h
 * @brief 敵キャラの管理をするクラス
 * @author フクヤマ
 * @date 2023_11/10
 */

#include "Enemy.h"
#include <list>
class EnemyManager
{

public:

	static EnemyManager* GetInstance();

	void PopEnemy( const std::string& enemyType,const Vector3& pos,const Vector3& movePoint = { 0,0,0 }, float moveSpeed = 1.0f, uint32_t bulletCT = 200);

	//初期化
	void Init();

	//更新
	void UpDate(const Vector3& playerPos);

	/// <summary>
	/// 敵の攻撃
	/// </summary>
	/// <param name="enemy">攻撃するenemy</param>
	/// <param name="playerPos">自機の位置</param>
	void EnemyAttack(Enemy enemy, const Vector3& playerPos);

	//描画
	void Draw();

	//まとめたやつを中身を全部消す
	void Reset() { enemys_.clear(); bullets_.clear(); };

private:

	EnemyManager() = default;
	~EnemyManager();

	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;
public:
	//エネミーのまとめ
	std::list<std::unique_ptr<Enemy>> enemys_;

	//モデル
	AnimationModel* bulletModel_ = nullptr;
	AnimationModel* enemyModel_ = nullptr;

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	int32_t enemyCount_ = 0;

	bool isDebugShot_ = false;

	bool isDebugShotStop_ = false;

	//移動量を追加
	float bulletSpeed_ = 1.0f;

	std::string enemyDownSound_;

};

