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

public:

	Vector3 pos_ = {};
	Vector3 rotate_ = {};

	Vector3 forward_ = { 0.0f, 0.0f, 1.0f };

	//�{��(�P���Ɉʒu�Ƃ��Ă����g���ĂȂ��̂ł������̂�init���g�����Ƃ͂Ȃ��͂�)
	Object3D cameobj_;

private:

	//�J�����Ƃ��Ă̋@�\
	Camera camera_;

};

