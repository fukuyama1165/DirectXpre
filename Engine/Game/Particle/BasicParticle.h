#pragma once
#include "IObjParticle.h"
#include "Object3D.h"


class BasicParticle
{
public:
	BasicParticle();
	~BasicParticle();

	//������
	void Initialize();
	void Initialize(const Vector3& position, const Vector3& velocity,float liveTime);

	//�I������
	void Finalize();

	//���t���[���X�V
	void Update();

	//�`��
	void Draw(AnimationModel* model);

	Object3D GetObj() { return obj_; };

	void SetliveTime(float time) { liveTime_ = time; liveMaxTime_ = time; };

	float GetliveTime() { return liveTime_; };

private:

	Object3D obj_;

	//���x
	Vector3	Velocity_;

	float liveTime_ = 50;
	float liveMaxTime_ = liveTime_;

};