#include "Wall.h"
#include <imgui.h>

WallObj::WallObj()
{
}

WallObj::~WallObj()
{
}

void WallObj::Init()
{

	obj_.FBXInit();

	collision_ = MobCollision("Wall");

	collider_.SetObject(&collision_);

	collider_.Initialize();

	collider_.size_ = obj_.Scale_;

	CollisionManager::GetInstance()->AddCollider(&collider_);
}

void WallObj::Update()
{

	obj_.Update();

	/*ImGui::Begin("check");

	ImGui::Text("tag:%s", collider_.GetCollisionObj()->tag_.c_str());
	ImGui::Text("pos:%0.3f,%0.3f,%0.3f", collider_.GetCollisionObj()->collisionObj_.GetWorldPos().x, collider_.GetCollisionObj()->collisionObj_.GetWorldPos().y, collider_.GetCollisionObj()->collisionObj_.GetWorldPos().z);

	ImGui::End();*/

	collider_.Update(obj_.GetWorldPos());

}

void WallObj::Draw(AnimationModel* model)
{
	obj_.FBXDraw(*model);

	//collision_.Draw(model);
}