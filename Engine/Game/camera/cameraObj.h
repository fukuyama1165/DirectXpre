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

	//カメラシェイクを起こす
	void CameraShake(Vector3 vec,Vector3 power={1,1,1});

public:

	Vector3 pos_ = {};
	Vector3 rotate_ = {};

	Vector3 forward_ = { 0.0f, 0.0f, 1.0f };

	//本体(単純に位置としてしか使ってないのでこいつ自体はinit等使うことはないはず)
	Object3D cameobj_;

	//マウスをの方向にカメラを剥けるかどうか
	bool IsUseCameraMouse_ = false;

private:

	//いろいろ処理をした後の位置
	Vector3 afterPos_ = {};

	//シェイク用のベクトル
	Vector3 shakeVec = {};

	//カメラとしての機能
	Camera camera_;

	//マウスカーソルをカメラで使っているときの角度
	Vector3 mouseCameraRot = {};

	//シェイク用の一時保存用変数
	Vector3 shekeSinNum = {};

};

