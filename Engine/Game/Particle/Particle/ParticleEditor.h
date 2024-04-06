#pragma once
#include "WinApp.h"
#include <filesystem>
#include <fstream>
#include "Util.h"
#include <json.hpp>
#include "Vector3.h"
#include "BasicObjEmitter.h"

class ParticleEditor
{
public:
	ParticleEditor();
	~ParticleEditor();

	void Init();

	void Update();

	void Draw();

	/// <summary>
	/// フルパスでのファイルの書き込み
	/// </summary>
	/// <param name="fileName">ファイルへのフルパス</param>
	void SaveParticleFullPathData(const std::string& fileName);

	//windowsの機能を使ってeefmファイルを保存
	void WindowsSavePcleFile();

private:

	//各パラメータ
	Vector3 pos;
	std::string particleName;
	float liveTime = 10;
	float actionTime = 10;
	float ActiveTime = -1;
	float ct = 1;
	Vector2 randRengeX = { -1,1 };
	Vector2 randRengeY = { -1,1 };
	Vector2 randRengeZ = { -1,1 };
	Vector3 startScale = { 1,1,1 };
	Vector3 endScale = { 1,1,1 };

	bool isDraw = false;

	//テスト描画用
	BasicObjEmitter testDraw;

	bool imguiParticleSaveWindow_ = false;

};

