#pragma once
#include "FBXLoader.h"
#include "camera.h"
#include "IParticle.h"

class IObjEmitter
{
public:
	virtual ~IObjEmitter() {};

	//������
	virtual void Initialize() {};

	//�I������
	virtual void Finalize() {};

	//���t���[���X�V
	virtual void Update(const Camera& camera) = 0;

	//�`��
	virtual void Draw() {};

	virtual void SetIsActive(bool flag) { isActive_=flag; };

	virtual bool GetIsActive() { return isActive_; };


	virtual void SetCT(float ct) { maxCT_ = ct; };

private:

	bool isActive_ = true;

	std::unique_ptr<AnimationModel> particleModel_ = nullptr;

	float CT_ = 0;

	float maxCT_ = 10;
};