#pragma once

class IParticle
{
public:
	virtual ~IParticle() {};

	//������
	virtual void Initialize() {};

	//�I������
	virtual void Finalize() {};

	//���t���[���X�V
	virtual void Update() {};

	//�`��
	virtual void Draw() {};
};

