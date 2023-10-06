#include "EmitterManager.h"
#include "EmitterFactory.h"
#include <imgui.h>

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

void EmitterManager::AddObjEmitter(const Vector3& pos, std::string emitterType, std::string particleType, float ActiveTime, std::string particleModel, std::string emitterModel)
{
	std::unique_ptr<IObjEmitter> newEmitter = std::move(emitterFactory_->CreateObjEmitter(emitterType));
	newEmitter->Initialize(pos, particleType,particleModel,emitterModel, ActiveTime);

	objEmitters_.push_back(std::move(newEmitter));
}

void EmitterManager::Update()
{
	for (std::unique_ptr<IObjEmitter>& emitter : objEmitters_)
	{
		emitter->Update();
	}

	objEmitters_.remove_if([](std::unique_ptr<IObjEmitter>& emitter)
	{
		return emitter->GetIsActive() == false && emitter->GetParticleNum() == 0;
	});


	//ImGui::Begin("check");

	//ImGui::Text("emittersize:%0.0f", (float)objEmitters_.size());

	//ImGui::End();
}

void EmitterManager::Draw()
{
	for (std::unique_ptr<IObjEmitter>& emitter : objEmitters_)
	{
		emitter->Draw();
	}
}