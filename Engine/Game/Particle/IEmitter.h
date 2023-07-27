#pragma once

class IEmitter
{
public:
	virtual ~IEmitter() {};

	//������
	virtual void Initialize() {};

	//�I������
	virtual void Finalize() {};

	//���t���[���X�V
	virtual void Update() {};

	//�`��
	virtual void Draw() {};
};