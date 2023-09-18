#pragma once
#include "camera.h"
#include "Object3D.h"
#include "WinApp.h"
class cameraObj
{
public:
	cameraObj(float win_width =(float)WinApp::SWindowWidth_,float win_height = (float)WinApp::SWindowHeight_);
	~cameraObj();

	void upDate();

	Camera GetCamera() { return camera_; };
	Camera* GetCameraP() { return &camera_; };

	void SetCamera(Camera camera) { camera_ = camera; };

	//�J�����V�F�C�N���N����
	void CameraShake(Vector3 vec,Vector3 power={1,1,1});

public:

	Vector3 pos_ = {};
	Vector3 rotate_ = {};

	Vector3 forward_ = { 0.0f, 0.0f, 1.0f };

	//�{��(�P���Ɉʒu�Ƃ��Ă����g���ĂȂ��̂ł������̂�init���g�����Ƃ͂Ȃ��͂�)
	Object3D cameobj_;

	//�}�E�X���̕����ɃJ�����𔍂��邩�ǂ���
	bool IsUseCameraMouse_ = false;

private:

	//���낢�돈����������̈ʒu
	Vector3 afterPos_ = {};

	//�V�F�C�N�p�̃x�N�g��
	Vector3 shakeVec = {};

	//�J�����Ƃ��Ă̋@�\
	Camera camera_;

	//�}�E�X�J�[�\�����J�����Ŏg���Ă���Ƃ��̊p�x
	Vector3 mouseCameraRot = {};

};

