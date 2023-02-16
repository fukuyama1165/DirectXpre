#pragma once

#include "Vector3.h"

class Object3D;
class BaseCollider;


/// <summary>
/// �Փˏ��
/// </summary>
struct CollisionInfo
{
	
public:
	CollisionInfo(Object3D* Object, BaseCollider* Collider, const Vector3& Inter)
	{

		object = Object;

		collider = Collider;

		inter = Inter;


	}

	//�Փˑ���̃I�u�W�F�N�g(�R���C�_�[������������Ă����̂ł߂�ǂ������Ȃ���������Ă�����)
	Object3D* object = nullptr;

	//�Փˑ���̃R���C�_�[
	BaseCollider* collider = nullptr;

	//�Փ˓_
	Vector3 inter;

};

