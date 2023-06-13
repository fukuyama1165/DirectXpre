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

	winApp->initialize();


	directXinit->Init(winApp->getW(), winApp->getHwnd(), winApp->getWindowSizeWidth(), winApp->getWindowSizeHeight());

	imGuiManager->Init();

	input->init(winApp->getW(), winApp->getHwnd());

}

//�I������
void IScene::Finalize()
{

	imGuiManager->Finalize();
}

//���t���[���X�V
void IScene::Update()
{

	if (winApp->processMassage() or input->TriggerKey(DIK_ESCAPE))
	{

		endRequst_ = true;

	}

	input->update();

	imGuiManager->Begin();

}
