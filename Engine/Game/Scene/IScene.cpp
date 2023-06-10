#include "IScene.h"

void IScene::Ran()
{

	//������
	Initialize();

	//�Q�[�����[�v
	while (true)
	{

		//���t���[���X�V
		Update();

		//�I�����N�G�X�g��������I��
		if (IsEndRequst())
		{
			break;
		}

		Draw();

	}

	Finalize();

}

//������
void IScene::Initialize()
{

}

//�I������
void IScene::Finalize()
{

}

//���t���[���X�V
void IScene::Update()
{

}
