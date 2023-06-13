#pragma once
#include <memory>
#include "WinApp.h"
#include "DirectXInit.h"
#include "ImGuiManager.h"
#include "Input.h"

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


	

private:

	//�Q�[�����I��点�邽�߂̃t���O
	bool endRequst_ = false;

	//windowAPI
	WinApp* winApp = WinApp::GetInstance();

	//directX�̏�����
	DirectXInit* directXinit = DirectXInit::GetInstance();

	ImGuiManager* imGuiManager = ImGuiManager::GetInstance();

	//���͂̏�����
	Input* input = input->GetInstance();
	

};
