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

	winApp_->initialize();


	directXinit_->Init();

	imGuiManager_->Init();

	input_->init();

	xAudio_->Init();
	
	Texture::GetInstance()->loadTexture("Resources/white1x1.png");

	postEffect_ = std::make_shared<PostEffect>();

	postEffect_->Initialize();

}

//�I������
void Framework::Finalize()
{
	sceneManager_->Finalize();
	XAudio::GetInstance()->Final();
	imGuiManager_->Finalize();
}

//���t���[���X�V
void Framework::Update()
{

	if (winApp_->processMassage() or input_->TriggerKey(DIK_ESCAPE))
	{

		endRequst_ = true;

	}

	input_->update();

	imGuiManager_->Begin();

	sceneManager_->Update();

	postEffect_->Update();

	imGuiManager_->End();
}

void Framework::Draw()
{

	postEffect_->PreDrawScene();

	sceneManager_->Draw();

	postEffect_->PostDrawScene();

	DirectXInit::GetInstance()->DrawStart();

	postEffect_->Draw();

	ImGuiManager::GetInstance()->Draw();

	DirectXInit::GetInstance()->DrawEnd();


}
