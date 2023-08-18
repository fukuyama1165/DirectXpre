#pragma once
#include "FBXLoader.h"
#include "camera.h"
#include "IParticle.h"
#include "Vector3.h"
#include <string>
#include <memory>

class IObjEmitter
{
public:
	virtual ~IObjEmitter() {};

	//������
	virtual void Initialize() = 0;
	virtual void Initialize(const Vector3& pos, std::string particleType,std::string particleModel = "whiteBox", std::string emitterModel = "whiteBox", float ActiveTime = -1) = 0;

	//�I������
	virtual void Finalize() = 0;

	//���t���[���X�V
	virtual void Update() = 0;

	//�`��
	virtual void Draw() = 0;

	virtual void SetIsActive(bool flag) = 0;

	virtual bool GetIsActive() = 0;

	virtual uint32_t GetParticleNum() = 0;

	virtual void SetCT(float ct) = 0;

private:

	bool isActive_ = true;

	std::unique_ptr<AnimationModel> particleModel_ = nullptr;

	float CT_ = 0;

	float maxCT_ = 10;
};