#pragma once

/**
 * @file CollisionManager.h
 * @brief 当たり判定を管理するクラス
 * @author フクヤマ
 * @date 2023_12/29
 */

#include <forward_list>

class BaseCollider;

class CollisionManager
{

public://シングルトンの為のやつ
	static CollisionManager* GetInstance();

public:

	/// <summary>
	/// コライダーの追加
	/// </summary>
	/// <param name="Collider">コライダー</param>
	void AddCollider(BaseCollider* Collider)
	{
		colliders_.push_front(Collider);
	}

	/// <summary>
	/// コライダーの削除
	/// </summary>
	/// <param name="Collider">コライダー</param>
	void RemoveCollider(BaseCollider* Collider)
	{
		colliders_.remove(Collider);
	}

	//すべての衝突チェック
	void CheckAllCollisions();

	void AllRemoveCollider() { colliders_.clear(); };

private:

	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager& operator=(const CollisionManager&) = delete;

	//コライダーのリスト
	std::forward_list<BaseCollider*>colliders_;


};

