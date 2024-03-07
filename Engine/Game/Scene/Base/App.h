#pragma once

/**
 * @file App.h
 * @brief アプリケーションの処理クラス
 * @author フクヤマ
 * @date 2023_12/29
 */

#include "Framework.h"




class App:public Framework
{
public:

	void Initialize()override;

	void Finalize()override;

	void Update()override;

	void Draw()override;

private:

	//シーンチェンジ用変数
	uint32_t sceneTypeNum_ = 0;

	std::string sceneType_ = "";

private:

	

	
};

