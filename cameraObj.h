#pragma once
#include "camera.h"
#include "Object3D.h"
class cameraObj
{
public:
	cameraObj(float win_width, float win_height);
	~cameraObj();

	void upDate();

	Camera GetCamera() { return camera; };

public:

	Vector3 pos = {};
	Vector3 rotate = {};

	Vector3 forward = { 0.0f, 0.0f, 1.0f };

	//本体(単純に位置としてしか使ってないのでこいつ自体はinit等使うことはないはず)
	Object3D cameobj;

private:

	

	//カメラとしての機能
	Camera camera;

};

