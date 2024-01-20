#include "EventEditorScene.h"
#include <imgui.h>
#include "Input.h"
#include "SceneManager.h"
#include "LightManager.h"
#include "Texture.h"
#include "EmitterManager.h"
#include "LevelLoader.h"

EventEditorScene::EventEditorScene()
{
}

EventEditorScene::~EventEditorScene()
{
}

void EventEditorScene::Initialize()
{
	Object3D::SetLight(&LightManager::GetInstance()->lightGroups_[0]);
	//LightManager::GetInstance()->lightGroups_[0].SetAmbientColor({ 0.05f,0.05f,0.05f });

	objobj3_.objDrawInit("Resources/obj/skydome/", "skydome.obj");
	objobj3_.SetScale({ 1000,1000,1000 });

	cameobj_ = cameraObj((float)WinApp::GetInstance()->getWindowSizeWidth(), (float)WinApp::GetInstance()->getWindowSizeHeight());

	cameobj_.pos_ = { 0,0,-50 };

	Camera::nowCamera = cameobj_.GetCameraP();

	//レベル読み込み
	LevelLoader::GetInstance()->LoadLevel("MapTest2");

	enemys_ = EnemyManager::GetInstance();

	testModel_ = ModelManager::GetInstance()->SearchModelData("basketballmanBox");
	enemyModel_ = ModelManager::GetInstance()->SearchModelData("Enemy");
	moveEventModel_ = ModelManager::GetInstance()->SearchModelData("whiteBox");

	eventManager_ = EventPointManager::GetInstance();

	player_.Init();

	eventManager_->isNoTimer = false;

	testExplosionObj.Init({ 0,20,-250 }, 0, { 1,1,1 }, { 10,10,10 }, 50.0f);
	testEnemy1.Init("Attack", { 2,20,-250 }, {}, 0, 900000);
	testEnemy2.Init("Attack", { -2,20,-250 }, {}, 0, 900000);

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

	
	if (!pause_)
	{
		objobj3_.Update();

		LevelLoader::GetInstance()->Update();
	}

	if (!isTest_)
	{
		cameobj_.upDate();
		//デバック用の処理
		DebugUpdate();

		//イベント管理部分
		AddEvent();

		EditEvent();

		DrawEventDataUpdate();

		SaveAndLoadEvent();

		ChangeMap();
	}

	TestEvent();

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
				explosionObj.obj[i].FBXDraw(*moveEventModel_);
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

	if (!testExplosionObj.isExplosion_)
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

}

void EventEditorScene::AddEvent()
{
	//イベント追加部分
	ImGui::Begin("addEvent");

	

	//intしか使えん許さん
	ImGui::Combo("emitterType", (int*)&eventTypeNum_, EventTypeChar, IM_ARRAYSIZE(EventTypeChar));

	


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

		seting_.push_back(addEvent);

		//描画用のやつにコピー
		EventMovePointData addMoveEventData;

		addMoveEventData.startPoint.FBXInit();
		addMoveEventData.endPoint.FBXInit();
		addMoveEventData.move.FBXInit();

		addMoveEventData.startPoint.pos_ = { moveStartPoint_[0] ,moveStartPoint_[1] ,moveStartPoint_[2] };
		addMoveEventData.endPoint.pos_ = { movePoint_[0] ,movePoint_[1] ,movePoint_[2] };
		addMoveEventData.move.pos_ = { moveStartPoint_[0] ,moveStartPoint_[1] ,moveStartPoint_[2] };

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

	seting_.push_back(addEvent);

	AddButtonBattleEventDebugObj();
	AddButtonBattleEventExplosionObjDebugObj();

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
	add.playerPoint = playerObj;

	for (int32_t i = 0; i < enemyNum_; i++)
	{
		Object3D enemyObj;
		enemyObj.FBXInit();
		enemyObj.pos_ = enemySpawnPos_[i];

		add.enemys.push_back(enemyObj);

		add.enemyTypes.push_back(enemyTypes_[i]);

		Object3D endPointObj;
		endPointObj.FBXInit();
		endPointObj.pos_ = enemyMovePos_[i];
		endPointObj.SetColor({ 0.5f,0.0f ,0.0f ,1.0f });
		add.endPoint.push_back(endPointObj);

		Object3D moveObj;
		moveObj.FBXInit();
		moveObj.pos_ = enemySpawnPos_[i];
		moveObj.SetColor({ 0.5f,0.0f ,0.5f ,1.0f });
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

	//登録されているイベントデータ編集したい
	for (auto setingI = seting_.begin(); setingI != seting_.end();)
	{
		std::string num = ("##" + std::to_string(eventCount));
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

			setingI->movePoint = { movePoint[0] ,movePoint[1] ,movePoint[2] };
			setingI->movePointRot = { movePointRot[0] ,movePointRot[1] ,movePointRot[2] };
			setingI->moveStartPoint = { moveStartPoint[0] ,moveStartPoint[1] ,moveStartPoint[2] };
			setingI->moveSpeed = moveSpeed;
			setingI->moveRotTime = moveRotTime;
			setingI->addTimer = addTime;


		}
		else if (setingI->eventType == EventType::BattleEvent)
		{
			float playerPos[3] = { setingI->playerPos.x,setingI->playerPos.y,setingI->playerPos.z };
			ImGui::DragFloat3(std::string("PlayerPos" + num).c_str(), playerPos, 1, -1000.0f, 1000.0f);


			float playerHideType = setingI->playerHideVector;
			float addTime = setingI->addTimer;

			//intしか使えん許さん
			ImGui::Combo("playerHideType", (int*)&playerHideTypeNum_, playerHideTypeChar, IM_ARRAYSIZE(playerHideTypeChar));
			//終了時に増える時間をセット
			ImGui::DragFloat(std::string("AddTime" + num).c_str(), &addTime, 1.0f, 0.0f, 6000.0f);

			switch (playerHideTypeNum_)
			{
			case playerHideVectorType::Down:
				playerHideType = playerHideVectorType::Down;
				break;
			case playerHideVectorType::Right:
				playerHideType = playerHideVectorType::Right;
				break;
			default:
				break;
			}

			for (uint16_t i = 0; i < setingI->enemyNum; i++)
			{
				ImGui::Text("enemyNum:%02d", i);

				//種類識別
				std::string enemyNum = (num + "_" + std::to_string(i));

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
				ImGui::Combo(std::string("EnemyType" + enemyNum).c_str(), (int*)&enemyTypeNum, EnemyTypeChar, IM_ARRAYSIZE(EnemyTypeChar));

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

					ImGui::DragFloat3(std::string("spawnPos" + enemyNum).c_str(), spawnPos, 1.0f, -1000.0f, 1000.0f);

					ImGui::DragFloat3(std::string("movePos" + enemyNum).c_str(), movePos, 1.0f, -1000.0f, 1000.0f);

					ImGui::DragFloat(std::string("spawnInterval" + enemyNum).c_str(), &enemySpawnInterval, 1.0f, 0.0f, 50.0f);

					ImGui::DragFloat(std::string("moveSpeed" + enemyNum).c_str(), &enemyMoveSpeed, 0.1f, 0.0f, 10.0f);

					enemyBulletCT = 0;

					break;
				case 1:

					ImGui::Text("enemytype:moveAttack");
					setingI->enemyTypes[i] = "moveAttack";

					ImGui::DragFloat3(std::string("spawnPos" + enemyNum).c_str(), spawnPos, 1.0f, -1000.0f, 1000.0f);

					ImGui::DragFloat3(std::string("movePos" + enemyNum).c_str(), movePos, 1.0f, -1000.0f, 1000.0f);

					ImGui::DragFloat(std::string("spawnInterval" + enemyNum).c_str(), &enemySpawnInterval, 1.0f, 0.0f, 50.0f);

					ImGui::DragFloat(std::string("moveSpeed" + enemyNum).c_str(), &enemyMoveSpeed, 0.1f, 0.0f, 10.0f);

					ImGui::DragInt(std::string("enemyBulletCT" + enemyNum).c_str(), (int*)&enemyBulletCT, 1, 0, 500);

					break;
				case 2:

					ImGui::Text("enemytype:Attack");
					setingI->enemyTypes[i] = "Attack";

					ImGui::DragFloat3(std::string("spawnPos" + enemyNum).c_str(), spawnPos, 1.0f, -1000.0f, 1000.0f);

					movePos[0] = { 0 };
					movePos[1] = { 0 };
					movePos[2] = { 0 };

					ImGui::DragFloat(std::string("spawnInterval" + enemyNum).c_str(), &enemySpawnInterval, 1.0f, 0.0f, 50.0f);

					ImGui::DragInt(std::string("enemyBulletCT" + enemyNum).c_str(), (int*)&enemyBulletCT, 1, 0, 500);

					enemyMoveSpeed = 0;

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
				break;

			}
			else
			{
				uint16_t battleNum = 0;
				uint16_t moveNum = 0;

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
					

				}

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
			if (setingI->eventType != EventType::BattleEvent || setingI->explosionObjPos.size() == 0)
			{
				setingI++;
				continue;
			}

			for (uint32_t i = 0; i < explosionObj->obj.size(); i++)
			{
				explosionObj->obj[i].pos_ = setingI->explosionObjPos[i];
				explosionObj->explosion[i].pos_ = setingI->explosionObjPos[i];
				explosionObj->explosion[i].Scale_ = lerp(setingI->explosionObjSize[i], setingI->explosionObjExplosionSize[i], moveEventMoveTimer / moveEventMoveMaxTime);
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

void EventEditorScene::SaveEventData(const std::string fileName)
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
	ImGui::Begin("ChangeMap");


	if (ImGui::Button("selectMap"))/*ImGui::SameLine(); HelpMarker("You can input value using the scientific notation,\n""  e.g. \"1e+8\" becomes \"100000000\".");*/
	{
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
			test = Util::WStringToString(filePath);

			test = Util::SeparateFilePath(test);
			
			

		}
		std::filesystem::current_path(old);
		//設定されたマップを読み込みなおす
		LevelLoader::GetInstance()->LoadLevel(test);
	}

	ImGui::End();
}

void EventEditorScene::TestEvent()
{
	ImGui::Begin("test");
	if (!isTest_ && seting_.size()!=0)
	{
		if (ImGui::Button("testStart"))
		{
			Camera::nowCamera = player_.playerCamera_.GetCameraP();
			eventManager_->setEventSeting(seting_);
			isTest_ = !isTest_;
		}
		
	}
	else if(isTest_)
	{
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
	}

	if (eventManager_->GetEventAllEnd())
	{
		ImGui::Text("EventAllEnd!!!!");
	}

	ImGui::End();

	if (isTest_)
	{

		if (!pause_)
		{
			player_.Update();

			enemys_->UpDate(player_.playerCamera_.GetCamera().eye_);

			CollisionManager::GetInstance()->CheckAllCollisions();

			eventManager_->Update();
		}

		

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

	testExplosionObj.Update("", 0);
	testEnemy1.Update("");
	testEnemy2.Update("");
	CollisionManager::GetInstance()->CheckAllCollisions();

	if (Input::GetInstance()->TriggerKey(DIK_L) && !testExplosionObj.isExplosion_)
	{
		testExplosionObj.OnCollision();
	}

#pragma region check

	ImGui::Begin("check");

	ImGui::Text("%0.0fFPS", ImGui::GetIO().Framerate);

	ImGui::Text("eventEnd:%d", eventManager_->GetInstance()->GetPEventPoint()->GetIsFinished());
	ImGui::Text("eventAllEnd:%d", eventManager_->GetInstance()->GetEventAllEnd());
	ImGui::Text("eventNum:%d", eventManager_->GetInstance()->GetEventNum());
	ImGui::Text("eventcount:%d", eventManager_->GetInstance()->GetEventCount());
	ImGui::Checkbox("useMouseCamera(B)", &IsUseCameraMouse_);

	ImGui::End();

#pragma endregion

	ImGui::Begin("camera");


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

#ifdef _DEBUG

	ImGui::ShowDemoWindow();
#endif

	//マップのテスト
	//LevelLoader::GetInstance()->reloadLevel(DIK_L, "MapTest");

}

void EventEditorScene::SaveEventFullPathData(const std::string fileName)
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

bool EventEditorScene::LoadFullPathEventData(std::string fileName)
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

		result = EventScanning(deserialized, events);

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

bool EventEditorScene::EventScanning(nlohmann::json deserialized, nlohmann::json& Event)
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
		nlohmann::json& seting = Event["seting"];

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


	}
	else if (type.compare("BattleEvent") == 0)
	{

		EventSeting eventData;

		//設定のパラメータ読み込み
		nlohmann::json& seting = Event["seting"];

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

		//爆発するオブジェクト読み込み
		for (uint16_t j = 0; j < (uint16_t)seting["explosionObjNum"]; j++)
		{
			ExplosionObjManager::GetInstance()->PopExplosionObj({ (float)seting["explosionObjPos"][j][0],(float)seting["explosionObjPos"][j][1] ,(float)seting["explosionObjPos"][j][2] },
				(int16_t)seting_.size(),
				{ (float)seting["explosionObjSize"][j][0], (float)seting["explosionObjSize"][j][1], (float)seting["explosionObjSize"][j][2] },
				{ (float)seting["explosionObjExplosionSize"][j][0], (float)seting["explosionObjExplosionSize"][j][1], (float)seting["explosionObjExplosionSize"][j][2] },
				(float)seting["explosionObjExplosionTime"][j]);
		}

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
		std::string test = Util::WStringToString(filePath);
		SaveEventFullPathData(test);
	}
	std::filesystem::current_path(old);
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
		std::string test = Util::WStringToString(filePath);
		//設定のまとめに選択したファイルを読み取り書き込む
		result = LoadFullPathEventData(test);

		if (!result)
		{
			return false;
		}

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

			for (int32_t i = 0; i < explosionObjNum_; i++)
			{
				Object3D explosionObj;
				explosionObj.FBXInit();
				explosionObj.pos_ = explosionObjPos_[i];
				explosionObj.Scale_ = explosionObjSize_[i];

				add.obj.push_back(explosionObj);
				add.explosion.push_back(explosionObj);

				add.endSize.push_back(explosionObjExplosionSize_[i]);

			}

			explosionObjDatas_.push_back(add);
		}
	}
}