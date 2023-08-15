#pragma once
#include "IEmitter.h"
#include "IObjEmitter.h"
#include "IEmitterFactory.h"


class EmitterManager
{
public:
	static EmitterManager* GetInstance();

	void AddObjEmitter(const Vector3& pos,std::string emitterType,std::string particleType, float ActiveTime = -1);

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

