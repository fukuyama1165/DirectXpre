#pragma once
#include "Object3D.h"
#include "Wall.h"
#include "JsonLevelLoader.h"


class LevelLoader
{
public:

	static LevelLoader* GetInstance();

	//レベル読み込み
	void LoadLevel(std::string filename);

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();

	struct LevelObj
	{
		Object3D obj;
		std::string name;
	};

	struct LevelWallObj
	{
		WallObj obj;
		std::string name;
	};

	std::vector<LevelObj> levelObj;
	std::vector<std::unique_ptr<LevelWallObj>> wallObj_;

	//キーを指定してレベルを読みこむ
	void reloadLevel(const BYTE& CheckKey, std::string filename);
private:

	AnimationModel* levelModel_ = nullptr;
	AnimationModel* levelBallModel_ = nullptr;
	AnimationModel* levelGroundModel_ = nullptr;
	AnimationModel* levelBuildingModel_ = nullptr;
	AnimationModel* levelWallModel_ = nullptr;

private:

	LevelLoader() = default;
	~LevelLoader();

	LevelLoader(const LevelLoader&) = delete;
	LevelLoader& operator=(const LevelLoader&) = delete;


};

