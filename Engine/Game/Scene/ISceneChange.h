#pragma once

class ISceneChenge
{
public:
	virtual ~ISceneChenge() {};

	//������
	virtual void Initialize() {};

	//�I������
	virtual void Finalize() {};

	//���t���[���X�V
	virtual void Update() {};

	//�`��
	virtual void Draw() {};

};

