#pragma once

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
	inline void AddCollider(BaseCollider* Collider)
	{
		colliders_.push_front(Collider);
	}

	/// <summary>
	/// コライダーの削除
	/// </summary>
	/// <param name="Collider">コライダー</param>
	inline void RemoveCollider(BaseCollider* Collider)
	{
		colliders_.remove(Collider);
	}

	//すべての衝突チェック
	void CheckAllCollisions();

private:

	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager& operator=(const CollisionManager&) = delete;

	//コライダーのリスト
	std::forward_list<BaseCollider*>colliders_;


};

