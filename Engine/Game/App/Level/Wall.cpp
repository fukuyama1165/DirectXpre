#include "Wall.h"
#include <imgui.h>

WallObj::WallObj()
{
}

WallObj::~WallObj()
{
}

void WallObj::Init(std::string tag)
{

	obj_.FBXInit();

	collision_ = MobCollision(tag);

	collider_.SetObject(&collision_);

	collider_.Initialize();

	collider_.size_ = obj_.Scale_;

	CollisionManager::GetInstance()->AddCollider(&collider_);
}

void WallObj::Update()
{

	obj_.Update();

	collider_.Update(obj_.GetWorldPos());

}

void WallObj::Draw(AnimationModel* model)
{
	obj_.FBXDraw(*model);

	//collision_.Draw(model);
}