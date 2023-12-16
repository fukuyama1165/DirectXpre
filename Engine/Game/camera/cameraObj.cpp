#include "cameraObj.h"
#include "Input.h"

cameraObj::cameraObj(float win_width,float win_height)
{
	camera_ = Camera(win_width, win_height);
}

cameraObj::~cameraObj()
{

}

void cameraObj::upDate()
{
	afterPos_ = pos_ + shakeVec;


	if (checkUseMouse_ != IsUseCameraMouse_ && !IsUseCameraMouse_)
	{
		rotate_ = mouseCameraRot;
	}
	

	if (IsUseCameraMouse_)
	{
		if (!Input::GetInstance()->PushKey(DIK_LCONTROL))
		{
			WinApp::GetInstance()->SetMousePos(WinApp::SWindowWidth_ / 2, WinApp::SWindowHeight_ / 2);
		}

		Camera mouseCamera = camera_;

		Vector3 mouseMove = Input::GetInstance()->GetMouseMove() / 1000;

		mouseCameraRot += {0, mouseMove.x, 0};

		Object3D cameobj;

		cameobj.SetPos(afterPos_);
		cameobj.SetRotate(mouseCameraRot);
		cameobj.matWorldGeneration();

		mouseCamera.eye_ = cameobj.GetWorldPos();

		Vector3 forward = { 0.0f, 0.0f, 1.0f };

		forward_ = VectorMat(forward, cameobj.GetWorldMat());

		mouseCamera.target_.x = mouseCamera.eye_.x + forward_.x;
		mouseCamera.target_.z = mouseCamera.eye_.z + forward_.z;
		mouseCamera.target_.y -= mouseMove.y + forward_.y;



		Vector3 up(0, 1, 0);

		mouseCamera.up_ = VectorMat(up, cameobj.GetWorldMat());

		camera_ = mouseCamera;
	}
	else
	{
		Object3D cameobj;

		cameobj.SetPos(afterPos_);
		cameobj.SetRotate(rotate_);
		cameobj.matWorldGeneration();

		camera_.eye_ = cameobj.GetWorldPos();

		Vector3 forward = { 0.0f, 0.0f, 1.0f };

		forward_ = VectorMat(forward, cameobj.GetWorldMat());

		camera_.target_ = camera_.eye_ + forward_;

		Vector3 up(0, 1, 0);

		camera_.up_ = VectorMat(up, cameobj.GetWorldMat());

	}

	camera_.upDate();

	checkUseMouse_ = IsUseCameraMouse_;

}

void cameraObj::CameraShake(Vector3 vec, Vector3 power)
{
	shekeSinNum += vec;

	if (vec == Vector3{ 0,0,0 })
	{
		shekeSinNum = { 0,0,0 };
	}

	shakeVec = { (sinf(shekeSinNum.x)) * power.x, (sinf(shekeSinNum.y)) * power.y, (sinf(shekeSinNum.z)) * power.z };


}
