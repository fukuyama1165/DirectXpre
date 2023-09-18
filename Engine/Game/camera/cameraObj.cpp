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

		forward = VectorMat(forward, cameobj.GetWorldMat());

		mouseCamera.target_.x = mouseCamera.eye_.x + forward.x;
		mouseCamera.target_.z = mouseCamera.eye_.z + forward.z;
		mouseCamera.target_.y -= mouseMove.y + forward.y;



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

		forward = VectorMat(forward, cameobj.GetWorldMat());

		camera_.target_ = camera_.eye_ + forward;

		Vector3 up(0, 1, 0);

		camera_.up_ = VectorMat(up, cameobj.GetWorldMat());

	}

	camera_.upDate();

}

void cameraObj::CameraShake(Vector3 vec, Vector3 power)
{
	shakeVec += { sinf(vec.x)* power.x, sinf(vec.y)* power.y, sinf(vec.z)* power.z};
}
