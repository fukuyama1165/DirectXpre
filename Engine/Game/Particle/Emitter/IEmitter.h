#pragma once
#include "Vector3.h"
#include <string>

class IEmitter
{
public:
	virtual ~IEmitter() = 0;

	//������
	virtual void Initialize() = 0;
	virtual void Initialize(const Vector3& pos, std::string particleType, float ActiveTime = -1) = 0;

	//�I������
	virtual void Finalize() = 0;

	//���t���[���X�V
	virtual void Update() = 0;

	//�`��
	virtual void Draw() = 0;
};