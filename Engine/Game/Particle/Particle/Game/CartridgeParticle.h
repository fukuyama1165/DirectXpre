#pragma once
#include "IObjParticle.h"
#include "Object3D.h"
#include "BulletCollision.h"
#include "CubeCollider.h"


class CartridgeParticle :public IObjParticle
{
public:
	CartridgeParticle();
	~CartridgeParticle();

	//������
	void Initialize()override;
	void Initialize(const Vector3& position, const Vector3& velocity, float liveTime)override;

	//�I������
	void Finalize()override;

	//���t���[���X�V
	void Update()override;

	//�`��
	void Draw(AnimationModel* model)override;

	Object3D GetObj()override { return obj_; };

	void SetliveTime(float time)override { liveTime_ = time; };

	float GetliveTime()override { return liveTime_; };

	void SetActionTime(float time)override { actionTime_ = time; };

	float GetActionTime()override { return actionTime_; };

	void SetObjColor(Vector4 color)override { obj_.SetColor(color); };


private:

	Object3D obj_;

	//���x
	Vector3	Velocity_;

	float liveTime_ = 50;

	float actionTime_ = 50;

	float moveSpeed_ = 0;
	uint16_t hitCount_ = 0;

	//�����蔻��̏���
	BulletCollision collision;

	//�����蔻��̖{��
	CubeCollider Collider;

};