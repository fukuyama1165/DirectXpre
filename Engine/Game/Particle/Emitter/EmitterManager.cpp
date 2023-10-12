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

void EmitterManager::AddObjEmitter(const Vector3& pos, std::string emitterType, std::string particleType, const float& liveTime, const float& actionTime, const float& ActiveTime, const Vector2& randRengeX, const Vector2& randRengeY, const Vector2& randRengeZ, std::string particleModel, std::string emitterModel)
{
	std::unique_ptr<IObjEmitter> newEmitter = std::move(emitterFactory_->CreateObjEmitter(emitterType));
	newEmitter->Initialize(pos, particleType, liveTime, ActiveTime, actionTime, randRengeX, randRengeY, randRengeZ, particleModel, emitterModel);

	objEmitters_.push_back(std::move(newEmitter));

#ifdef _DEBUG
	pos_.push_back(pos);

	particleLiveTime_.push_back(liveTime);
	activeTime_.push_back(ActiveTime);
	maxCT_.push_back(1.0f);
	particleTypeNum_.push_back(0);
	particleActionTime_.push_back(actionTime);

	particleType_.push_back(particleType);

	randRangeX_.push_back(randRengeX);
	randRangeY_.push_back(randRengeY);
	randRangeZ_.push_back(randRengeZ);
#endif

}

void EmitterManager::Update()
{

#ifdef _DEBUG

	uint32_t count = 0;
	for (std::unique_ptr<IObjEmitter>& emitter : objEmitters_)
	{
		if (emitter.get()->GetIsEnd())
		{
			for (auto posI = pos_.begin(); posI != pos_.end();)
			{
				posI += count;
				posI = pos_.erase(posI);
				break;
			}
			for (auto CTI = maxCT_.begin(); CTI != maxCT_.end();)
			{
				CTI += count;
				CTI = maxCT_.erase(CTI);
				break;
			}
			for (auto particleLiveTimeI = particleLiveTime_.begin(); particleLiveTimeI != particleLiveTime_.end();)
			{
				particleLiveTimeI += count;
				particleLiveTimeI = particleLiveTime_.erase(particleLiveTimeI);
				break;
			}
			for (auto activeTimeI = activeTime_.begin(); activeTimeI != activeTime_.end();)
			{
				activeTimeI += count;
				activeTimeI = activeTime_.erase(activeTimeI);
				break;
			}
			for (auto particleActionTimeI = particleActionTime_.begin(); particleActionTimeI != particleActionTime_.end();)
			{
				particleActionTimeI += count;
				particleActionTimeI = particleActionTime_.erase(particleActionTimeI);
				break;
			}
			for (auto particleTypeI = particleType_.begin(); particleTypeI != particleType_.end();)
			{
				particleTypeI += count;
				particleTypeI = particleType_.erase(particleTypeI);
				break;
			}
			for (auto particleTypeNumI = particleTypeNum_.begin(); particleTypeNumI != particleTypeNum_.end();)
			{
				particleTypeNumI += count;
				particleTypeNumI = particleTypeNum_.erase(particleTypeNumI);
				break;
			}
			for (auto randRangeXI = randRangeX_.begin(); randRangeXI != randRangeX_.end();)
			{
				randRangeXI += count;
				randRangeXI = randRangeX_.erase(randRangeXI);
				break;
			}
			for (auto randRangeYI = randRangeY_.begin(); randRangeYI != randRangeY_.end();)
			{
				randRangeYI += count;
				randRangeYI = randRangeY_.erase(randRangeYI);
				break;
			}
			for (auto randRangeZI = randRangeZ_.begin(); randRangeZI != randRangeZ_.end();)
			{
				randRangeZI += count;
				randRangeZI = randRangeZ_.erase(randRangeZI);
				break;
			}
			
		}
		count++;
	}

#endif

	for (std::unique_ptr<IObjEmitter>& emitter : objEmitters_)
	{
		emitter->Update();
	}

	objEmitters_.remove_if([](std::unique_ptr<IObjEmitter>& emitter)
	{
		return emitter->GetIsEnd();
	});

#ifdef _DEBUG


#pragma region effect

	ImGui::Begin("effect");

	ImGui::DragFloat3("pos", particlePos);

	ImGui::DragFloat2("RandX", effectTestRandX, 0.1f);
	ImGui::DragFloat2("RandY", effectTestRandY, 0.1f);
	ImGui::DragFloat2("RandZ", effectTestRandZ, 0.1f);

	ImGui::DragFloat("liveTime", &effectTestliveTime, 0.1f);
	ImGui::DragFloat("actionTime", &effectTestactionTime, 0.1f);



	const char* emittrChar[] = { "BASIC" };

	//intしか使えん許さん
	ImGui::Combo("emitterType", (int*)&emittrTypeNum, emittrChar, IM_ARRAYSIZE(emittrChar));



	const char* particleChar[] = { "BASIC","Cartridge","Fall" };

	//intしか使えん許さん
	ImGui::Combo("particleType", (int*)&particleTypeNum, particleChar, IM_ARRAYSIZE(particleChar));

	std::string emitterType;
	std::string particleType;

	switch (emittrTypeNum)
	{
	case 1:
		emitterType = "BASIC";
		break;
	default:
		emitterType = "BASIC";
		break;
	}

	switch (particleTypeNum)
	{
	case 1:
		particleType = "BASIC";
		break;
	case 2:
		particleType = "Cartridge";
		break;
	case 3:
		particleType = "Fall";
		break;

	default:
		particleType = "BASIC";
		break;
	}

	if (ImGui::Button("emittrpop"))
	{
		EmitterManager::GetInstance()->AddObjEmitter({ particlePos[0],particlePos[1],particlePos[2] }, emitterType, particleType, effectTestliveTime, effectTestactionTime, -1, { effectTestRandX[0],effectTestRandX[1] }, { effectTestRandY[0],effectTestRandY[1] }, { effectTestRandZ[0],effectTestRandZ[1] });
	}

	if (ImGui::Button("alldel"))
	{
		EmitterManager::GetInstance()->reset();
	}

	ImGui::End();

#pragma endregion

#pragma region effect

	ImGui::Begin("effectList");

	uint32_t effectListCount = 0;

	for (auto i = objEmitters_.begin(); i != objEmitters_.end(); i++)
	{
		float posbuff[3] = { pos_[effectListCount].x,pos_[effectListCount].y ,pos_[effectListCount].z };
		ImGui::DragFloat3("pos", posbuff, 0.1f);

		ImGui::DragFloat("particleLiveTime", &particleLiveTime_[effectListCount], 0.1f);
		ImGui::DragFloat("activeTime", &activeTime_[effectListCount], 0.1f);
		ImGui::DragFloat("CT", &maxCT_[effectListCount], 0.1f);
		ImGui::DragFloat("particleActionTime", &particleActionTime_[effectListCount], 0.1f);

		//intしか使えん許さん
		ImGui::Combo("particleType", (int*)&particleTypeNum_[effectListCount], particleChar, IM_ARRAYSIZE(particleChar));

		switch (particleTypeNum_[effectListCount])
		{
		case 1:
			particleType_[effectListCount] = "BASIC";
			break;
		case 2:
			particleType_[effectListCount] = "Cartridge";
			break;
		case 3:
			particleType_[effectListCount] = "Fall";
			break;

		default:
			particleType_[effectListCount] = "BASIC";
			break;
		}

		float randRangeXbuff[2] = { randRangeX_[effectListCount].x,randRangeX_[effectListCount].y};
		float randRangeYbuff[2] = { randRangeY_[effectListCount].x,randRangeY_[effectListCount].y};
		float randRangeZbuff[2] = { randRangeZ_[effectListCount].x,randRangeZ_[effectListCount].y};
		ImGui::DragFloat2("randRangeX", randRangeXbuff, 0.1f);
		ImGui::DragFloat2("randRangeY", randRangeYbuff, 0.1f);
		ImGui::DragFloat2("randRangeZ", randRangeZbuff, 0.1f);


		pos_[effectListCount] = { posbuff[0],posbuff[1] ,posbuff[2] };

		randRangeX_[effectListCount] = { randRangeXbuff[0],randRangeXbuff[1] };
		randRangeY_[effectListCount] = { randRangeYbuff[0],randRangeYbuff[1] };
		randRangeZ_[effectListCount] = { randRangeZbuff[0],randRangeZbuff[1] };

		i->get()->SetPos(pos_[effectListCount]);
		i->get()->SetActiveTime(activeTime_[effectListCount]);
		i->get()->SetParticleLiveTime(particleLiveTime_[effectListCount]);
		i->get()->SetCT(maxCT_[effectListCount]);
		i->get()->SetParticleType(particleType_[effectListCount]);
		i->get()->SetRandRangeX(randRangeX_[effectListCount]);
		i->get()->SetRandRangeY(randRangeY_[effectListCount]);
		i->get()->SetRandRangeZ(randRangeZ_[effectListCount]);
		i->get()->SetParticleactionTime(particleActionTime_[effectListCount]);

		effectListCount++;
	
	}

	ImGui::End();

#pragma endregion

#endif


}

void EmitterManager::Draw()
{
	for (std::unique_ptr<IObjEmitter>& emitter : objEmitters_)
	{
		emitter->Draw();
	}
}

void EmitterManager::reset() 
{ 

	objEmitters_.clear(); 
	emitters_.clear();
	pos_.clear();

	particleLiveTime_.clear();
	activeTime_.clear();
	maxCT_.clear();
	particleTypeNum_.clear();

	particleType_.clear();

	randRangeX_.clear();
	randRangeY_.clear();
	randRangeZ_.clear();

}