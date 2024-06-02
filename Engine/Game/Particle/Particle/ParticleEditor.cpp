#include "ParticleEditor.h"
#include <imgui.h>


ParticleEditor::ParticleEditor()
{
}

ParticleEditor::~ParticleEditor()
{
}

void ParticleEditor::Init()
{
	testDraw.Initialize(pos, "BASIC", liveTime, actionTime, ActiveTime, ct, randRengeX, randRengeY, randRengeZ, startScale, endScale);
}

void ParticleEditor::Update()
{
	//imguiの機能を追加するためのフラグ
	ImGuiWindowFlags window_flags = 0;
	//メニューバーを使います
	window_flags |= ImGuiWindowFlags_MenuBar;

	ImGui::Begin("ParticleEdit",NULL, window_flags);

	//エディタの機能をまとめたメニュー
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File\n"))
		{
			ImGui::MenuItem("ParticleSave", NULL, &imguiParticleSaveWindow_);
			
			ImGui::EndMenu();
		}
	
		ImGui::EndMenuBar();
	}

	if (imguiParticleSaveWindow_)
	{
		WindowsSavePcleFile();
		imguiParticleSaveWindow_ = false;
	}

	bool testIsActive = testDraw.GetIsActive();
	ImGui::Checkbox("Draw", &isDraw);
	ImGui::Checkbox("isActive", &testIsActive);
	testDraw.SetIsActive(testIsActive);

	if (testDraw.GetIsEnd())
	{
		testDraw.SetIsEnd(false);
	}

	float buffPos[3] = { pos.x,pos.y,pos.z };
	ImGui::DragFloat3("pos", buffPos, 1, -1000.0f, 1000.0f);

	float buffLiveTime = liveTime;
	ImGui::DragFloat("liveTime", &buffLiveTime, 1, 0.0f, 1000.0f);

	float buffActionTime = actionTime;
	ImGui::DragFloat("actionTime", &buffActionTime, 1, 0.0f, 1000.0f);

	float buffActiveTime = ActiveTime;
	ImGui::DragFloat("ActiveTime", &buffActiveTime, 1, -1.0f, 1000.0f);

	float buffCt = ct;
	ImGui::DragFloat("ct", &buffCt, 1, 0.1f, 1000.0f);

	float buffRandRengeX[2] = { randRengeX.x,randRengeX.y};
	ImGui::DragFloat2("randRengeX", buffRandRengeX, 1, -1000.0f, 1000.0f);

	float buffRandRengeY[2] = { randRengeY.x,randRengeY.y };
	ImGui::DragFloat2("randRengeY", buffRandRengeY, 1, -1000.0f, 1000.0f);

	float buffRandRengeZ[2] = { randRengeZ.x,randRengeZ.y };
	ImGui::DragFloat2("randRengeZ", buffRandRengeZ, 1, -1000.0f, 1000.0f);
	
	float buffStartScale[3] = { startScale.x,startScale.y,startScale.z };
	ImGui::DragFloat3("startScale", buffStartScale, 1, -1000.0f, 1000.0f);

	float buffEndScale[3] = { endScale.x,endScale.y,endScale.z };
	ImGui::DragFloat3("endScale", buffEndScale, 1, -1000.0f, 1000.0f);

	pos = { buffPos[0],buffPos[1],buffPos[2] };
	liveTime = buffLiveTime;
	actionTime = buffActionTime;
	ActiveTime = buffActiveTime;
	ct = buffCt;
	randRengeX = { buffRandRengeX[0],buffRandRengeX[1]};
	randRengeY = { buffRandRengeY[0],buffRandRengeY[1]};
	randRengeZ = { buffRandRengeZ[0],buffRandRengeZ[1]};
	startScale = { buffStartScale[0],buffStartScale[1],buffStartScale[2] };
	endScale = { buffEndScale[0],buffEndScale[1],buffEndScale[2] };
	
	ImGui::End();

	testDraw.SetPos(pos);
	testDraw.SetParticleLiveTime(liveTime);
	testDraw.SetParticleactionTime(actionTime);
	testDraw.SetActiveTime(buffActiveTime);
	testDraw.SetCT(ct);
	testDraw.SetRandRangeX(randRengeX);
	testDraw.SetRandRangeY(randRengeY);
	testDraw.SetRandRangeZ(randRengeZ);
	testDraw.SetStartScale(startScale);
	testDraw.SetEndScale(endScale);

	testDraw.Update();
}

void ParticleEditor::Draw()
{
	if (isDraw)
	{
		testDraw.Draw();
	}
}

void ParticleEditor::SaveParticleFullPathData(const std::string& fileName)
{
	std::string name = fileName;

	if (fileName == "")
	{
		name = std::string("ParticleData");
	}

	nlohmann::json jsonfile;

	jsonfile["name"] = "Particle";

	nlohmann::json data;

	data["Name"] = particleName;
	data["liveTime"] = liveTime;
	data["actionTime"] = actionTime;
	data["ActiveTime"] = ActiveTime;
	data["ct"] = ct;
	data["randRengeX"] = { randRengeX.x,randRengeX.y };
	data["randRengeY"] = { randRengeY.x,randRengeY.y };
	data["randRengeZ"] = { randRengeZ.x,randRengeZ.y };
	data["startScale"] = { startScale.x,startScale.y,startScale.z };
	data["endScale"] = { endScale.x,endScale.y,endScale.z };
		
	jsonfile["ParticleParameters"] = { data };

	//インデントを入れるらしい
	std::ofstream ofs(name);
	if (ofs) {
		ofs << jsonfile.dump(4);
	}
}

void ParticleEditor::WindowsSavePcleFile()
{
	wchar_t filePath[MAX_PATH] = { 0 };
	OPENFILENAME fileObj = {};
	//構造体の大きさ基本的にこれ
	fileObj.lStructSize = sizeof(OPENFILENAME);
	//使いたい(占有)ウインドウハンドル
	fileObj.hwndOwner = WinApp::GetInstance()->getHwnd();
	//フィルターを設定?
	fileObj.lpstrFilter = L"パーティクルの設定(Pcle)\0 * .Pcle*\0\0";
	//拡張子決定
	fileObj.lpstrDefExt = L"Pcle";
	//何個目のフィルターを使うん?みたいな感じ?
	fileObj.nFilterIndex = 0;
	//保存の時ファイル名を入れるやつ?
	fileObj.lpstrFile = filePath;
	//ファイルのバッファの大きさ？
	fileObj.nMaxFile = MAX_PATH;
	//ファイルを保存するときにどういう動きをするか
	fileObj.Flags = OFN_OVERWRITEPROMPT;

	auto old = std::filesystem::current_path();
	if (GetSaveFileName(&fileObj))
	{
		std::string test = DirectXpre::Util::WStringToString(filePath);
		SaveParticleFullPathData(test);
	}
	std::filesystem::current_path(old);
}