#pragma once

/**
 * @file IScene.h
 * @brief シーンの基底クラス
 * @author フクヤマ
 * @date 2023_12/29
 */

class IScene
{
public:
	virtual ~IScene() {};

	//初期化
	virtual void Initialize() {};

	//終了処理
	virtual void Finalize() {};

	//毎フレーム更新
	virtual void Update() {};

	//描画
	virtual void Draw() {};

};

