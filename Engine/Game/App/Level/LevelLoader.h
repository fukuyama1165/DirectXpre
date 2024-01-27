#pragma once

/**
 * @file LevelLoader.h
 * @brief ゲーム内のステージの読み込み描画を行うクラス
 * @author フクヤマ
 * @date 2023_11/10
 */

#include "Object3D.h"
#include "Wall.h"
#include "JsonLevelLoader.h"


class LevelLoader
{
public:

	static LevelLoader* GetInstance();

	//レベル読み込み(前回のがあった場合消去)
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

	//キーを指定してレベルを読み込みなおす
	void reloadLevel(const BYTE& CheckKey, std::string filename);

private:

	AnimationModel* levelModel_ = nullptr;
	AnimationModel* levelBallModel_ = nullptr;
	AnimationModel* levelGroundModel_ = nullptr;
	AnimationModel* levelBuildingModel_ = nullptr;
	AnimationModel* levelWallModel_ = nullptr;
	AnimationModel* levelLightModel_ = nullptr;
	AnimationModel* levelCylinderModel_ = nullptr;
	AnimationModel* levelExplosionObjModel_ = nullptr;
	AnimationModel* levelShipModel_ = nullptr;

	float test_[2] = {1,1};
	float test2_[2] = { 1,1 };
	float test3_[2] = { 1,1 };

	Object3D testObj_;

	//ライトの数(初期値が1なのは要素番号を指定して行うため0番はplayerが使うため)
	int32_t lightCount_ = 1;

private:

	LevelLoader() = default;
	~LevelLoader();

	LevelLoader(const LevelLoader&) = delete;
	LevelLoader& operator=(const LevelLoader&) = delete;


};

