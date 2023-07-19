#pragma once

#include "Vector3.h"

class ICollisionObj;
class BaseCollider;


/// <summary>
/// �Փˏ��
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

	//�Փˑ���̃I�u�W�F�N�g(�R���C�_�[������������Ă����̂ł߂�ǂ������Ȃ���������Ă�����)
	ICollisionObj* object_ = nullptr;

	//�Փˑ���̃R���C�_�[
	BaseCollider* collider_ = nullptr;

	//�Փ˓_
	Vector3 inter_;

};

