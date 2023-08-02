#include "EmitterManager.h"

EmitterManager::~EmitterManager()
{
}

EmitterManager* EmitterManager::GetInstance()
{
	static EmitterManager instance;

	return &instance;
}

void EmitterManager::AddEmitter(const Vector3& pos, float ActiveTime)
{
	std::unique_ptr<BasicEmitter> newEmitter = std::make_unique<BasicEmitter>();
	newEmitter->Initialize(pos, ActiveTime);

	emitters_.push_back(std::move(newEmitter));
}

void EmitterManager::Update()
{
	for (std::unique_ptr<BasicEmitter>& emitter : emitters_)
	{
		emitter->Update();
	}
}

void EmitterManager::Draw()
{
	for (std::unique_ptr<BasicEmitter>& emitter : emitters_)
	{
		emitter->Draw();
	}
}