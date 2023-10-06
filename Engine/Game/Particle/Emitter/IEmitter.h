#pragma once
#include "Vector3.h"
#include <string>

class IEmitter
{
public:
	virtual ~IEmitter() = 0;

	//初期化
	virtual void Initialize() = 0;
	virtual void Initialize(const Vector3& pos, std::string particleType, float ActiveTime = -1) = 0;

	//終了処理
	virtual void Finalize() = 0;

	//毎フレーム更新
	virtual void Update() = 0;

	//描画
	virtual void Draw() = 0;
};