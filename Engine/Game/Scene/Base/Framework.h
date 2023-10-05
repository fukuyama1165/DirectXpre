#pragma once
#include <memory>
#include "WinApp.h"
#include "DirectXInit.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "SceneManager.h"
#include "PostEffect.h"
#include "XAudio.h"

class Framework
{
public:

	virtual ~Framework() = default;
	
	//������
	virtual void Initialize();

	//�I������
	virtual void Finalize();

	//���t���[���X�V
	virtual void Update();

	//�`��
	virtual void Draw();

	//�I���`�F�b�N
	virtual bool IsEndRequst() { return endRequst_; };

	//�V�[���̈�A�����s
	void Ran();


	

private:

	//�Q�[�����I��点�邽�߂̃t���O
	bool endRequst_ = false;

	//windowAPI
	WinApp* winApp_ = WinApp::GetInstance();

	//directX�̏�����
	DirectXInit* directXinit_ = DirectXInit::GetInstance();

	ImGuiManager* imGuiManager_ = ImGuiManager::GetInstance();

	SceneManager* sceneManager_ = SceneManager::GetInstance();

	//���͂̏�����
	Input* input_ = Input::GetInstance();
	
	std::shared_ptr<PostEffect> postEffect_ = nullptr;

	XAudio* xAudio_ = XAudio::GetInstance();


};
