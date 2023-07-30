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
	collider_.radius_ = collisionSize_;

	collider_.Initialize();

	CollisionManager::GetInstance()->AddCollider(&collider_);
}

void WallObj::Update(const Camera& camera)
{

	obj_.Update(camera);

	ImGui::Begin("check");

	ImGui::Text("tag:%s", collider_.GetCollisionObj()->tag_.c_str());
	ImGui::Text("pos:%0.3f,%0.3f,%0.3f", collider_.GetCollisionObj()->collisionObj_.GetWorldPos().x, collider_.GetCollisionObj()->collisionObj_.GetWorldPos().y, collider_.GetCollisionObj()->collisionObj_.GetWorldPos().z);

	ImGui::End();

	collider_.Update(camera, obj_.GetWorldPos());

}

void WallObj::Draw(AnimationModel* model)
{
	obj_.FBXDraw(*model);

	//collision_.Draw(model);
}