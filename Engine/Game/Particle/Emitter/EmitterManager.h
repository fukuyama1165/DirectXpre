#pragma once
#include "IEmitter.h"
#include "IObjEmitter.h"
#include "IEmitterFactory.h"


class EmitterManager
{
public:
	static EmitterManager* GetInstance();

	void AddObjEmitter(const Vector3& pos, std::string emitterType, std::string particleType, const float& liveTime, const float& ActiveTime = -1, const Vector2& randRengeX = { -100,100 }, const Vector2& randRengeY = { -100,100 }, const Vector2& randRengeZ = { -100,100 }, std::string particleModel = "whiteBox", std::string emitterModel = "whiteBox");

	//中身全部消す
	void reset() { objEmitters_.clear(); emitters_.clear(); };

	//毎フレーム更新
	void Update();

	//描画
	void Draw();

private:

	EmitterManager() = default;
	~EmitterManager();

	EmitterManager(const EmitterManager&) = delete;
	EmitterManager& operator=(const EmitterManager&) = delete;


private:
	std::list<std::unique_ptr<IObjEmitter>> objEmitters_;
	std::list<std::unique_ptr<IEmitter>> emitters_;

	std::unique_ptr<IEmitterFactory> emitterFactory_ = nullptr;

};

