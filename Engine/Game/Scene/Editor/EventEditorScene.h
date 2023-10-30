#pragma once
#include "IScene.h"
#include "Object3D.h"
#include "cameraObj.h"
#include "Sprite.h"
#include "Player.h"
#include "EventPointManager.h"
#include "JsonLevelLoader.h"
#include "ModelManager.h"
#include "Wall.h"
#include "EnemyManager.h"

class EventEditorScene : public IScene
{
public:
	EventEditorScene();
	~EventEditorScene();

	void Initialize()override;

	void Finalize()override;

	void Update()override;

	void Draw()override;

private:
	Object3D objobj3_;

	cameraObj cameobj_;

	Vector3 cameraPos_ = { 0,20,-200 };

	bool IsUseCameraMouse_ = false;

	EventPointManager* eventManager_ = nullptr;

	EnemyManager* enemys_ = nullptr;

	//モデル
	AnimationModel* testModel_ = nullptr;

};