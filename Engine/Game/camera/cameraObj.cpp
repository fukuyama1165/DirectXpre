#include "cameraObj.h"

cameraObj::cameraObj(const float& win_width,const float& win_height)
{
	camera_ = Camera(win_width, win_height);
}

cameraObj::~cameraObj()
{

}

void cameraObj::upDate()
{
	cameobj_.SetPos(pos_);
	cameobj_.SetRotate(rotate_);
	cameobj_.matWorldGeneration();

	camera_.eye_ = cameobj_.GetWorldPos();

	forward_ = { 0.0f, 0.0f, 1.0f };

	forward_ = VectorMat(forward_, cameobj_.GetWorldMat());

	camera_.target_ = camera_.eye_ + forward_;

	Vector3 up(0, 1, 0);

	camera_.up_ = VectorMat(up, cameobj_.GetWorldMat());

	camera_.upDate();

}
