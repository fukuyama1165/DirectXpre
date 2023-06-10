#pragma once
#include <memory>

class IScene
{
public:

	virtual ~IScene() = default;
	
	//������
	virtual void Initialize();

	//�I������
	virtual void Finalize();

	//���t���[���X�V
	virtual void Update();

	//�`��
	virtual void Draw() = 0;

	//�I���`�F�b�N
	virtual bool IsEndRequst() { return endRequst_; };

	//�V�[���̈�A�����s
	void Ran();


	//�Q�[�����I��点�邽�߂̃t���O
	bool endRequst_ = false;

private:

	

};
