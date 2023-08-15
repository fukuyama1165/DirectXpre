#include "EmitterManager.h"
#include "EmitterFactory.h"

EmitterManager::~EmitterManager()
{
}

EmitterManager* EmitterManager::GetInstance()
{
	static EmitterManager instance;

	if (instance.emitterFactory_ == nullptr)
	{
		instance.emitterFactory_ = std::make_unique<EmitterFactory>();
	}

	return &instance;
}

void EmitterManager::AddObjEmitter(const Vector3& pos, std::string emitterType, std::string particleType, float ActiveTime)
{
	std::unique_ptr<IObjEmitter> newEmitter = std::move(emitterFactory_->CreateObjEmitter(emitterType));
	newEmitter->Initialize(pos, particleType, ActiveTime);

	objEmitters_.push_back(std::move(newEmitter));
}

void EmitterManager::Update()
{
	for (std::unique_ptr<IObjEmitter>& emitter : objEmitters_)
	{
		emitter->Update();
	}
}

void EmitterManager::Draw()
{
	for (std::unique_ptr<IObjEmitter>& emitter : objEmitters_)
	{
		emitter->Draw();
	}
}