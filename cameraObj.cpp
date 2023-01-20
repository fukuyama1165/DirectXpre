#include "cameraObj.h"

cameraObj::cameraObj(float win_width, float win_height)
{
	camera = Camera(win_width, win_height);
}

cameraObj::~cameraObj()
{

}

void cameraObj::upDate()
{
	cameobj.SetPos(pos);
	cameobj.SetRotate(rotate);
	cameobj.matWorldGeneration();

	camera.eye_ = cameobj.GetWorldPos();

	forward = { 0.0f, 0.0f, 1.0f };

	forward = VectorMat(forward, cameobj.GetWorldMat());

	camera.target_ = camera.eye_ + forward;

	Vector3 up(0, 1, 0);

	camera.up_ = VectorMat(up, cameobj.GetWorldMat());

	camera.upDate();

}
