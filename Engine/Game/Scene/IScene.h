#pragma once

class IScene
{
public:
	virtual ~IScene() {};

	//������
	virtual void Initialize() {};

	//�I������
	virtual void Finalize() {};

	//���t���[���X�V
	virtual void Update() {};

	//�`��
	virtual void Draw() {};

};

