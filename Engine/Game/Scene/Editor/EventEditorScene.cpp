#include "EventEditorScene.h"
#include <imgui.h>
#include "Input.h"
#include "SceneManager.h"
#include "LightManager.h"
#include "Texture.h"
#include "EmitterManager.h"
#include "LevelLoader.h"
#include <ImGuizmo.h>
#include <GraphEditor.h>

void EditTransform(Vector3& pos);

EventEditorScene::EventEditorScene()
{
}

EventEditorScene::~EventEditorScene()
{
}

void EventEditorScene::Initialize()
{
	Object3D::SetLight(&LightManager::GetInstance()->lightGroups_[0]);
	LightManager::GetInstance()->lightGroups_[0].SetAmbientColor({ 0.25f,0.25f,0.25f });
	LightManager::GetInstance()->lightGroups_[0].SetDirLightColor(0, { 0.05f,0.05f,0.05f });
	LightManager::GetInstance()->lightGroups_[0].SetDirLightActive(0, true);
	LightManager::GetInstance()->lightGroups_[0].SetDirLightDir(0, { 0,-1,0,0 });
	LightManager::GetInstance()->lightGroups_[0].SetDirLightColor(0, { 30.0f / 255.0f,30.0f / 255.0f ,30.0f / 255.0f });

	ModelManager::GetInstance()->Load("testFBX", "gltf", "TNTBox", "TNTBox");

	objobj3_.objDrawInit("Resources/obj/skydome/", "skydome.obj");
	objobj3_.SetScale({ 1000,1000,1000 });
	objobj3_.useLight_ = false;

	cameobj_ = cameraObj((float)WinApp::GetInstance()->getWindowSizeWidth(), (float)WinApp::GetInstance()->getWindowSizeHeight());

	cameobj_.pos_ = { 0,0,-50 };

	Camera::nowCamera = cameobj_.GetCameraP();

	//レベル読み込み
	LevelLoader::GetInstance()->LoadLevel("GroundTest");

	enemys_ = EnemyManager::GetInstance();

	testModel_ = ModelManager::GetInstance()->SearchModelData("basketballmanBox");
	enemyModel_ = ModelManager::GetInstance()->SearchModelData("Enemy");
	moveEventModel_ = ModelManager::GetInstance()->SearchModelData("whiteBox");
	explosionModel_ = ModelManager::GetInstance()->SearchModelData("TNTBox");

	eventManager_ = EventPointManager::GetInstance();

	player_.Init();

	eventManager_->isNoTimer = false;

	testExplosionObj.Init({ 0,20,-250 }, 0, { 1,1,1 }, { 10,10,10 }, 50.0f);
	testEnemy1.Init("Attack", { 2,20,-250 }, {}, 0, 900000);
	testEnemy2.Init("Attack", { -2,20,-250 }, {}, 0, 900000);

	testObj_.FBXInit();
	testObj2_.FBXInit();

}

void EventEditorScene::Finalize()
{
	CollisionManager::GetInstance()->AllRemoveCollider();
	eventManager_->reset();
	enemys_->Reset();
	EmitterManager::GetInstance()->reset();
}

void EventEditorScene::Update()
{

	
	
	objobj3_.Update();

	LevelLoader::GetInstance()->Update();
	

	if (!isTest_)
	{
		cameobj_.upDate();
		//デバック用の処理
		DebugUpdate();

		//イベント管理部分
		AddEvent();

		EditEvent();

		DrawEventDataUpdate();

	}

	TestEvent();

	testObj_.Update();
	testObj2_.Update();

	LightManager::GetInstance()->ALLLightUpdate();
	

}

void EventEditorScene::Draw()
{

	LevelLoader::GetInstance()->Draw();

	objobj3_.Draw();

	
	if (!isTest_)
	{

		for (auto enemyobj : enemyDatas_)
		{
			for (uint32_t i = 0; i < enemyobj.enemys.size(); i++)
			{
				enemyobj.enemys[i].FBXDraw(*enemyModel_);
				if (enemyobj.enemyTypes[i] != EnemyType::Attack)
				{
					enemyobj.endPoint[i].FBXDraw(*enemyModel_);
					enemyobj.move[i].FBXDraw(*enemyModel_);
				}
			}
			enemyobj.playerPoint.FBXDraw(*moveEventModel_);
		}

		for (auto movePointobj : movePointDatas_)
		{
			movePointobj.startPoint.FBXDraw(*moveEventModel_);
			movePointobj.endPoint.FBXDraw(*moveEventModel_);
			movePointobj.move.FBXDraw(*moveEventModel_);
		}

		for (auto explosionObj : explosionObjDatas_)
		{
			for (int32_t i = 0; i < explosionObj.obj.size(); i++)
			{
				explosionObj.obj[i].FBXDraw(*explosionModel_);
			}
			for (int32_t i = 0; i < explosionObj.obj.size(); i++)
			{
				explosionObj.explosion[i].FBXDraw(*moveEventModel_);
			}
			
		}

	}
	else
	{
		enemys_->Draw();
		player_.Draw();		
		eventManager_->Draw();
	}

	EmitterManager::GetInstance()->Draw();

	//if (!testExplosionObj.isExplosion_)
	{
		testExplosionObj.Draw(moveEventModel_);
	}

	if (testEnemy1.isAlive_)
	{
		testEnemy1.Draw(enemyModel_);
	}
	if (testEnemy2.isAlive_)
	{
		testEnemy2.Draw(enemyModel_);
	}

	//testObj_.FBXDraw(*testModel_);
	//testObj2_.FBXDraw(*testModel_);

}

void EventEditorScene::AddEvent()
{
	//イベント追加部分
	//ImGui::Begin("addEvent");

	if (!imguiAddEventWindow_)return;

	if (!ImGui::Begin("addEvent", &imguiAddEventWindow_))
	{
		ImGui::End();
		return;
	}

	//intしか使えん許さん
	testImguiFlag_ = ImGui::Combo("emitterType", (int*)&eventTypeNum_, EventTypeChar, IM_ARRAYSIZE(EventTypeChar));

	


	switch (eventTypeNum_)
	{
	case 0:

		AddMoveEvent();

		break;

	case 1:

		AddBattleEvent();

		break;
	default:
		break;
	}
		

	ImGui::End();
}

void EventEditorScene::AddMoveEvent()
{
	ImGui::DragFloat3("movePoint", movePoint_, 1.0f, -1000.0f, 1000.0f);
	ImGui::DragFloat3("movePointRot", movePointRot_, 1.0f, -1000.0f, 1000.0f);
	ImGui::DragFloat3("moveStartPoint", moveStartPoint_, 1.0f, -1000.0f, 1000.0f);
	ImGui::DragFloat("moveSpeed", &moveSpeed_, 1.0f, 0.0f, 1000.0f);
	ImGui::DragFloat("moveRotTime", &moveRotTime_, 1.0f, 0.0f, 1000.0f);
	//終了時に増える時間をセット
	ImGui::DragFloat("AddTime", &addTime_, 1.0f, 0.0f, 6000.0f);

	if (ImGui::Button("addEvent"))
	{
		EventSeting addEvent;
		addEvent.eventType = EventType::moveEvent;
		addEvent.movePoint = { movePoint_[0] ,movePoint_[1] ,movePoint_[2] };
		addEvent.movePointRot = { movePointRot_[0] ,movePointRot_[1] ,movePointRot_[2] };
		addEvent.moveStartPoint = { moveStartPoint_[0] ,moveStartPoint_[1] ,moveStartPoint_[2] };
		addEvent.moveSpeed = moveSpeed_;
		addEvent.moveRotTime = moveRotTime_;
		addEvent.addTimer = addTime_;

		//保存
		saveSeting_ = seting_;
		saveEnemyDatas_ = enemyDatas_;
		saveMovePointDatas_ = movePointDatas_;
		saveExplosionObjDatas_ = explosionObjDatas_;

		seting_.push_back(addEvent);

		//描画用のやつにコピー
		EventMovePointData addMoveEventData;

		addMoveEventData.startPoint.FBXInit();
		addMoveEventData.endPoint.FBXInit();
		addMoveEventData.move.FBXInit();

		addMoveEventData.startPoint.pos_ = { moveStartPoint_[0] ,moveStartPoint_[1] ,moveStartPoint_[2] };
		addMoveEventData.endPoint.pos_ = { movePoint_[0] ,movePoint_[1] ,movePoint_[2] };
		addMoveEventData.move.pos_ = { moveStartPoint_[0] ,moveStartPoint_[1] ,moveStartPoint_[2] };

		addMoveEventData.startPoint.Scale_ = { 0.3f,0.3f,0.3f };
		addMoveEventData.endPoint.Scale_ = { 0.3f,0.3f,0.3f };
		addMoveEventData.move.Scale_ = { 0.3f,0.3f,0.3f };

		addMoveEventData.startPoint.Update();
		addMoveEventData.endPoint.Update();
		addMoveEventData.move.Update();

		//各ポイントの色を変更

		addMoveEventData.startPoint.SetColor({ 0.0f,0.0f ,0.5f ,1.0f });
		addMoveEventData.endPoint.SetColor({ 0.5f,0.0f ,0.0f ,1.0f });
		addMoveEventData.move.SetColor({ 0.5f,0.0f ,0.5f ,1.0f });

		movePointDatas_.push_back(addMoveEventData);

		EventFlagBuff addflag;
		addflag.eventType = EventType::moveEvent;

		eventFlags_.push_back(addflag);
		
	}
}

void EventEditorScene::AddBattleEvent()
{
	//前フレームの情報を保存
	int32_t oldEnemyNum_ = enemyNum_;

	//intしかないのでintに変換
	ImGui::DragInt("enemyNum", (int*)&enemyNum_, 1, 0, 10);

	//画面の最大湧き数を設定
	ImGui::DragInt("maxSpawn", (int*)&enemyMaxSpawn_, 1, 1, 10);

	//プレイヤーがバトルするところを設定
	ImGui::DragFloat3("PlayerPos", playerPos_, 1, -1000.0f, 1000.0f);

	//終了時に増える時間をセット
	ImGui::DragFloat("AddTime", &addTime_, 1.0f, 0.0f, 6000.0f);

	//前フレームから変更されていたら大きさを変更
	if (oldEnemyNum_ != enemyNum_)
	{
		enemyTypeNum_.resize(enemyNum_);
		enemySpawnPos_.resize(enemyNum_);
		enemyMovePos_.resize(enemyNum_);
		enemyMoveSpeed_.resize(enemyNum_);
		enemySpawnInterval_.resize(enemyNum_);
		enemyBulletCT_.resize(enemyNum_);
		enemyTypes_.resize(enemyNum_);
	}

	//intしか使えん許さん
	ImGui::Combo("playerHideType", (int*)&playerHideTypeNum_, playerHideTypeChar, IM_ARRAYSIZE(playerHideTypeChar));

	switch (playerHideTypeNum_)
	{
	case playerHideVectorType::Down:
		playerHideType_ = playerHideVectorType::Down;
		break;
	case playerHideVectorType::Right:
		playerHideType_ = playerHideVectorType::Right;
		break;
	case playerHideVectorType::Left:
		playerHideType_ = playerHideVectorType::Left;
		break;
	default:
		break;
	}

	//イベント追加するよ
	if (ImGui::Button("addEvent"))
	{
		AddButtonBattleEvent();
	}

	AddBattleEventEnemy();

	ImGui::Text("ExplosionObj");

	//前フレームの情報を保存
	int32_t oldExplosionObjNum_ = explosionObjNum_;

	ImGui::DragInt("explosionObjNum", (int*)&explosionObjNum_, 1, 0, 10);

	if (oldExplosionObjNum_ != explosionObjNum_)
	{
		explosionObjPos_.resize(explosionObjNum_);
		explosionObjSize_.resize(explosionObjNum_);
		explosionObjExplosionSize_.resize(explosionObjNum_);
		explosionObjExplosionTime_.resize(explosionObjNum_);
	}

	AddBattleEventExplosionObj();
	
}

void EventEditorScene::AddButtonBattleEvent()
{
	//バトルなので敵の情報を入れましょう
	EventSeting addEvent;
	addEvent.eventType = EventType::BattleEvent;
	addEvent.enemyNum = enemyNum_;
	addEvent.enemyTypes = enemyTypes_;
	addEvent.enemyMaxSpawn = enemyMaxSpawn_;
	addEvent.enemySpawnPos = enemySpawnPos_;
	addEvent.enemyMovePos = enemyMovePos_;
	addEvent.enemySpawnInterval = enemySpawnInterval_;
	addEvent.enemyMoveSpeed = enemyMoveSpeed_;
	addEvent.enemyBulletCT = enemyBulletCT_;
	addEvent.playerHideVector = playerHideType_;
	addEvent.playerPos = { playerPos_[0],playerPos_[1] ,playerPos_[2] };
	addEvent.addTimer = addTime_;

	addEvent.explosionObjNum = explosionObjNum_;
	addEvent.explosionObjPos = explosionObjPos_;
	addEvent.explosionObjSize = explosionObjSize_;
	addEvent.explosionObjExplosionSize = explosionObjExplosionSize_;
	addEvent.explosionObjExplosionTime = explosionObjExplosionTime_;

	//操作したので保存
	saveSeting_ = seting_;
	saveEnemyDatas_ = enemyDatas_;
	saveMovePointDatas_ = movePointDatas_;
	saveExplosionObjDatas_ = explosionObjDatas_;

	seting_.push_back(addEvent);

	AddButtonBattleEventDebugObj();
	AddButtonBattleEventExplosionObjDebugObj();

	//フラグ群を追加
	EventFlagBuff addflag;
	addflag.eventType = EventType::BattleEvent;

	addflag.isEnemySpawnPoss.resize(enemyNum_);
	addflag.isEnemyMoveEndPoint.resize(enemyNum_);

	addflag.isBattleEventEnemySpawnPos.resize(enemyNum_);
	addflag.isBattleEventEnemyMovePos.resize(enemyNum_);
	addflag.isBattleEventEnemySpawnInterval.resize(enemyNum_);
	addflag.isBattleEventEnemyMoveSpeed.resize(enemyNum_);
	addflag.isBattleEventEnemyBulletCT.resize(enemyNum_);

	addflag.isExplosionObjPoints.resize(explosionObjNum_);

	addflag.isExplosionObjPos.resize(explosionObjNum_);
	addflag.isExplosionObjSize.resize(explosionObjNum_);
	addflag.isExplosionObjExplosionSize.resize(explosionObjNum_);
	addflag.isExplosionObjExplosionTime.resize(explosionObjNum_);

	eventFlags_.push_back(addflag);

	//次の設定用に中身を削除
	enemyTypeNum_.clear();
	enemySpawnPos_.clear();
	enemyMovePos_.clear();
	enemyMoveSpeed_.clear();
	enemySpawnInterval_.clear();
	enemyTypes_.clear();
	enemyNum_ = 0;
	enemyMaxSpawn_ = 1;
	playerPos_[0] = 0.0f;
	playerPos_[1] = 0.0f;
	playerPos_[2] = 0.0f;
}

void EventEditorScene::AddButtonBattleEventDebugObj()
{
	EventEnemyData add;

	Object3D playerObj;
	playerObj.FBXInit();
	playerObj.pos_ = { playerPos_[0],playerPos_[1] ,playerPos_[2] };
	playerObj.Scale_ = { 0.3f,0.3f,0.3f };
	add.playerPoint = playerObj;

	for (int32_t i = 0; i < enemyNum_; i++)
	{
		Object3D enemyObj;
		enemyObj.FBXInit();
		enemyObj.pos_ = enemySpawnPos_[i];
		enemyObj.Scale_ = { 0.5f,0.5f ,0.5f };

		add.enemys.push_back(enemyObj);

		add.enemyTypes.push_back(enemyTypes_[i]);

		Object3D endPointObj;
		endPointObj.FBXInit();
		endPointObj.pos_ = enemyMovePos_[i];
		endPointObj.SetColor({ 0.5f,0.0f ,0.0f ,1.0f });
		endPointObj.Scale_ = { 0.5f,0.5f ,0.5f };
		add.endPoint.push_back(endPointObj);

		Object3D moveObj;
		moveObj.FBXInit();
		moveObj.pos_ = enemySpawnPos_[i];
		moveObj.SetColor({ 0.5f,0.0f ,0.5f ,1.0f });
		moveObj.Scale_ = { 0.5f,0.5f ,0.5f };
		add.move.push_back(moveObj);


	}

	enemyDatas_.push_back(add);
}

void EventEditorScene::AddButtonBattleEventExplosionObjDebugObj()
{
	EventExplosionObjData add;

	for (int32_t i = 0; i < explosionObjNum_; i++)
	{
		Object3D explosionObj;
		explosionObj.FBXInit();
		explosionObj.pos_ = explosionObjPos_[i];
		explosionObj.Scale_= explosionObjSize_[i];

		add.obj.push_back(explosionObj);
		add.explosion.push_back(explosionObj);

		add.endSize.push_back(explosionObjExplosionSize_[i]);


	}

	explosionObjDatas_.push_back(add);
}

void EventEditorScene::AddBattleEventEnemy()
{
	//敵の数分回す
	for (int32_t i = 0; i < enemyNum_; i++)
	{
		ImGui::Text("enemyNum:%02d", i);

		std::string num = ("##" + std::to_string(i));

		//intしか使えん許さん
		ImGui::Combo(std::string("EnemyType" + num).c_str(), (int*)&enemyTypeNum_[i], EnemyTypeChar, IM_ARRAYSIZE(EnemyTypeChar));

		//現在の値を取得
		float spawnPos[3] = { enemySpawnPos_[i].x,enemySpawnPos_[i].y,enemySpawnPos_[i].z };
		float movePos[3] = { enemyMovePos_[i].x,enemyMovePos_[i].y,enemyMovePos_[i].z };
		float enemySpawnInterval = { enemySpawnInterval_[i] };
		float enemyMoveSpeed = { enemyMoveSpeed_[i] };
		uint32_t enemyBulletCT = { enemyBulletCT_[i] };



		switch (enemyTypeNum_[i])
		{
		case 0:



			ImGui::Text("enemytype:moveOnly");
			enemyTypes_[i] = "moveOnly";

			ImGui::DragFloat3(std::string("spawnPos" + num).c_str(), spawnPos, 1.0f, -1000.0f, 1000.0f);

			ImGui::DragFloat3(std::string("movePos" + num).c_str(), movePos, 1.0f, -1000.0f, 1000.0f);

			ImGui::DragFloat(std::string("spawnInterval" + num).c_str(), &enemySpawnInterval, 1.0f, 0.0f, 50.0f);

			ImGui::DragFloat(std::string("moveSpeed" + num).c_str(), &enemyMoveSpeed, 0.1f, 0.0f, 10.0f);

			enemyBulletCT = 10000000;

			break;
		case 1:

			ImGui::Text("enemytype:moveAttack");
			enemyTypes_[i] = "moveAttack";

			ImGui::DragFloat3(std::string("spawnPos" + num).c_str(), spawnPos, 1.0f, -1000.0f, 1000.0f);

			ImGui::DragFloat3(std::string("movePos" + num).c_str(), movePos, 1.0f, -1000.0f, 1000.0f);

			ImGui::DragFloat(std::string("spawnInterval" + num).c_str(), &enemySpawnInterval, 1.0f, 0.0f, 50.0f);

			ImGui::DragInt(std::string("enemyBulletCT" + num).c_str(), (int*)&enemyBulletCT, 1, 0, 500);

			ImGui::DragFloat(std::string("moveSpeed" + num).c_str(), &enemyMoveSpeed, 0.1f, 0.0f, 10.0f);

			break;
		case 2:

			ImGui::Text("enemytype:Attack");
			enemyTypes_[i] = "Attack";

			ImGui::DragFloat3(std::string("spawnPos" + num).c_str(), spawnPos, 1.0f, -1000.0f, 1000.0f);

			movePos[0] = { 0 };
			movePos[1] = { 0 };
			movePos[2] = { 0 };

			ImGui::DragFloat(std::string("spawnInterval" + num).c_str(), &enemySpawnInterval, 1.0f, 0.0f, 50.0f);

			ImGui::DragInt(std::string("enemyBulletCT" + num).c_str(), (int*)&enemyBulletCT, 1, 0, 500);

			enemyMoveSpeed = 0;

			break;
		default:

			break;
		}

		enemySpawnPos_[i] = { spawnPos[0],spawnPos[1] ,spawnPos[2] };
		enemyMovePos_[i] = { movePos[0],movePos[1] ,movePos[2] };
		enemySpawnInterval_[i] = enemySpawnInterval;
		enemyMoveSpeed_[i] = enemyMoveSpeed;

		enemyBulletCT_[i] = enemyBulletCT;

	}
}

void EventEditorScene::AddBattleEventExplosionObj()
{
	//爆発するオブジェクトの数分回す
	for (int32_t i = 0; i < explosionObjNum_; i++)
	{
		ImGui::Text("explosionObjNum:%02d", i);

		std::string num = ("##" + std::to_string(i));

		//現在の値を取得
		float pos[3] = { explosionObjPos_[i].x,explosionObjPos_[i].y,explosionObjPos_[i].z };
		float size[3] = { explosionObjSize_[i].x,explosionObjSize_[i].y,explosionObjSize_[i].z };
		float explosionSize[3] = { explosionObjExplosionSize_[i].x,explosionObjExplosionSize_[i].y,explosionObjExplosionSize_[i].z };
		float explosionTime = { explosionObjExplosionTime_[i] };
		
		ImGui::DragFloat3(std::string("Pos" + num).c_str(), pos, 1.0f, -1000.0f, 1000.0f);

		ImGui::DragFloat3(std::string("Objsize" + num).c_str(), size, 1.0f, 1.0f, 100.0f);

		ImGui::DragFloat3(std::string("explosionSize" + num).c_str(), explosionSize, 1.0f, 1.0f, 100.0f);

		ImGui::DragFloat(std::string("explosionTime" + num).c_str(), &explosionTime, 0.1f, 1.0f, 100.0f);

		explosionObjPos_[i] = { pos[0],pos[1] ,pos[2] };
		explosionObjSize_[i] = { size[0],size[1] ,size[2] };
		explosionObjExplosionSize_[i] = { explosionSize[0],explosionSize[1] ,explosionSize[2] };
		explosionObjExplosionTime_[i] = explosionTime;


	}
}

void EventEditorScene::EditEvent()
{
	ImGui::Begin("Eventseting");

	uint16_t eventCount = 0;
	uint16_t eventBattleCount = 0;

	//登録されているイベントデータ編集したい
	for (auto setingI = seting_.begin(); setingI != seting_.end();)
	{
		std::string num = ("##" + std::to_string(eventCount));
		if (setingI->eventType == EventType::BattleEvent)
		{
			eventBattleCount++;
		}
		if (!ImGui::CollapsingHeader(std::string("eventNum"+ std::to_string(eventCount) + num).c_str()))
		{
			eventCount++;
			setingI++;
			continue;
		}
		
		if (setingI->eventType == EventType::moveEvent)
		{
			ImGui::Text("eventNum:%02d", eventCount);
			ImGui::Text("moveEvent");
			float movePoint[3] = { setingI->movePoint.x,setingI->movePoint.y,setingI->movePoint.z };
			float movePointRot[3] = { setingI->movePointRot.x,setingI->movePointRot.y,setingI->movePointRot.z };
			float moveStartPoint[3] = { setingI->moveStartPoint.x,setingI->moveStartPoint.y,setingI->moveStartPoint.z };
			float moveSpeed = setingI->moveSpeed;
			float moveRotTime = setingI->moveRotTime;
			float addTime = setingI->addTimer;

			ImGui::DragFloat3(std::string("movePoint" + num).c_str(), movePoint, 1.0f, -1000.0f, 1000.0f);
			ImGui::DragFloat3(std::string("movePointRot" + num).c_str(), movePointRot, 1.0f, -1000.0f, 1000.0f);
			ImGui::DragFloat3(std::string("moveStartPoint" + num).c_str(), moveStartPoint, 1.0f, -1000.0f, 1000.0f);
			ImGui::DragFloat(std::string("moveSpeed" + num).c_str(), &moveSpeed, 1.0f, 0.0f, 1000.0f);
			ImGui::DragFloat(std::string("moveRotTime" + num).c_str(), &moveRotTime, 1.0f, 0.0f, 1000.0f);
			//終了時に増える時間をセット
			ImGui::DragFloat(std::string("AddTime" + num).c_str(), &addTime, 1.0f, 0.0f, 6000.0f);

			if (ImGui::Button(std::string("moveStartPointGui" + num).c_str()))EventImguizmoMoveStartPointFlag(eventCount);
			if (ImGui::Button(std::string("movePointGui" + num).c_str()))EventImguizmoMovePointFlag(eventCount);

			if (eventFlags_[eventCount].isMoveStratPoint)
			{
				Vector3 buff = { moveStartPoint[0] ,moveStartPoint[1] ,moveStartPoint[2] };
				EditTransform(buff);
				moveStartPoint[0] = buff.x;
				moveStartPoint[1] = buff.y;
				moveStartPoint[2] = buff.z;
			}
			if (eventFlags_[eventCount].isMoveEndPoint)
			{
				Vector3 buff = { movePoint[0] ,movePoint[1] ,movePoint[2] };
				EditTransform(buff);
				movePoint[0] = buff.x;
				movePoint[1] = buff.y;
				movePoint[2] = buff.z;
			}

			setingI->movePoint = { movePoint[0] ,movePoint[1] ,movePoint[2] };
			setingI->movePointRot = { movePointRot[0] ,movePointRot[1] ,movePointRot[2] };
			setingI->moveStartPoint = { moveStartPoint[0] ,moveStartPoint[1] ,moveStartPoint[2] };
			setingI->moveSpeed = moveSpeed;
			setingI->moveRotTime = moveRotTime;
			setingI->addTimer = addTime;


		}
		else if (setingI->eventType == EventType::BattleEvent)
		{
			//前フレームの情報を保存
			int32_t oldEnemyNum = setingI->enemyNum;
			int32_t enemyNum = setingI->enemyNum;
			int32_t enemyMaxSpawn = setingI->enemyMaxSpawn;

			//intしかないのでintに変換
			ImGui::DragInt("enemyNum", (int*)&enemyNum, 1, 0, 10);

			setingI->enemyNum = enemyNum;

			//画面の最大湧き数を設定
			ImGui::DragInt("maxSpawn", (int*)&enemyMaxSpawn, 1, 1, 10);

			setingI->enemyMaxSpawn = enemyMaxSpawn;

			//前フレームから変更されていたら大きさを変更
			if (oldEnemyNum != enemyNum)
			{
				setingI->enemySpawnPos.resize(enemyNum);
				setingI->enemyMovePos.resize(enemyNum);
				setingI->enemyMoveSpeed.resize(enemyNum);
				setingI->enemySpawnInterval.resize(enemyNum);
				setingI->enemyBulletCT.resize(enemyNum);
				setingI->enemyTypes.resize(enemyNum);

				//フラグもサイズを変更しておく
				eventFlags_[eventCount].isEnemySpawnPoss.resize(enemyNum);
				eventFlags_[eventCount].isEnemyMoveEndPoint.resize(enemyNum);

				eventFlags_[eventCount].isBattleEventEnemySpawnPos.resize(enemyNum);
				eventFlags_[eventCount].isBattleEventEnemyMovePos.resize(enemyNum);
				eventFlags_[eventCount].isBattleEventEnemySpawnInterval.resize(enemyNum);
				eventFlags_[eventCount].isBattleEventEnemyMoveSpeed.resize(enemyNum);
				eventFlags_[eventCount].isBattleEventEnemyBulletCT.resize(enemyNum);
			}

			float playerPos[3] = { setingI->playerPos.x,setingI->playerPos.y,setingI->playerPos.z };
			ImGui::DragFloat3(std::string("PlayerPos" + num).c_str(), playerPos, 1, -1000.0f, 1000.0f);

			if (ImGui::Button(std::string("movePlayerPosGui" + num).c_str()))EventImguizmoBattleEventPlayerPoint(eventCount);
			
			if (eventFlags_[eventCount].isBattlePlayerPoint)
			{
				Vector3 buff = { playerPos[0] ,playerPos[1] ,playerPos[2] };
				EditTransform(buff);
				playerPos[0] = buff.x;
				playerPos[1] = buff.y;
				playerPos[2] = buff.z;
			}
			


			float playerHideType = setingI->playerHideVector;
			float addTime = setingI->addTimer;
			int32_t playerHideTypeNum = (int32_t)setingI->playerHideVector;

			//intしか使えん許さん
			ImGui::Combo(std::string("playerHideType" + num).c_str(), (int*)&playerHideTypeNum, playerHideTypeChar, IM_ARRAYSIZE(playerHideTypeChar));
			//終了時に増える時間をセット
			ImGui::DragFloat(std::string("AddTime" + num).c_str(), &addTime, 1.0f, 0.0f, 6000.0f);

			switch (playerHideTypeNum)
			{
			case playerHideVectorType::Down:
				playerHideType = playerHideVectorType::Down;
				break;
			case playerHideVectorType::Right:
				playerHideType = playerHideVectorType::Right;
				break;
			case playerHideVectorType::Left:
				playerHideType = playerHideVectorType::Left;
				break;
			default:
				break;
			}

			for (uint16_t i = 0; i < setingI->enemyNum; i++)
			{
				ImGui::Text("enemyNum:%02d", i);

				//種類識別
				std::string enemyNumString = (num + "_" + std::to_string(i));

				uint32_t enemyTypeNum = 0;

				if (setingI->enemyTypes[i] == "moveOnly")
				{
					enemyTypeNum = 0;
				}
				else if (setingI->enemyTypes[i] == "moveAttack")
				{
					enemyTypeNum = 1;
				}
				else if (setingI->enemyTypes[i] == "Attack")
				{
					enemyTypeNum = 2;
				}

				

				//intしか使えん許さん
				ImGui::Combo(std::string("EnemyType" + enemyNumString).c_str(), (int*)&enemyTypeNum, EnemyTypeChar, IM_ARRAYSIZE(EnemyTypeChar));

				//現在の値を取得
				float spawnPos[3] = { setingI->enemySpawnPos[i].x,setingI->enemySpawnPos[i].y,setingI->enemySpawnPos[i].z };
				float movePos[3] = { setingI->enemyMovePos[i].x,setingI->enemyMovePos[i].y,setingI->enemyMovePos[i].z };
				float enemySpawnInterval = { setingI->enemySpawnInterval[i] };
				float enemyMoveSpeed = { setingI->enemyMoveSpeed[i] };
				uint32_t enemyBulletCT = { setingI->enemyBulletCT[i] };


				switch (enemyTypeNum)
				{
				case 0:



					ImGui::Text("enemytype:moveOnly");
					setingI->enemyTypes[i] = "moveOnly";

					ImGui::DragFloat3(std::string("spawnPos" + enemyNumString).c_str(), spawnPos, 1.0f, -1000.0f, 1000.0f);

					ImGui::DragFloat3(std::string("movePos" + enemyNumString).c_str(), movePos, 1.0f, -1000.0f, 1000.0f);

					ImGui::DragFloat(std::string("spawnInterval" + enemyNumString).c_str(), &enemySpawnInterval, 1.0f, 0.0f, 50.0f);

					ImGui::DragFloat(std::string("moveSpeed" + enemyNumString).c_str(), &enemyMoveSpeed, 0.1f, 0.0f, 10.0f);

					enemyBulletCT = 0;

					if (ImGui::Button(std::string("enemySpawnPosGui" + enemyNumString).c_str()))EventImguizmoEnemySpawnPosFlag(eventCount,i);

					if (eventFlags_[eventCount].isEnemySpawnPoss[i])
					{
						Vector3 buff = { spawnPos[0] ,spawnPos[1] ,spawnPos[2] };
						EditTransform(buff);
						spawnPos[0] = buff.x;
						spawnPos[1] = buff.y;
						spawnPos[2] = buff.z;
					}

					if (ImGui::Button(std::string("enemyMovePosGui" + enemyNumString).c_str()))EventImguizmoEnemyMoveEndPointFlag(eventCount,i);

					if (eventFlags_[eventCount].isEnemyMoveEndPoint[i])
					{
						Vector3 buff = { movePos[0] ,movePos[1] ,movePos[2] };
						EditTransform(buff);
						movePos[0] = buff.x;
						movePos[1] = buff.y;
						movePos[2] = buff.z;
					}

					break;
				case 1:

					ImGui::Text("enemytype:moveAttack");
					setingI->enemyTypes[i] = "moveAttack";

					ImGui::DragFloat3(std::string("spawnPos" + enemyNumString).c_str(), spawnPos, 1.0f, -1000.0f, 1000.0f);

					ImGui::DragFloat3(std::string("movePos" + enemyNumString).c_str(), movePos, 1.0f, -1000.0f, 1000.0f);

					ImGui::DragFloat(std::string("spawnInterval" + enemyNumString).c_str(), &enemySpawnInterval, 1.0f, 0.0f, 50.0f);

					ImGui::DragFloat(std::string("moveSpeed" + enemyNumString).c_str(), &enemyMoveSpeed, 0.1f, 0.0f, 10.0f);

					ImGui::DragInt(std::string("enemyBulletCT" + enemyNumString).c_str(), (int*)&enemyBulletCT, 1, 0, 500);

					if (ImGui::Button(std::string("enemySpawnPosGui" + enemyNumString).c_str()))EventImguizmoEnemySpawnPosFlag(eventCount, i);

					if (eventFlags_[eventCount].isEnemySpawnPoss[i])
					{
						Vector3 buff = { spawnPos[0] ,spawnPos[1] ,spawnPos[2] };
						EditTransform(buff);
						spawnPos[0] = buff.x;
						spawnPos[1] = buff.y;
						spawnPos[2] = buff.z;
					}

					if (ImGui::Button(std::string("enemyMovePosGui" + enemyNumString).c_str()))EventImguizmoEnemyMoveEndPointFlag(eventCount, i);

					if (eventFlags_[eventCount].isEnemyMoveEndPoint[i])
					{
						Vector3 buff = { movePos[0] ,movePos[1] ,movePos[2] };
						EditTransform(buff);
						movePos[0] = buff.x;
						movePos[1] = buff.y;
						movePos[2] = buff.z;
					}

					break;
				case 2:

					ImGui::Text("enemytype:Attack");
					setingI->enemyTypes[i] = "Attack";

					ImGui::DragFloat3(std::string("spawnPos" + enemyNumString).c_str(), spawnPos, 1.0f, -1000.0f, 1000.0f);

					movePos[0] = { 0 };
					movePos[1] = { 0 };
					movePos[2] = { 0 };

					ImGui::DragFloat(std::string("spawnInterval" + enemyNumString).c_str(), &enemySpawnInterval, 1.0f, 0.0f, 50.0f);

					ImGui::DragInt(std::string("enemyBulletCT" + enemyNumString).c_str(), (int*)&enemyBulletCT, 1, 0, 500);

					enemyMoveSpeed = 0;

					if (ImGui::Button(std::string("enemySpawnPosGui" + enemyNumString).c_str()))EventImguizmoEnemySpawnPosFlag(eventCount, i);

					if (eventFlags_[eventCount].isEnemySpawnPoss[i])
					{
						Vector3 buff = { spawnPos[0] ,spawnPos[1] ,spawnPos[2] };
						EditTransform(buff);
						spawnPos[0] = buff.x;
						spawnPos[1] = buff.y;
						spawnPos[2] = buff.z;
					}

					break;
				default:

					break;
				}

				setingI->enemySpawnPos[i] = { spawnPos[0],spawnPos[1] ,spawnPos[2] };
				setingI->enemyMovePos[i] = { movePos[0],movePos[1] ,movePos[2] };
				setingI->enemySpawnInterval[i] = enemySpawnInterval;
				setingI->enemyMoveSpeed[i] = enemyMoveSpeed;
				setingI->enemyBulletCT[i] = enemyBulletCT;

			}

			if (oldEnemyNum != enemyNum)
			{
				AddBattleEventDebugObjRandomAccess(eventBattleCount);
			}

			//前フレームの情報を保存
			int32_t oldExplosionObjNum = setingI->explosionObjNum;
			int32_t explosionObjNum = setingI->explosionObjNum;

			ImGui::Text("explosionObj");

			ImGui::DragInt("explosionObjNum", (int*)&explosionObjNum, 1, 0, 10);

			setingI->explosionObjNum = explosionObjNum;

			if (oldExplosionObjNum != explosionObjNum)
			{
				setingI->explosionObjPos.resize(explosionObjNum);
				setingI->explosionObjSize.resize(explosionObjNum);
				setingI->explosionObjExplosionSize.resize(explosionObjNum);
				setingI->explosionObjExplosionTime.resize(explosionObjNum);

				eventFlags_[eventCount].isExplosionObjPoints.resize(explosionObjNum);

				eventFlags_[eventCount].isExplosionObjPos.resize(explosionObjNum);
				eventFlags_[eventCount].isExplosionObjSize.resize(explosionObjNum);
				eventFlags_[eventCount].isExplosionObjExplosionSize.resize(explosionObjNum);
				eventFlags_[eventCount].isExplosionObjExplosionTime.resize(explosionObjNum);
				
				for (int32_t i = oldExplosionObjNum; i < explosionObjNum; i++)
				{
					setingI->explosionObjSize[i] = { 1,1,1 };
					setingI->explosionObjExplosionSize[i] = { 1,1,1 };
					setingI->explosionObjExplosionTime[i] = 1;
				}
			}


			//爆発するオブジェクトの数分回す
			for (int32_t i = 0; i < explosionObjNum; i++)
			{
				ImGui::Text("explosionObjNum:%02d", i);

				//種類識別
				std::string explosionNum = (num + "_" + std::to_string(i));

				//現在の値を取得
				float pos[3] = { setingI->explosionObjPos[i].x,setingI->explosionObjPos[i].y,setingI->explosionObjPos[i].z };
				float size[3] = { setingI->explosionObjSize[i].x,setingI->explosionObjSize[i].y,setingI->explosionObjSize[i].z };
				float explosionSize[3] = { setingI->explosionObjExplosionSize[i].x,setingI->explosionObjExplosionSize[i].y,setingI->explosionObjExplosionSize[i].z };
				float explosionTime = { setingI->explosionObjExplosionTime[i] };

				ImGui::DragFloat3(std::string("Pos" + explosionNum).c_str(), pos, 1.0f, -1000.0f, 1000.0f);

				ImGui::DragFloat3(std::string("Objsize" + explosionNum).c_str(), size, 1.0f, 1.0f, 100.0f);

				ImGui::DragFloat3(std::string("explosionSize" + explosionNum).c_str(), explosionSize, 1.0f, 1.0f, 100.0f);

				ImGui::DragFloat(std::string("explosionTime" + explosionNum).c_str(), &explosionTime, 0.1f, 1.0f, 100.0f);

				if (ImGui::Button(std::string("explosionObjPosGui" + explosionNum).c_str()))EventImguizmoExplosionObjPosFlag(eventCount, i);

				if (eventFlags_[eventCount].isExplosionObjPoints[i])
				{
					Vector3 buff = { pos[0] ,pos[1] ,pos[2] };
					EditTransform(buff);
					pos[0] = buff.x;
					pos[1] = buff.y;
					pos[2] = buff.z;
				}

				setingI->explosionObjPos[i] = { pos[0],pos[1] ,pos[2] };
				setingI->explosionObjSize[i] = { size[0],size[1] ,size[2] };
				setingI->explosionObjExplosionSize[i] = { explosionSize[0],explosionSize[1] ,explosionSize[2] };
				setingI->explosionObjExplosionTime[i] = explosionTime;


			}
			
			if (oldExplosionObjNum != explosionObjNum)
			{
				AddBattleEventExplosionObjDebugObjRandomAccess(eventBattleCount);
			}


			setingI->playerHideVector = playerHideType;
			setingI->playerPos = { playerPos[0],playerPos[1] ,playerPos[2] };
			setingI->addTimer = addTime;
		}

		if (ImGui::Button(std::string("erase" + num).c_str()))
		{
			//一つしかないなら
			if (seting_.size() == 1)
			{
				//それしかないなら全部消す
				seting_.clear();
				enemyDatas_.clear();
				movePointDatas_.clear();
				explosionObjDatas_.clear();
				eventFlags_.clear();
				break;

			}
			else
			{
				uint16_t battleNum = 0;
				uint16_t moveNum = 0;
				auto eventFlag = eventFlags_.begin();

				for (auto setingJ = seting_.begin(); setingJ != seting_.end(); setingJ++)
				{

					if (setingJ->eventType == EventType::BattleEvent)
					{
						battleNum++;

						if (setingJ == setingI)
						{
							enemyDatas_[battleNum-1].isEnd = true;
							explosionObjDatas_[battleNum-1].isEnd = true;
							break;
						}

					}
					else if (setingJ->eventType == EventType::moveEvent)
					{

						moveNum++;

						if (setingJ == setingI)
						{
							movePointDatas_[moveNum-1].isEnd = true;
							break;
						}

					}
					
					eventFlag++;

				}
				eventFlags_.erase(eventFlag);
				setingI = seting_.erase(setingI);
				continue;
			}
		}

		eventCount++;
		setingI++;
	}


	ImGui::End();
}

void EventEditorScene::SaveAndLoadEvent()
{
	ImGui::Begin("saveAndLoad EventFile");


	if (ImGui::Button("saveFile"))
	{

		WindowsSaveEEFMFile();

	}

	if (ImGui::Button("openFile"))
	{

		isLoad_= WindowsOpenEEFMFile();	

	}

	if (!isLoad_)
	{
		ImGui::Text("Loading failed");
		ImGui::Text(std::string("reason " + loadErrorText_).c_str());
	}

	ImGui::End();
}

void EventEditorScene::DrawEventDataUpdate()
{
	if (seting_.size() == 0)
	{
		return;
	}

	for (auto enemyDatas = enemyDatas_.begin(); enemyDatas != enemyDatas_.end();)
	{
		if (enemyDatas->isEnd)
		{
			enemyDatas = enemyDatas_.erase(enemyDatas);
			continue;
		}
		enemyDatas++;
	}

	for (auto movePointDatas = movePointDatas_.begin(); movePointDatas != movePointDatas_.end();)
	{
		if (movePointDatas->isEnd)
		{
			movePointDatas = movePointDatas_.erase(movePointDatas);
			continue;
		}
		movePointDatas++;
	}

	for (auto explosionObjDatas = explosionObjDatas_.begin(); explosionObjDatas != explosionObjDatas_.end();)
	{
		if (explosionObjDatas->isEnd)
		{
			explosionObjDatas = explosionObjDatas_.erase(explosionObjDatas);
			continue;
		}
		explosionObjDatas++;
	}

	auto moveobj = movePointDatas_.begin();

	if (movePointDatas_.size() != 0)
	{

		for (auto setingI = seting_.begin(); setingI != seting_.end();)
		{
			if (setingI->eventType != EventType::moveEvent)
			{
				setingI++;
				continue;
			}

			moveobj->startPoint.pos_ = setingI->moveStartPoint;
			moveobj->startPoint.Update();
			moveobj->endPoint.pos_ = setingI->movePoint;
			moveobj->endPoint.Update();

			setingI++;
			moveobj++;
		}
	}

	auto enemyobj = enemyDatas_.begin();

	if (enemyDatas_.size() != 0 && enemyobj->enemys.size() != 0)
	{

		for (auto setingI = seting_.begin(); setingI != seting_.end();)
		{
			if (setingI->eventType != EventType::BattleEvent)
			{
				setingI++;
				continue;
			}

			for (uint32_t i = 0; i < enemyobj->enemys.size(); i++)
			{
				enemyobj->enemys[i].pos_ = setingI->enemySpawnPos[i];
				enemyobj->endPoint[i].pos_ = setingI->enemyMovePos[i];
				enemyobj->enemyTypes[i] = setingI->enemyTypes[i];
				enemyobj->enemys[i].Update();
				enemyobj->endPoint[i].Update();
			}

			enemyobj->playerPoint.pos_ = setingI->playerPos;
			enemyobj->playerPoint.Update();

			setingI++;
			enemyobj++;
		}





		for (auto enemyPointobj : enemyDatas_)
		{

			for (auto setingI = seting_.begin(); setingI != seting_.end();)
			{
				if (setingI->eventType != EventType::BattleEvent)
				{
					setingI++;
					continue;
				}
				for (uint16_t i = 0; i < enemyPointobj.enemyTypes.size(); i++)
				{
					if (enemyPointobj.enemyTypes[i] == EnemyType::Attack)
					{
						continue;
					}
					enemyPointobj.move[i].pos_ = lerp(enemyPointobj.enemys[i].GetWorldPos(), enemyPointobj.endPoint[i].GetWorldPos(), moveEventMoveTimer / moveEventMoveMaxTime);
					enemyPointobj.endPoint[i].Update();
					enemyPointobj.move[i].Update();
				}

				setingI++;
			}



		}
	}

	for (auto movePointobj : movePointDatas_)
	{

		for (auto setingI = seting_.begin(); setingI != seting_.end();)
		{
			if (setingI->eventType != EventType::moveEvent)
			{
				setingI++;
				continue;
			}

			movePointobj.move.pos_ = lerp(movePointobj.startPoint.GetWorldPos(), movePointobj.endPoint.GetWorldPos(), moveEventMoveTimer / moveEventMoveMaxTime);
			movePointobj.startPoint.Update();
			movePointobj.endPoint.Update();
			movePointobj.move.Update();

			setingI++;
		}



	}

	auto explosionObj = explosionObjDatas_.begin();

	if (explosionObjDatas_.size() != 0)
	{

		for (auto setingI = seting_.begin(); setingI != seting_.end();)
		{
			if (setingI->eventType != EventType::BattleEvent)
			{
				setingI++;
				continue;
			}
			if (setingI->explosionObjPos.size() == 0)
			{
				//最後ではないなら
				if (explosionObjDatas_.end() != explosionObj)
				{
					explosionObj++;
				}
				setingI++;
				continue;
			}

			for (uint32_t i = 0; i < explosionObj->obj.size(); i++)
			{
				explosionObj->obj[i].pos_ = setingI->explosionObjPos[i];
				explosionObj->obj[i].Scale_ = setingI->explosionObjSize[i];
				explosionObj->explosion[i].pos_ = setingI->explosionObjPos[i];
				explosionObj->explosion[i].Scale_ = lerp(setingI->explosionObjSize[i], setingI->explosionObjSize[i]+setingI->explosionObjExplosionSize[i], moveEventMoveTimer / moveEventMoveMaxTime);
				explosionObj->explosion[i].SetColor({ 1.0f,0.0f,0.0f,0.2f });
				explosionObj->obj[i].Update();
				explosionObj->explosion[i].Update();
			}

			
			setingI++;
			explosionObj++;
		}

	}

	if (moveEventMoveTimer < moveEventMoveMaxTime)
	{
		moveEventMoveTimer++;
	}
	else
	{
		moveEventMoveTimer = 0;
	}
}

void EventEditorScene::SaveEventData(const std::string& fileName)
{
	
	std::string name = fileName;

	if (fileName == "")
	{
		name = std::string("jsonEventdata");
	}

	nlohmann::json jsonfile;

	jsonfile["name"] = "event";

	for (auto eventSeting : seting_)
	{
		nlohmann::json data;
		if (eventSeting.eventType == EventType::moveEvent)
		{
			data["seting"]["movePoint"] = { eventSeting.movePoint.x,eventSeting.movePoint.y,eventSeting.movePoint.z };
			data["seting"]["movePointRot"] = { eventSeting.movePointRot.x,eventSeting.movePointRot.y,eventSeting.movePointRot.z };
			data["seting"]["moveStartPoint"] = { eventSeting.moveStartPoint.x,eventSeting.moveStartPoint.y,eventSeting.moveStartPoint.z };
			data["seting"]["moveSpeed"] = eventSeting.moveSpeed;
			data["seting"]["moveRotTime"] = eventSeting.moveRotTime;
			data["type"] = "moveEvent";
			data["seting"]["addTime"] = eventSeting.addTimer;
		}
		else if (eventSeting.eventType == EventType::BattleEvent)
		{
			data["seting"]["enemyMaxSpawn"] = eventSeting.enemyMaxSpawn;
			data["seting"]["enemyNum"] = eventSeting.enemyNum;
			for (uint16_t i = 0; i < eventSeting.enemyNum; i++)
			{
				data["seting"]["spawnPoint"] += { eventSeting.enemySpawnPos[i].x,eventSeting.enemySpawnPos[i].y,eventSeting.enemySpawnPos[i].z};
				
				data["seting"]["enemyMovePos"] += { eventSeting.enemyMovePos[i].x, eventSeting.enemyMovePos[i].y, eventSeting.enemyMovePos[i].z };
			}
			data["seting"]["spawnInterval"] = eventSeting.enemySpawnInterval;
			data["seting"]["enemyType"] = eventSeting.enemyTypes;
			data["seting"]["enemySpeed"] = eventSeting.enemyMoveSpeed;
			data["seting"]["enemyBulletCT"] = eventSeting.enemyBulletCT;
			data["type"] = "BattleEvent";
			data["seting"]["playerHideType"] = eventSeting.playerHideVector;
			data["seting"]["addTime"] = eventSeting.addTimer;
		}
		jsonfile["events"] += { data };
	}
	
	//書き込みこの名前で
	std::ofstream ofs("Resources/eefm/" + name + ".eefm");
	if (ofs) {
		ofs << jsonfile.dump(4);
	}

}

void EventEditorScene::ChangeMap()
{
	//ImGui::Begin("ChangeMap");


	//if (ImGui::Button("selectMap"))/*ImGui::SameLine(); HelpMarker("You can input value using the scientific notation,\n""  e.g. \"1e+8\" becomes \"100000000\".");*/
	//{
		wchar_t filePath[MAX_PATH] = { 0 };
		OPENFILENAME a = {};
		//構造体の大きさ基本的にこれ
		a.lStructSize = sizeof(OPENFILENAME);
		//使いたい(占有)ウインドウハンドル
		a.hwndOwner = WinApp::GetInstance()->getHwnd();
		//フィルターを設定?
		a.lpstrFilter = L"マップデータ\0 * .json*\0"
			L"すべてのファイル (*.*)\0*.*\0";
		//何個目のフィルターを使うん?みたいな感じ?
		a.nFilterIndex = 0;
		//保存の時ファイル名を入れるやつ?
		a.lpstrFile = filePath;
		//ファイルのバッファの大きさ？
		a.nMaxFile = MAX_PATH;

		std::string test;
		auto old = std::filesystem::current_path();
		if (GetOpenFileName(&a))
		{
			test = DirectXpre::Util::WStringToString(filePath);

			test = DirectXpre::Util::SeparateFilePath(test);
			
			

		}
		std::filesystem::current_path(old);

		if (test == "")
		{
			//キャンセルされたら
			LevelLoader::GetInstance()->LoadLevel("GroundTest");
		}
		else
		{
			//設定されたマップを読み込みなおす
			LevelLoader::GetInstance()->LoadLevel(test);
		}
		
	/*}

	ImGui::End();*/
}

void EventEditorScene::TestEvent()
{
	

	if (isTest_)
	{

		if (!pause_)
		{
			

			enemys_->UpDate(player_.playerCamera_.GetCamera().eye_);

			CollisionManager::GetInstance()->CheckAllCollisions();

			eventManager_->Update();
		}
		player_.Update();
		

		ImGui::Begin("player");

		ImGui::Text("rotate:%0.2f,%0.2f,%0.2f", player_.playerCamera_.rotate_.x, player_.playerCamera_.rotate_.y, player_.playerCamera_.rotate_.z);

		ImGui::End();

		ImGui::Begin("eventNum");
		
		eventNum_ = eventManager_->GetEventCount();

		//イベントの位置を指定
		ImGui::DragInt("maxSpawn", (int*)&eventNum_, 1, 0, (int)seting_.size());

		ImGui::End();

		if (eventManager_->GetEventCount() != eventNum_)
		{
			//変更されていたならそこに指定する
			eventManager_->MoveEventNum(eventNum_-1);
			eventManager_->SetEventAllEnd(false);
			enemys_->Reset();
			player_.Reset();
		}

		ImGui::Begin("test");

		if (ImGui::Button("testStop"))
		{
			Camera::nowCamera = cameobj_.GetCameraP();
			eventManager_->reset();
			eventManager_->SetEventAllEnd(false);
			enemys_->Reset();
			player_.Reset();
			ExplosionObjManager::GetInstance()->Reset();
			isTest_ = !isTest_;
		}

		if (ImGui::Button("pause"))
		{
			pause_ = !pause_;
		}

		ImGui::End();

		TestDebugUpdate();
	}

	if (!pause_)
	{
		EmitterManager::GetInstance()->Update();
	}

	/*ImGui::Begin("Test");

	ImGui::Text("日本語テスト");

	ImGui::DragFloat2("tile", test_, 0.1f, 0.0f, 10.0f);

	ImGui::End();

	objobj3_.SetMaterialTiring({ test_[0],test_[1] });*/


}

void EventEditorScene::DebugUpdate()
{
	if (Input::GetInstance()->PushKey(DIK_UP))
	{
		cameraPos_.x += Vector3::normalize(Camera::nowCamera->forward_).x;
		cameraPos_.z += Vector3::normalize(Camera::nowCamera->forward_).z;
	}
	if (Input::GetInstance()->PushKey(DIK_DOWN))
	{
		cameraPos_.x += -Vector3::normalize(Camera::nowCamera->forward_).x;
		cameraPos_.z += -Vector3::normalize(Camera::nowCamera->forward_).z;
	}
	if (Input::GetInstance()->PushKey(DIK_RIGHT))
	{
		cameraPos_ += Camera::nowCamera->rightDirection;
	}
	if (Input::GetInstance()->PushKey(DIK_LEFT))
	{
		cameraPos_ += -Camera::nowCamera->rightDirection;
	}

	if (Input::GetInstance()->PushKey(DIK_I))
	{
		cameraPos_ += {0, 1, 0};
	}
	if (Input::GetInstance()->PushKey(DIK_K))
	{
		cameraPos_ += {0, -1, 0};
	}

	cameobj_.pos_ = cameraPos_;
	cameobj_.IsUseCameraMouse_ = IsUseCameraMouse_;

	if (Input::GetInstance()->TriggerKey(DIK_B))
	{
		IsUseCameraMouse_ = !IsUseCameraMouse_;
	}

	

#pragma region check

	if (imguiCheckWindow_)
	{
		//2つ目の引数にbool型のポインタがあるとウインドウにバツが出てくる
		if (!ImGui::Begin("check", &imguiCheckWindow_))
		{
			ImGui::End();
			return;
		}
		//上のやつのタイトルバーを右クリックしたときのメニュー一覧
		if (ImGui::BeginPopupContextItem())
		{
			//ウインドウ消す
			if (ImGui::MenuItem("Close Console"))
				*p_open = false;
			ImGui::EndPopup();
		}

		ImGui::Text("%0.0fFPS", ImGui::GetIO().Framerate);

		ImGui::Text("eventEnd:%d", eventManager_->GetInstance()->GetPEventPoint()->GetIsFinished());
		ImGui::Text("eventAllEnd:%d", eventManager_->GetInstance()->GetEventAllEnd());
		ImGui::Text("eventNum:%d", eventManager_->GetInstance()->GetEventNum());
		ImGui::Text("eventcount:%d", eventManager_->GetInstance()->GetEventCount());
		ImGui::Checkbox("useMouseCamera(B)", &IsUseCameraMouse_);
		ImGui::DragFloat3("dir", test_, 1.0f, -100, 100);
		ImGui::ColorEdit3("light", test2_);
		ImGui::DragFloat("AngleOfView", &player_.playCamera_.nowCamera->AngleOfView, 1.0f, 1.0f, 200.0f);
		if (testImguiFlag_)
		{
			ImGui::Text("useMove");
		}

		ImGui::End();
	}

	LightManager::GetInstance()->lightGroups_[0].SetDirLightDir(0, { test_[0],test_[1],test_[2],0});
	LightManager::GetInstance()->lightGroups_[0].SetDirLightColor(0, { test2_[0],test2_[1],test2_[2]});

#pragma endregion

	if (imguiCameraWindow_)
	{
		//2つ目の引数にbool型のポインタがあるとウインドウにバツが出てくる
		if (!ImGui::Begin("camera", &imguiCameraWindow_))
		{
			ImGui::End();
			return;
		}
		//上のやつのタイトルバーを右クリックしたときのメニュー一覧
		if (ImGui::BeginPopupContextItem())
		{
			//ウインドウ消す
			if (ImGui::MenuItem("Close Console"))
				*p_open = false;
			ImGui::EndPopup();
		}

		ImGui::DragFloat("cameraX", &cameraPos_.x, 1.0f, -1000.0f, 1000.0f);
		ImGui::DragFloat("cameraY", &cameraPos_.y, 1.0f, -1000.0f, 1000.0f);
		ImGui::DragFloat("cameraZ", &cameraPos_.z, 1.0f, -1000.0f, 1000.0f);


		ImGui::Text("reset");

		if (ImGui::Button("posX"))
		{
			cameraPos_.x = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("posY"))
		{
			cameraPos_.y = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("posZ"))
		{
			cameraPos_.z = -200;
		}


		ImGui::Text("eye:%0.2f,%0.2f,%0.2f", cameobj_.GetCamera().eye_.x, cameobj_.GetCamera().eye_.y, cameobj_.GetCamera().eye_.z);
		ImGui::Text("target:%0.2f,%0.2f,%0.2f", cameobj_.GetCamera().target_.x, cameobj_.GetCamera().target_.y, cameobj_.GetCamera().target_.z);
		ImGui::Text("up:%0.2f,%0.2f,%0.2f", cameobj_.GetCamera().up_.x, cameobj_.GetCamera().up_.y, cameobj_.GetCamera().up_.z);

		ImGui::Text("forward:%0.2f,%0.2f,%0.2f", cameobj_.GetCamera().forward_.x, cameobj_.GetCamera().forward_.y, cameobj_.GetCamera().forward_.z);
		ImGui::Text("rightDirection:%0.2f,%0.2f,%0.2f", cameobj_.GetCamera().rightDirection.x, cameobj_.GetCamera().rightDirection.y, cameobj_.GetCamera().rightDirection.z);

		ImGui::End();
	}

#ifdef _DEBUG

	ImGui::ShowDemoWindow();

	//imguiの機能を追加するためのフラグ
	ImGuiWindowFlags window_flags = 0;
	//メニューバーを使います
	window_flags |= ImGuiWindowFlags_MenuBar;
	//beginで渡すことでフラグをこのウインドウで有効にする
	ImGui::Begin("Editor", NULL, window_flags);

	
	//エディタの機能をまとめたメニュー
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File\n"))
		{
			ImGui::MenuItem("EventSave", NULL, &imguiEventSaveWindow_);
			ImGui::MenuItem("EventOverwriteSave", NULL, &imguiEventOverwriteSaveWindow_);
			ImGui::MenuItem("EventLoad", NULL, &imguiEventLoadWindow_);
			ImGui::MenuItem("ChangeMap", NULL, &imguiChangeMapWindow_);
			ImGui::EndMenu();			
		}
		if (ImGui::BeginMenu("addEvent\n"))
		{
			imguiAddEventWindow_ = true;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("window\n"))
		{
			ImGui::MenuItem("check", NULL, &imguiCheckWindow_);
			ImGui::MenuItem("camera", NULL, &imguiCameraWindow_);
			ImGui::MenuItem("testWindow", NULL, &testflag);
			ImGui::EndMenu();
		}
		
		ImGui::Text("%0.0fFPS", ImGui::GetIO().Framerate);
		ImGui::EndMenuBar();
	}

	if (testflag)
	{
		//2つ目の引数にbool型のポインタがあるとウインドウにバツが出てくる
		if (!ImGui::Begin("testWindow", &testflag))
		{
			ImGui::End();
			return;
		}
		//上のやつのタイトルバーを右クリックしたときのメニュー一覧
		if (ImGui::BeginPopupContextItem())
		{
			//ウインドウ消す
			if (ImGui::MenuItem("Close Console"))
				*p_open = false;
			ImGui::EndPopup();
		}
		ImGui::Text("%0.0fFPS", ImGui::GetIO().Framerate);

		ImGui::End();
	}

	if (imguiEventSaveWindow_)
	{
		WindowsSaveEEFMFile();
		imguiEventSaveWindow_ = false;
	}
	if (imguiEventOverwriteSaveWindow_)
	{
		SaveAsEEFMFile();
	}
	if (imguiEventLoadWindow_)
	{
		isLoad_ = WindowsOpenEEFMFile();
		imguiEventLoadWindow_ = false;
	}
	if (imguiChangeMapWindow_)
	{
		ChangeMap();
		imguiChangeMapWindow_ = false;
	}

	if (!isLoad_)
	{
		ImGui::Text("Loading failed");
		ImGui::Text(std::string("reason " + loadErrorText_).c_str());
	}

	if (saveFileName_ != "")
	{
		ImGui::Text(std::string("fileName:" + DirectXpre::Util::SeparateFilePath(saveFileName_)).c_str());
	}

	if (!isTest_ && seting_.size() != 0)
	{
		if (ImGui::Button("testStart"))
		{
			Camera::nowCamera = player_.playerCamera_.GetCameraP();
			eventManager_->setEventSeting(seting_);
			isTest_ = !isTest_;
		}

	}

	if (eventManager_->GetEventAllEnd())
	{
		ImGui::Text("EventAllEnd!!!!");
	}

	ImGui::End();

	ImGui::Begin("imguizmo");
	ImGui::Checkbox("useFlag", &testimguizmoFlag);
	ImGui::End();

	if (testimguizmoFlag)
	{
		//EditTransform(&testObj_);
	}

	//EditTransform(&testObj2_);


	if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_C))
	{
		saveSeting_ = seting_;
		saveEnemyDatas_ = enemyDatas_;
		saveMovePointDatas_ = movePointDatas_;
		saveExplosionObjDatas_ = explosionObjDatas_;
	}

	if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_Z))
	{
		seting_ = saveSeting_;
		enemyDatas_ = saveEnemyDatas_;
		movePointDatas_ = saveMovePointDatas_;
		explosionObjDatas_ = saveExplosionObjDatas_;
	}

#endif

	
}

void EventEditorScene::TestDebugUpdate()
{
	ImGui::Begin("check");

	ImGui::Text("%0.0fFPS", ImGui::GetIO().Framerate);

	ImGui::DragFloat("AngleOfView", &player_.playCamera_.nowCamera->AngleOfView, 1.0f, 1.0f, 200.0f);

	ImGui::Checkbox("enemyShotStop", &enemys_->isDebugShotStop_);

	ImGui::End();
}

void EventEditorScene::SaveEventFullPathData(const std::string& fileName)
{
	std::string name = fileName;

	if (fileName == "")
	{
		name = std::string("jsonEventdata");
	}

	nlohmann::json jsonfile;

	jsonfile["name"] = "event";

	for (auto eventSeting : seting_)
	{
		nlohmann::json data;
		if (eventSeting.eventType == EventType::moveEvent)
		{
			data["seting"]["movePoint"] = { eventSeting.movePoint.x,eventSeting.movePoint.y,eventSeting.movePoint.z };
			data["seting"]["movePointRot"] = { eventSeting.movePointRot.x,eventSeting.movePointRot.y,eventSeting.movePointRot.z };
			data["seting"]["moveStartPoint"] = { eventSeting.moveStartPoint.x,eventSeting.moveStartPoint.y,eventSeting.moveStartPoint.z };
			data["seting"]["moveSpeed"] = eventSeting.moveSpeed;
			data["seting"]["moveRotTime"] = eventSeting.moveRotTime;
			data["type"] = "moveEvent";
			data["seting"]["addTime"] = eventSeting.addTimer;
		}
		else if (eventSeting.eventType == EventType::BattleEvent)
		{
			data["seting"]["enemyMaxSpawn"] = eventSeting.enemyMaxSpawn;
			data["seting"]["enemyNum"] = eventSeting.enemyNum;
			for (uint16_t i = 0; i < eventSeting.enemyNum; i++)
			{
				data["seting"]["spawnPoint"] += { eventSeting.enemySpawnPos[i].x, eventSeting.enemySpawnPos[i].y, eventSeting.enemySpawnPos[i].z};

				data["seting"]["enemyMovePos"] += { eventSeting.enemyMovePos[i].x, eventSeting.enemyMovePos[i].y, eventSeting.enemyMovePos[i].z };
			}
			if (eventSeting.enemyNum == 0)
			{
				data["seting"]["spawnPoint"] = {};

				data["seting"]["enemyMovePos"] = {};
			}
			data["seting"]["spawnInterval"] = eventSeting.enemySpawnInterval;
			data["seting"]["enemyType"] = eventSeting.enemyTypes;
			data["seting"]["enemySpeed"] = eventSeting.enemyMoveSpeed;
			data["seting"]["enemyBulletCT"] = eventSeting.enemyBulletCT;
			data["type"] = "BattleEvent";
			data["seting"]["playerHideType"] = eventSeting.playerHideVector;
			data["seting"]["playerPos"] = { eventSeting.playerPos.x,eventSeting.playerPos.y,eventSeting.playerPos.z };
			data["seting"]["addTime"] = eventSeting.addTimer;

			data["seting"]["explosionObjNum"] = eventSeting.explosionObjNum;
			for (uint16_t i = 0; i < eventSeting.explosionObjNum; i++)
			{
				data["seting"]["explosionObjPos"] += { eventSeting.explosionObjPos[i].x, eventSeting.explosionObjPos[i].y, eventSeting.explosionObjPos[i].z };
				data["seting"]["explosionObjSize"] += { eventSeting.explosionObjSize[i].x, eventSeting.explosionObjSize[i].y, eventSeting.explosionObjSize[i].z };
				data["seting"]["explosionObjExplosionSize"] += { eventSeting.explosionObjExplosionSize[i].x, eventSeting.explosionObjExplosionSize[i].y, eventSeting.explosionObjExplosionSize[i].z };
			}
			if (eventSeting.explosionObjNum == 0)
			{
				data["seting"]["explosionObjPos"] = {};
				data["seting"]["explosionObjSize"] = {};
				data["seting"]["explosionObjExplosionSize"] = {};
			}
			data["seting"]["explosionObjExplosionTime"] = eventSeting.explosionObjExplosionTime;
		}
		jsonfile["events"] += { data };
	}

	//インデントを入れるらしい
	std::ofstream ofs(name);
	if (ofs) {
		ofs << jsonfile.dump(4);
	}
}

bool EventEditorScene::LoadFullPathEventData(const std::string& fileName)
{

	seting_.clear();

	//ファイルストリーム
	std::ifstream file(fileName);

	if (!file)
	{
		loadErrorText_ = "can not find file";
		return false;
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいイベントファイルかチェック
	if (!deserialized.is_object() || !deserialized.contains("name") || !deserialized["name"].is_string())
	{
		loadErrorText_ = "Not the correct event file";
		return false;
	}

	//"name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();

	//正しいかどうかチェック
	if (name.compare("event") != 0)
	{
		loadErrorText_ = "Not event file";
		return false;
	}

	//"events"の全オブジェクトを走査
	for (nlohmann::json& events : deserialized["events"])
	{

		bool result = true;

		result = EventScanning(events);

		if (!result)
		{
			seting_.clear();
			return false;
		}
	}


	if (seting_.empty())
	{
		seting_.push_back(EventSeting());
	}

	return true;

}

bool EventEditorScene::EventScanning(const nlohmann::json& Event)
{


	//typeがなければ止める
	if (!Event.contains("type"))
	{
		loadErrorText_ = "event type is missing";
		return false;
	}
	//タイプを取得
	std::string type = Event["type"].get<std::string>();



	//moveEventなら
	if (type.compare("moveEvent") == 0)
	{



		EventSeting eventData;

		eventData.eventType = EventType::moveEvent;

		//設定のパラメータ読み込み
		const nlohmann::json& seting = Event["seting"];

		//ちゃんとパラメータがあるかチェック
		if (!seting.contains("movePoint"))
		{
			loadErrorText_ = "movePoint is missing";
			return false;
		}
		if (!seting.contains("moveStartPoint"))
		{
			loadErrorText_ = "moveStartPoint is missing";
			return false;
		}
		if (!seting.contains("movePointRot"))
		{
			loadErrorText_ = "movePointRot is missing";
			return false;
		}
		if (!seting.contains("moveSpeed"))
		{
			loadErrorText_ = "moveSpeed is missing";
			return false;
		}
		if (!seting.contains("moveRotTime"))
		{
			loadErrorText_ = "moveRotTime is missing";
			return false;
		}
		if (!seting.contains("addTime"))
		{
			loadErrorText_ = "addTime is missing";
			return false;
		}

		//移動する場所読み込み
		eventData.movePoint.x = (float)seting["movePoint"][0];
		eventData.movePoint.y = (float)seting["movePoint"][1];
		eventData.movePoint.z = (float)seting["movePoint"][2];

		//移動開始位置取得
		eventData.moveStartPoint.x = (float)seting["moveStartPoint"][0];
		eventData.moveStartPoint.y = (float)seting["moveStartPoint"][1];
		eventData.moveStartPoint.z = (float)seting["moveStartPoint"][2];

		//移動するときの角度読み込み
		eventData.movePointRot.x = (float)seting["movePointRot"][0];
		eventData.movePointRot.y = (float)seting["movePointRot"][1];
		eventData.movePointRot.z = (float)seting["movePointRot"][2];

		//スピードのセット
		eventData.moveSpeed = (float)seting["moveSpeed"];
		eventData.moveRotTime = (float)seting["moveRotTime"];

		//増やす時間のセット
		eventData.addTimer = (float)seting["addTime"];

		seting_.push_back(eventData);

		EventFlagBuff addflag;
		addflag.eventType = EventType::moveEvent;

		eventFlags_.push_back(addflag);


	}
	else if (type.compare("BattleEvent") == 0)
	{

		EventSeting eventData;

		//設定のパラメータ読み込み
		const nlohmann::json& seting = Event["seting"];

		//ちゃんとパラメータがあるかチェック
		if (!seting.contains("enemyMaxSpawn"))
		{
			loadErrorText_ = "enemyMaxSpawn is missing";
			return false;
		}
		if (!seting.contains("enemyNum"))
		{
			loadErrorText_ = "enemyNum is missing";
			return false;
		}
		if (!seting.contains("playerHideType"))
		{
			loadErrorText_ = "playerHideType is missing";
			return false;
		}
		if (!seting.contains("spawnPoint"))
		{
			loadErrorText_ = "EnemySpawnPoint is missing";
			return false;
		}
		if (!seting.contains("spawnInterval"))
		{
			loadErrorText_ = "EnemySpawnInterval is missing";
			return false;
		}
		if (!seting.contains("enemyType"))
		{
			loadErrorText_ = "enemyType is missing";
			return false;
		}
		if (!seting.contains("enemySpeed"))
		{
			loadErrorText_ = "enemySpeed is missing";
			return false;
		}
		if (!seting.contains("enemyMovePos"))
		{
			loadErrorText_ = "enemyMovePos is missing";
			return false;
		}
		if (!seting.contains("enemyBulletCT"))
		{
			loadErrorText_ = "enemyBulletCT is missing";
			return false;
		}

		if (!seting.contains("playerPos"))
		{
			loadErrorText_ = "playerPos is missing";
			return false;
		}
		if (!seting.contains("addTime"))
		{
			loadErrorText_ = "addTime is missing";
			return false;
		}

		if (!seting.contains("explosionObjNum"))
		{
			loadErrorText_ = "explosionObjNum is missing";
			return false;
		}

		if (!seting.contains("explosionObjPos"))
		{
			loadErrorText_ = "explosionObjPos is missing";
			return false;
		}

		if (!seting.contains("explosionObjSize"))
		{
			loadErrorText_ = "explosionObjSize is missing";
			return false;
		}

		if (!seting.contains("explosionObjExplosionSize"))
		{
			loadErrorText_ = "explosionObjExplosionSize is missing";
			return false;
		}

		if (!seting.contains("explosionObjExplosionTime"))
		{
			loadErrorText_ = "explosionObjExplosionTime is missing";
			return false;
		}

		//沸き数と画面内の最大数をセット
		eventData.enemyMaxSpawn = seting["enemyMaxSpawn"];
		eventData.enemyNum = seting["enemyNum"];

		//イベントのタイプをセット
		eventData.eventType = BattleEvent;
		eventData.playerHideVector = seting["playerHideType"];
		//プレイヤーの位置をセット
		eventData.playerPos.x = (float)seting["playerPos"][0];
		eventData.playerPos.y = (float)seting["playerPos"][1];
		eventData.playerPos.z = (float)seting["playerPos"][2];

		//増やす時間のセット
		eventData.addTimer = (float)seting["addTime"];

		//エネミーの数だけ回す
		for (uint16_t i = 0; i < (uint16_t)seting["enemyNum"]; i++)
		{
			//設定されてないやつをみようとしたらそもそもよばないように
			if ((float)seting["spawnPoint"].size() <= i ||
				(float)seting["spawnInterval"].size() <= i ||
				(float)seting["enemyType"].size() <= i ||
				(float)seting["enemySpeed"].size() <= i ||
				(float)seting["enemyMovePos"].size() <= i) continue;


			//湧く場所をセット
			eventData.enemySpawnPos.push_back({ (float)seting["spawnPoint"][i][0],(float)seting["spawnPoint"][i][1] ,(float)seting["spawnPoint"][i][2] });

			//湧く間隔をセット
			eventData.enemySpawnInterval.push_back((float)seting["spawnInterval"][i]);

			//エネミーの種類をセット
			eventData.enemyTypes.push_back(seting["enemyType"][i].get<std::string>());

			//エネミーが動く場合動くときの速度をセット
			eventData.enemyMoveSpeed.push_back((float)seting["enemySpeed"][i]);

			eventData.enemyBulletCT.push_back((uint32_t)seting["enemyBulletCT"][i]);

			//エネミーが動く場合の動く位置
			eventData.enemyMovePos.push_back({ (float)seting["enemyMovePos"][i][0],(float)seting["enemyMovePos"][i][1] ,(float)seting["enemyMovePos"][i][2] });

		}

		eventData.explosionObjNum = seting["explosionObjNum"];

		//爆発するオブジェクト読み込み
		for (uint16_t j = 0; j < (uint16_t)seting["explosionObjNum"]; j++)
		{		
			eventData.explosionObjPos.push_back({ (float)seting["explosionObjPos"][j][0],(float)seting["explosionObjPos"][j][1] ,(float)seting["explosionObjPos"][j][2] });
			eventData.explosionObjSize.push_back({ (float)seting["explosionObjSize"][j][0], (float)seting["explosionObjSize"][j][1], (float)seting["explosionObjSize"][j][2] });
			eventData.explosionObjExplosionSize.push_back({ (float)seting["explosionObjExplosionSize"][j][0], (float)seting["explosionObjExplosionSize"][j][1], (float)seting["explosionObjExplosionSize"][j][2] });
			eventData.explosionObjExplosionTime.push_back((float)seting["explosionObjExplosionTime"][j]);
		}

		EventFlagBuff addflag;
		addflag.eventType = EventType::BattleEvent;

		addflag.isEnemySpawnPoss.resize(eventData.enemyNum);
		addflag.isEnemyMoveEndPoint.resize(eventData.enemyNum);

		addflag.isBattleEventEnemySpawnPos.resize(eventData.enemyNum);
		addflag.isBattleEventEnemyMovePos.resize(eventData.enemyNum);
		addflag.isBattleEventEnemySpawnInterval.resize(eventData.enemyNum);
		addflag.isBattleEventEnemyMoveSpeed.resize(eventData.enemyNum);
		addflag.isBattleEventEnemyBulletCT.resize(eventData.enemyNum);

		addflag.isExplosionObjPoints.resize(eventData.explosionObjNum);

		addflag.isExplosionObjPos.resize(eventData.explosionObjNum);
		addflag.isExplosionObjSize.resize(eventData.explosionObjNum);
		addflag.isExplosionObjExplosionSize.resize(eventData.explosionObjNum);
		addflag.isExplosionObjExplosionTime.resize(eventData.explosionObjNum);

		eventFlags_.push_back(addflag);

		seting_.push_back(eventData);

		

	}
	else
	{
		loadErrorText_ = "event type is missing";
		return false;
	}


	return true;

}

void EventEditorScene::WindowsSaveEEFMFile()
{
	wchar_t filePath[MAX_PATH] = { 0 };
	OPENFILENAME a = {};
	//構造体の大きさ基本的にこれ
	a.lStructSize = sizeof(OPENFILENAME);
	//使いたい(占有)ウインドウハンドル
	a.hwndOwner = WinApp::GetInstance()->getHwnd();
	//フィルターを設定?
	a.lpstrFilter = L"イベントエディタ作成ファイル(eefm)\0 * .eefm*\0\0";
	//拡張子決定
	a.lpstrDefExt = L"eefm";
	//何個目のフィルターを使うん?みたいな感じ?
	a.nFilterIndex = 0;
	//保存の時ファイル名を入れるやつ?
	a.lpstrFile = filePath;
	//ファイルのバッファの大きさ？
	a.nMaxFile = MAX_PATH;
	//ファイルを保存するときにどういう動きをするか
	a.Flags = OFN_OVERWRITEPROMPT;

	auto old = std::filesystem::current_path();
	if (GetSaveFileName(&a))
	{
		std::string test = DirectXpre::Util::WStringToString(filePath);
		saveFileName_ = test;
		SaveEventFullPathData(test);
	}
	std::filesystem::current_path(old);
}

void EventEditorScene::SaveAsEEFMFile()
{
	if (saveFileName_ == "")
	{
		return;
	}

	if (imguiEventOverwriteSaveWindow_)
	{
		//2つ目の引数にbool型のポインタがあるとウインドウにバツが出てくる
		if (!ImGui::Begin("check", &imguiEventOverwriteSaveWindow_))
		{
			ImGui::End();
			return;
		}
		

		ImGui::Text(std::string("fileName:" + DirectXpre::Util::SeparateFilePath(saveFileName_)+"に上書きしますか?").c_str());

		if (ImGui::Button("yes", { 100,100 }))
		{
			SaveEventFullPathData(saveFileName_);
			imguiEventOverwriteSaveWindow_ = false;
		}
		ImGui::SameLine();

		if (ImGui::Button("no", { 100,100 }))
		{
			imguiEventOverwriteSaveWindow_ = false;
		}
		
		ImGui::End();
	}
}

bool EventEditorScene::WindowsOpenEEFMFile()
{
	wchar_t filePath[MAX_PATH] = { 0 };
	OPENFILENAME a = {};
	//構造体の大きさ基本的にこれ
	a.lStructSize = sizeof(OPENFILENAME);
	//使いたい(占有)ウインドウハンドル
	a.hwndOwner = WinApp::GetInstance()->getHwnd();
	//フィルターを設定?
	a.lpstrFilter = L"イベントエディタ作成ファイル(eefm)\0 * .eefm*\0"
		L"すべてのファイル (*.*)\0*.*\0";
	//何個目のフィルターを使うん?みたいな感じ?
	a.nFilterIndex = 0;
	//保存の時ファイル名を入れるやつ?
	a.lpstrFile = filePath;
	//ファイルのバッファの大きさ？
	a.nMaxFile = MAX_PATH;

	auto old = std::filesystem::current_path();
	if (GetOpenFileName(&a))
	{
		bool result = true;
		std::string test = DirectXpre::Util::WStringToString(filePath);
		//設定のまとめに選択したファイルを読み取り書き込む
		result = LoadFullPathEventData(test);

		if (!result)
		{
			return false;
		}
		saveFileName_ = test;

		//読み込んだデータにあるエディタに描画するオブジェクトの登録
		AddEventDebugObj();

	}
	std::filesystem::current_path(old);

	return true;
}

void EventEditorScene::AddEventDebugObj()
{
	if (seting_.size() != 0)
	{
		movePointDatas_.clear();
		enemyDatas_.clear();
		explosionObjDatas_.clear();

		AddMoveEventDebugObj();

		AddBattleEventDebugObj();

		AddBattleEventExplosionObjDebugObj();
		
	}
}

void EventEditorScene::AddMoveEventDebugObj()
{
	for (auto eventData = seting_.begin(); eventData != seting_.end(); eventData++)
	{
		if (eventData->eventType == EventType::moveEvent)
		{
			//描画用のやつにコピー
			EventMovePointData addMoveEventData;

			addMoveEventData.startPoint.FBXInit();
			addMoveEventData.endPoint.FBXInit();
			addMoveEventData.move.FBXInit();

			addMoveEventData.startPoint.pos_ = { eventData->moveStartPoint };
			addMoveEventData.endPoint.pos_ = { eventData->movePoint };
			addMoveEventData.move.pos_ = { eventData->moveStartPoint };

			addMoveEventData.startPoint.Scale_ = { 0.3f,0.3f,0.3f };
			addMoveEventData.endPoint.Scale_ = { 0.3f,0.3f,0.3f };
			addMoveEventData.move.Scale_ = { 0.3f,0.3f,0.3f };

			addMoveEventData.startPoint.Update();
			addMoveEventData.endPoint.Update();
			addMoveEventData.move.Update();

			//各ポイントの色を変更

			addMoveEventData.startPoint.SetColor({ 0.0f,0.0f ,0.5f ,1.0f });
			addMoveEventData.endPoint.SetColor({ 0.5f,0.0f ,0.0f ,1.0f });
			addMoveEventData.move.SetColor({ 0.5f,0.0f ,0.5f ,1.0f });

			movePointDatas_.push_back(addMoveEventData);
		}
		
	}
}

void EventEditorScene::AddBattleEventDebugObj()
{
	for (auto eventData = seting_.begin(); eventData != seting_.end(); eventData++)
	{
		if (eventData->eventType == EventType::BattleEvent)
		{
			EventEnemyData add;

			Object3D playerObj;
			playerObj.FBXInit();
			playerObj.pos_ = eventData->playerPos;
			playerObj.Scale_ = { 0.3f,0.3f,0.3f };
			add.playerPoint = playerObj;

			for (int32_t i = 0; i < eventData->enemyNum; i++)
			{
				Object3D enemyObj;
				enemyObj.FBXInit();
				enemyObj.pos_ = eventData->enemySpawnPos[i];
				enemyObj.Scale_ = { 0.5f,0.5f ,0.5f };

				add.enemys.push_back(enemyObj);

				add.enemyTypes.push_back(eventData->enemyTypes[i]);

				Object3D endPointObj;
				endPointObj.FBXInit();
				endPointObj.pos_ = eventData->enemyMovePos[i];
				endPointObj.Scale_ = { 0.5f,0.5f ,0.5f };
				endPointObj.SetColor({ 0.5f,0.0f ,0.0f ,1.0f });
				add.endPoint.push_back(endPointObj);

				Object3D moveObj;
				moveObj.FBXInit();
				moveObj.pos_ = eventData->enemySpawnPos[i];
				moveObj.Scale_ = { 0.5f,0.5f ,0.5f };
				moveObj.SetColor({ 0.5f,0.0f ,0.5f ,1.0f });
				add.move.push_back(moveObj);
			}

			enemyDatas_.push_back(add);
		}
	}
}

void EventEditorScene::AddBattleEventExplosionObjDebugObj()
{
	for (auto eventData = seting_.begin(); eventData != seting_.end(); eventData++)
	{
		if (eventData->eventType == EventType::BattleEvent)
		{
			EventExplosionObjData add;

			for (int32_t i = 0; i < eventData->explosionObjNum; i++)
			{
				Object3D explosionObj;
				explosionObj.FBXInit();
				explosionObj.pos_ = eventData->explosionObjPos[i];
				explosionObj.Scale_ = eventData->explosionObjSize[i];

				add.obj.push_back(explosionObj);

				Object3D explosionObj2;
				explosionObj2.FBXInit();
				explosionObj2.pos_ = eventData->explosionObjPos[i];
				explosionObj2.Scale_ = eventData->explosionObjSize[i];
				add.explosion.push_back(explosionObj2);

				add.endSize.push_back(eventData->explosionObjExplosionSize[i]);

			}

			explosionObjDatas_.push_back(add);
		}
	}
}

void EventEditorScene::AddBattleEventDebugObjRandomAccess(uint16_t num)
{
	uint16_t eventBattleCount = 0;

	for (auto eventData = seting_.begin(); eventData != seting_.end(); eventData++)
	{
		if (eventData->eventType == EventType::BattleEvent)
		{

			if (eventBattleCount != num)
			{
				eventBattleCount++;
				continue;
			}
			EventEnemyData add;

			Object3D playerObj;
			playerObj.FBXInit();
			playerObj.pos_ = eventData->playerPos;
			playerObj.Scale_ = { 0.3f,0.3f,0.3f };
			add.playerPoint = playerObj;

			for (int32_t i = 0; i < eventData->enemyNum; i++)
			{
				Object3D enemyObj;
				enemyObj.FBXInit();
				enemyObj.pos_ = eventData->enemySpawnPos[i];
				enemyObj.Scale_ = { 0.5f,0.5f ,0.5f };

				add.enemys.push_back(enemyObj);

				add.enemyTypes.push_back(eventData->enemyTypes[i]);

				Object3D endPointObj;
				endPointObj.FBXInit();
				endPointObj.pos_ = eventData->enemyMovePos[i];
				endPointObj.Scale_ = { 0.5f,0.5f ,0.5f };
				endPointObj.SetColor({ 0.5f,0.0f ,0.0f ,1.0f });
				add.endPoint.push_back(endPointObj);

				Object3D moveObj;
				moveObj.FBXInit();
				moveObj.pos_ = eventData->enemySpawnPos[i];
				moveObj.Scale_ = { 0.5f,0.5f ,0.5f };
				moveObj.SetColor({ 0.5f,0.0f ,0.5f ,1.0f });
				add.move.push_back(moveObj);
			}

			enemyDatas_[num] = add;
			break;
		}
	}
}

void EventEditorScene::AddBattleEventExplosionObjDebugObjRandomAccess(uint16_t num)
{
	uint16_t eventBattleCount = 0;

	for (auto eventData = seting_.begin(); eventData != seting_.end(); eventData++)
	{
		
		if (eventData->eventType == EventType::BattleEvent)
		{
			//処理順的に1多いので
			if (eventBattleCount != num-1)
			{
				eventBattleCount++;
				continue;
			}
			EventExplosionObjData add;

			for (int32_t i = 0; i < eventData->explosionObjNum; i++)
			{
				Object3D explosionObj;
				explosionObj.FBXInit();
				explosionObj.pos_ = eventData->explosionObjPos[i];
				explosionObj.Scale_ = eventData->explosionObjSize[i];

				add.obj.push_back(explosionObj);

				Object3D explosionObj2;
				explosionObj2.FBXInit();
				explosionObj2.pos_ = eventData->explosionObjPos[i];
				explosionObj2.Scale_ = eventData->explosionObjSize[i];
				add.explosion.push_back(explosionObj2);

				add.endSize.push_back(eventData->explosionObjExplosionSize[i]);

			}

			explosionObjDatas_[num-1] = add;
			break;
			
		}
	}
}

void EventEditorScene::EventImguizmoMoveStartPointFlag(const uint32_t& count)
{
	//指定したやつを使うように
	eventFlags_[count].isMoveEndPoint = true;
	for (uint16_t i = 0; i < eventFlags_.size(); i++)
	{
		//同じイベント種類の干渉するフラグを消す
		eventFlags_[i].isMoveStratPoint = false;
		if (i == count)continue;
		//編集中以外は使わないようにする
		eventFlags_[i].isMoveEndPoint = false;
	}
}

void EventEditorScene::EventImguizmoMovePointFlag(const uint32_t& count)
{
	//指定したやつを使うように
	eventFlags_[count].isMoveStratPoint = true;
	for (uint16_t i = 0; i < eventFlags_.size(); i++)
	{
		//同じイベント種類の干渉するフラグを消す
		eventFlags_[i].isMoveEndPoint = false;
		if (i == count)continue;
		//編集中以外は使わないようにする
		eventFlags_[i].isMoveStratPoint = false;
	}
}

void EventEditorScene::EventImguizmoBattleEventPlayerPoint(const uint32_t& count)
{
	//指定したやつを使うように
	eventFlags_[count].isBattlePlayerPoint = true;
	for (uint16_t i = 0; i < eventFlags_.size(); i++)
	{
		//同じイベント種類の干渉するフラグを消す
		for (uint32_t n = 0; n < eventFlags_[i].isEnemyMoveEndPoint.size(); n++)
		{
			eventFlags_[i].isEnemyMoveEndPoint[n] = false;
			eventFlags_[i].isEnemySpawnPoss[n] = false;
		}

		for (uint16_t m = 0; m < eventFlags_[i].isExplosionObjPoints.size(); m++)
		{
			//編集中以外は使わないようにする
			eventFlags_[i].isExplosionObjPoints[m] = false;
		}
		if (i == count)continue;
		//編集中以外は使わないようにする
		eventFlags_[i].isBattlePlayerPoint = false;
	}
}

void EventEditorScene::EventImguizmoExplosionObjPosFlag(const uint32_t& count, const uint32_t& ExplosionObjCount)
{
	//指定したやつを使うように
	eventFlags_[count].isExplosionObjPoints[ExplosionObjCount] = true;
	for (uint16_t i = 0; i < eventFlags_.size(); i++)
	{
		//同じイベント種類の干渉するフラグを消す
		for (uint32_t n = 0; n < eventFlags_[i].isEnemyMoveEndPoint.size(); n++)
		{
			eventFlags_[i].isEnemyMoveEndPoint[n] = false;
			eventFlags_[i].isEnemySpawnPoss[n] = false;
		}

		for (uint16_t j = 0; j < eventFlags_[i].isExplosionObjPoints.size(); j++)
		{
			if (i == count && j == ExplosionObjCount)continue;
			//編集中以外は使わないようにする
			eventFlags_[i].isExplosionObjPoints[j] = false;
		}

		eventFlags_[i].isBattlePlayerPoint = false;
	}
}

void EventEditorScene::EventImguizmoEnemySpawnPosFlag(const uint32_t& count, const uint32_t& enemyCount)
{
	//指定したやつを使うように
	eventFlags_[count].isEnemySpawnPoss[enemyCount] = true;
	for (uint16_t i = 0; i < eventFlags_.size(); i++)
	{
		//同じイベント種類の干渉するフラグを消す
		for (uint32_t n = 0; n < eventFlags_[i].isEnemyMoveEndPoint.size(); n++)
		{
			eventFlags_[i].isEnemyMoveEndPoint[n] = false;
		}

		for (uint16_t m = 0; m < eventFlags_[i].isExplosionObjPoints.size(); m++)
		{
			//編集中以外は使わないようにする
			eventFlags_[i].isExplosionObjPoints[m] = false;
		}

		for (uint16_t j = 0; j < eventFlags_[i].isEnemySpawnPoss.size(); j++)
		{
			if (i == count && j == enemyCount)continue;
			//編集中以外は使わないようにする
			eventFlags_[i].isEnemySpawnPoss[j] = false;
		}

		eventFlags_[i].isBattlePlayerPoint = false;
	}
}

void EventEditorScene::EventImguizmoEnemyMoveEndPointFlag(const uint32_t& count, const uint32_t& enemyCount)
{
	//指定したやつを使うように
	eventFlags_[count].isEnemyMoveEndPoint[enemyCount] = true;
	for (uint16_t i = 0; i < eventFlags_.size(); i++)
	{
		//同じイベント種類の干渉するフラグを消す
		for (uint32_t n = 0; n < eventFlags_[i].isEnemySpawnPoss.size(); n++)
		{
			eventFlags_[i].isEnemySpawnPoss[n] = false;
		}

		for (uint16_t m = 0; m < eventFlags_[i].isExplosionObjPoints.size(); m++)
		{
			//編集中以外は使わないようにする
			eventFlags_[i].isExplosionObjPoints[m] = false;
		}

		for (uint16_t j = 0; j < eventFlags_[i].isEnemyMoveEndPoint.size(); j++)
		{
			if (i == count && j == enemyCount)continue;
			//編集中以外は使わないようにする
			eventFlags_[i].isEnemyMoveEndPoint[j] = false;
		}

		eventFlags_[i].isBattlePlayerPoint = false;
	}
}

void EditTransform(Vector3& pos)
{
	//操作用の行列を用意
	Matrix4x4 nowPosMat;
	nowPosMat.IdentityMatrix();
	nowPosMat.m[3][0] = pos.x;
	nowPosMat.m[3][1] = pos.y;
	nowPosMat.m[3][2] = pos.z;

	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

	float matm16[16];
	ChengeMatrix(nowPosMat, matm16);
	
	float camePm16[16];
	float cameVm16[16];
	Matrix4x4 mat;
	ChengeMatrix(Camera::nowCamera->matProjection_, camePm16);
	ChengeMatrix(Camera::nowCamera->matView_, cameVm16);
	ImGuizmo::SetRect(0, 0, WinApp::GetInstance()->getWindowSizeWidthF(), WinApp::GetInstance()->getWindowSizeHeightF());
	ImGuizmo::Manipulate(cameVm16, camePm16, mCurrentGizmoOperation, mCurrentGizmoMode, matm16, NULL);
	mat = ChengeTwoDimensionalMatrix(matm16);

	pos = { mat.m[3][0],mat.m[3][1],mat.m[3][2]};
}