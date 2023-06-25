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

	SpriteCommon::GetInstance()->initialize();
	Texture::GetInstance()->loadTexture("Resources/white1x1.png");

	postEffect_ = std::make_shared<PostEffect>();

	postEffect_->initialize(SpriteCommon::GetInstance(),1);

	postEffect_->scaleX_ = 100.0f;
	postEffect_->scaleY_ = 100.0f;
	postEffect_->posX_ = 500;
	postEffect_->posY_ = 200;

}

//�I������
void Framework::Finalize()
{

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
	postEffect_->Draw();
}
