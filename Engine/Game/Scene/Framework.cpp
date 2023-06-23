#include "Framework.h"

void Framework::Ran()
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
void Framework::Initialize()
{

	winApp->initialize();


	directXinit->Init();

	imGuiManager->Init();

	input->init();

}

//�I������
void Framework::Finalize()
{

	imGuiManager->Finalize();
}

//���t���[���X�V
void Framework::Update()
{

	if (winApp->processMassage() or input->TriggerKey(DIK_ESCAPE))
	{

		endRequst_ = true;

	}

	input->update();

	imGuiManager->Begin();

	sceneManager->Update();

}
