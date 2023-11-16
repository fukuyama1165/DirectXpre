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

	cameobj_.upDate();


	objobj3_.Update();

	LevelLoader::GetInstance()->Update();

	//デバック用の処理
	DebugUpdate();

	//イベント管理部分
	AddEvent();

	EditEvent();

	DrawEventDataUpdate();

	ImGui::Begin("save");

	ImGui::InputTextWithHint("fileName", "seveFileName", str1, IM_ARRAYSIZE(str1));

	if (ImGui::Button("save"))
	{
		SaveEventData(std::string(str1));
	}

	if (ImGui::Button("test"))
	{
		wchar_t filePath[MAX_PATH] = {0};
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

	ImGui::End();
	

	eventManager_->Update();

	EmitterManager::GetInstance()->Update();

}

void EventEditorScene::Draw()
{

	LevelLoader::GetInstance()->Draw();

	objobj3_.Draw();

	
	enemys_->Draw();

	if (!isTest_)
	{

		for (auto enemyobj : enemyDatas_)
		{
			for (auto enemys : enemyobj.enemys)
			{
				enemys.FBXDraw(*enemyModel_);
			}
		}

		for (auto movePointobj : movePointDatas_)
		{
			movePointobj.startPoint.FBXDraw(*moveEventModel_);
			movePointobj.endPoint.FBXDraw(*moveEventModel_);
			movePointobj.move.FBXDraw(*moveEventModel_);
		}
	}

	eventManager_->Draw();

	EmitterManager::GetInstance()->Draw();

}

void EventEditorScene::AddEvent()
{
	//イベント追加部分
	ImGui::Begin("addEvent");

	

	//intしか使えん許さん
	ImGui::Combo("emitterType", (int*)&eventTypeNum_, EventTypeChar, IM_ARRAYSIZE(EventTypeChar));

	//前フレームの敵の数
	int32_t oldEnemyNum_ = 0;


	switch (eventTypeNum_)
	{
	case 0:

		ImGui::DragFloat3("movePoint", movePoint_, 1.0f, -1000.0f, 1000.0f);
		ImGui::DragFloat3("movePointRot", movePointRot_, 1.0f, -1000.0f, 1000.0f);
		ImGui::DragFloat3("moveStartPoint", moveStartPoint_, 1.0f, -1000.0f, 1000.0f);
		ImGui::DragFloat("moveSpeed", &moveSpeed_, 1.0f, 0.0f, 1000.0f);

		if (ImGui::Button("addEvent"))
		{
			EventSeting addEvent;
			addEvent.eventType = EventType::moveEvent;
			addEvent.movePoint = { movePoint_[0] ,movePoint_[1] ,movePoint_[2] };
			addEvent.movePointRot = { movePointRot_[0] ,movePointRot_[1] ,movePointRot_[2] };
			addEvent.moveStartPoint = { moveStartPoint_[0] ,moveStartPoint_[1] ,moveStartPoint_[2] };
			addEvent.moveSpeed = moveSpeed_;

			seting_.push_back(addEvent);

			//描画用のやつにコピー
			EventMovePointData addMoveEventData;

			addMoveEventData.startPoint.FBXInit();
			addMoveEventData.endPoint.FBXInit();
			addMoveEventData.move.FBXInit();

			addMoveEventData.startPoint.Trans_ = { moveStartPoint_[0] ,moveStartPoint_[1] ,moveStartPoint_[2] };
			addMoveEventData.endPoint.Trans_ = { movePoint_[0] ,movePoint_[1] ,movePoint_[2] };
			addMoveEventData.move.Trans_ = { moveStartPoint_[0] ,moveStartPoint_[1] ,moveStartPoint_[2] };

			addMoveEventData.startPoint.Update();
			addMoveEventData.endPoint.Update();
			addMoveEventData.move.Update();

			//各ポイントの色を変更

			addMoveEventData.startPoint.SetColor({ 0.0f,0.0f ,0.5f ,1.0f });
			addMoveEventData.endPoint.SetColor({ 0.5f,0.0f ,0.0f ,1.0f });
			addMoveEventData.move.SetColor({ 0.5f,0.0f ,0.5f ,1.0f });

			movePointDatas_.push_back(addMoveEventData);
		}

		break;

	case 1:


		//前フレームの情報を保存
		oldEnemyNum_ = enemyNum_;
		//intしかないのでintに変換
		ImGui::DragInt("enemyNum", (int*)&enemyNum_, 1, 0, 10);

		//画面の最大湧き数を設定
		ImGui::DragInt("maxSpawn", (int*)&enemyMaxSpawn_, 1, 1, 10);

		//前フレームから変更されていたら大きさを変更
		if (oldEnemyNum_ != enemyNum_)
		{
			enemyTypeNum_.resize(enemyNum_);
			enemySpawnPos_.resize(enemyNum_);
			enemyMovePos_.resize(enemyNum_);
			enemyMoveSpeed_.resize(enemyNum_);
			enemySpawnInterval_.resize(enemyNum_);
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
			addEvent.playerHideVector = playerHideType_;

			seting_.push_back(addEvent);

			EventEnemyData add;

			for (uint16_t i = 0; i < enemyNum_; i++)
			{
				Object3D enemyObj;
				enemyObj.FBXInit();
				enemyObj.Trans_ = enemySpawnPos_[i];

				add.enemys.push_back(enemyObj);
			}

			enemyDatas_.push_back(add);

			//次の設定用に中身を削除
			enemyTypeNum_.clear();
			enemySpawnPos_.clear();
			enemyMovePos_.clear();
			enemyMoveSpeed_.clear();
			enemySpawnInterval_.clear();
			enemyTypes_.clear();
			enemyNum_ = 0;
			enemyMaxSpawn_ = 1;
		}


		break;
	default:
		break;
	}


		//敵の数分回す
		for (uint16_t i = 0; i < enemyNum_; i++)
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



			switch (enemyTypeNum_[i])
			{
			case 0:



				ImGui::Text("enemytype:moveOnly");
				enemyTypes_[i] = "moveOnly";

				ImGui::DragFloat3(std::string("spawnPos" + num).c_str(), spawnPos, 1.0f, -1000.0f, 1000.0f);

				ImGui::DragFloat3(std::string("movePos" + num).c_str(), movePos, 1.0f, -1000.0f, 1000.0f);

				ImGui::DragFloat(std::string("spawnInterval" + num).c_str(), &enemySpawnInterval, 1.0f, 0.0f, 50.0f);

				ImGui::DragFloat(std::string("moveSpeed" + num).c_str(), &enemyMoveSpeed, 0.1f, 0.0f, 10.0f);

				break;
			case 1:

				ImGui::Text("enemytype:moveAttack");
				enemyTypes_[i] = "moveAttack";

				ImGui::DragFloat3(std::string("spawnPos" + num).c_str(), spawnPos, 1.0f, -1000.0f, 1000.0f);

				ImGui::DragFloat3(std::string("movePos" + num).c_str(), movePos, 1.0f, -1000.0f, 1000.0f);

				ImGui::DragFloat(std::string("spawnInterval" + num).c_str(), &enemySpawnInterval, 1.0f, 0.0f, 50.0f);

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

				enemyMoveSpeed = 0;

				break;
			default:

				break;
			}

			enemySpawnPos_[i] = { spawnPos[0],spawnPos[1] ,spawnPos[2] };
			enemyMovePos_[i] = { movePos[0],movePos[1] ,movePos[2] };
			enemySpawnInterval_[i] = enemySpawnInterval;
			enemyMoveSpeed_[i] = enemyMoveSpeed;


		}

		

	ImGui::End();
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

			ImGui::DragFloat3(std::string("movePoint" + num).c_str(), movePoint, 1.0f, -1000.0f, 1000.0f);
			ImGui::DragFloat3(std::string("movePointRot" + num).c_str(), movePointRot, 1.0f, -1000.0f, 1000.0f);
			ImGui::DragFloat3(std::string("moveStartPoint" + num).c_str(), moveStartPoint, 1.0f, -1000.0f, 1000.0f);
			ImGui::DragFloat(std::string("moveSpeed" + num).c_str(), &moveSpeed, 1.0f, 0.0f, 1000.0f);

			setingI->movePoint = { movePoint[0] ,movePoint[1] ,movePoint[2] };
			setingI->movePointRot = { movePointRot[0] ,movePointRot[1] ,movePointRot[2] };
			setingI->moveStartPoint = { moveStartPoint[0] ,moveStartPoint[1] ,moveStartPoint[2] };
			setingI->moveSpeed = moveSpeed;


		}
		else if (setingI->eventType == EventType::BattleEvent)
		{
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



				switch (enemyTypeNum)
				{
				case 0:



					ImGui::Text("enemytype:moveOnly");
					setingI->enemyTypes[i] = "moveOnly";

					ImGui::DragFloat3(std::string("spawnPos" + enemyNum).c_str(), spawnPos, 1.0f, -1000.0f, 1000.0f);

					ImGui::DragFloat3(std::string("movePos" + enemyNum).c_str(), movePos, 1.0f, -1000.0f, 1000.0f);

					ImGui::DragFloat(std::string("spawnInterval" + enemyNum).c_str(), &enemySpawnInterval, 1.0f, 0.0f, 50.0f);

					ImGui::DragFloat(std::string("moveSpeed" + enemyNum).c_str(), &enemyMoveSpeed, 0.1f, 0.0f, 10.0f);

					break;
				case 1:

					ImGui::Text("enemytype:moveAttack");
					setingI->enemyTypes[i] = "moveAttack";

					ImGui::DragFloat3(std::string("spawnPos" + enemyNum).c_str(), spawnPos, 1.0f, -1000.0f, 1000.0f);

					ImGui::DragFloat3(std::string("movePos" + enemyNum).c_str(), movePos, 1.0f, -1000.0f, 1000.0f);

					ImGui::DragFloat(std::string("spawnInterval" + enemyNum).c_str(), &enemySpawnInterval, 1.0f, 0.0f, 50.0f);

					ImGui::DragFloat(std::string("moveSpeed" + enemyNum).c_str(), &enemyMoveSpeed, 0.1f, 0.0f, 10.0f);

					break;
				case 2:

					ImGui::Text("enemytype:Attack");
					setingI->enemyTypes[i] = "Attack";

					ImGui::DragFloat3(std::string("spawnPos" + enemyNum).c_str(), spawnPos, 1.0f, -1000.0f, 1000.0f);

					movePos[0] = { 0 };
					movePos[1] = { 0 };
					movePos[2] = { 0 };

					ImGui::DragFloat(std::string("spawnInterval" + enemyNum).c_str(), &enemySpawnInterval, 1.0f, 0.0f, 50.0f);

					enemyMoveSpeed = 0;

					break;
				default:

					break;
				}

				setingI->enemySpawnPos[i] = { spawnPos[0],spawnPos[1] ,spawnPos[2] };
				setingI->enemyMovePos[i] = { movePos[0],movePos[1] ,movePos[2] };
				setingI->enemySpawnInterval[i] = enemySpawnInterval;
				setingI->enemyMoveSpeed[i] = enemyMoveSpeed;



			}

			float playerHideType = setingI->playerHideVector;

			//intしか使えん許さん
			ImGui::Combo("playerHideType", (int*)&playerHideTypeNum_, playerHideTypeChar, IM_ARRAYSIZE(playerHideTypeChar));

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

			setingI->playerHideVector = playerHideType;
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

void EventEditorScene::DrawEventDataUpdate()
{
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

	auto moveobj = movePointDatas_.begin();

	for (auto setingI = seting_.begin(); setingI != seting_.end();)
	{
		if (setingI->eventType != EventType::moveEvent)
		{
			setingI++;
			continue;
		}
		
		moveobj->startPoint.Trans_ = setingI->moveStartPoint;
		moveobj->startPoint.Update();
		moveobj->endPoint.Trans_ = setingI->movePoint;
		moveobj->endPoint.Update();
		
		setingI++;
		moveobj++;
	}

	auto enemyobj = enemyDatas_.begin();

	for (auto setingI = seting_.begin(); setingI != seting_.end();)
	{
		if (setingI->eventType != EventType::BattleEvent)
		{
			setingI++;
			continue;
		}

		for (uint32_t i = 0; i < enemyobj->enemys.size(); i++)
		{
			enemyobj->enemys[i].Trans_ = setingI->enemySpawnPos[i];
			enemyobj->enemys[i].Update();
		}

		setingI++;
		enemyobj++;
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

			movePointobj.move.Trans_ = lerp(movePointobj.startPoint.GetWorldPos(), movePointobj.endPoint.GetWorldPos(), moveEventMoveTimer / moveEventMoveMaxTime);
			movePointobj.startPoint.Update();
			movePointobj.endPoint.Update();
			movePointobj.move.Update();

			setingI++;
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
			data["type"] = "moveEvent";
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
			data["type"] = "BattleEvent";
			data["playerHideType"] = eventSeting.playerHideVector;
		}
		jsonfile["events"] += { data };
	}
	

	std::ofstream ofs("Resources/json/" + name + ".json");
	if (ofs) {
		ofs << jsonfile.dump(4);
	}

}

void EventEditorScene::TestEvent()
{

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
	LevelLoader::GetInstance()->reloadLevel(DIK_L, "MapTest2");

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
			data["type"] = "moveEvent";
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
			data["seting"]["spawnInterval"] = eventSeting.enemySpawnInterval;
			data["seting"]["enemyType"] = eventSeting.enemyTypes;
			data["seting"]["enemySpeed"] = eventSeting.enemyMoveSpeed;
			data["type"] = "BattleEvent";
			data["playerHideType"] = eventSeting.playerHideVector;
		}
		jsonfile["events"] += { data };
	}


	std::ofstream ofs(name);
	if (ofs) {
		ofs << jsonfile.dump(4);
	}
}