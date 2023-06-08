#pragma once

#include <forward_list>

class BaseCollider;

class CollisionManager
{

public://�V���O���g���ׂ̈̂��
	static CollisionManager* GetInstance();

public:

	/// <summary>
	/// �R���C�_�[�̒ǉ�
	/// </summary>
	/// <param name="Collider">�R���C�_�[</param>
	inline void AddCollider(BaseCollider* Collider)
	{
		colliders_.push_front(Collider);
	}

	/// <summary>
	/// �R���C�_�[�̍폜
	/// </summary>
	/// <param name="Collider">�R���C�_�[</param>
	inline void RemoveCollider(BaseCollider* Collider)
	{
		colliders_.remove(Collider);
	}

	//���ׂĂ̏Փ˃`�F�b�N
	void CheckAllCollisions();

private:

	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager& operator=(const CollisionManager&) = delete;

	//�R���C�_�[�̃��X�g
	std::forward_list<BaseCollider*>colliders_;


};

