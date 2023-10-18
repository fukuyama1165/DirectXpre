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

void EmitterManager::AddObjEmitter(const Vector3& pos, std::string emitterType, std::string particleType, const float& liveTime, const float& actionTime, const float& ActiveTime, float ct, const Vector2& randRengeX, const Vector2& randRengeY, const Vector2& randRengeZ, Vector3 startScale, Vector3 endScale, std::string particleModel, std::string emitterModel)
{
	std::unique_ptr<IObjEmitter> newEmitter = std::move(emitterFactory_->CreateObjEmitter(emitterType));
	newEmitter->Initialize(pos, particleType, liveTime, actionTime, ActiveTime, ct, randRengeX, randRengeY, randRengeZ, startScale, endScale, particleModel, emitterModel);

	objEmitters_.push_back(std::move(newEmitter));

#ifdef _DEBUG
	//デバック用のデータを追加するところ
	pos_.push_back(pos);

	particleLiveTime_.push_back(liveTime);
	activeTime_.push_back(ActiveTime);
	maxCT_.push_back(ct);
	
	particleActionTime_.push_back(actionTime);

	particleType_.push_back(particleType);

	randRangeX_.push_back(randRengeX);
	randRangeY_.push_back(randRengeY);
	randRangeZ_.push_back(randRengeZ);

	if (particleType == "BASIC")
	{
		particleTypeNum_.push_back(0);
	}
	else if (particleType == "Cartridge")
	{
		particleTypeNum_.push_back(1);
	}
	else if (particleType == "Fall")
	{
		particleTypeNum_.push_back(2);
	}
	else
	{
		particleTypeNum_.push_back(0);
	}

#endif

}

void EmitterManager::AddSpriteEmitter(const Vector2& pos, std::string emitterType, std::string particleType, const float& liveTime, const float& actionTime, const float& ActiveTime, float ct, const Vector2& randRengeX, const Vector2& randRengeY, Vector2 startScale, Vector2 endScale, std::string particleTexture, std::string emitterTexture)
{
	std::unique_ptr<IEmitter> newEmitter = std::move(emitterFactory_->CreateEmitter(emitterType));
	newEmitter->Initialize(pos, particleType, liveTime, actionTime, ActiveTime, ct, randRengeX, randRengeY, startScale, endScale, particleTexture, emitterTexture);

	emitters_.push_back(std::move(newEmitter));

#ifdef _DEBUG
	//デバック用のデータを追加するところ
	spritePos_.push_back(pos);

	spriteParticleLiveTime_.push_back(liveTime);
	spriteActiveTime_.push_back(ActiveTime);
	spriteMaxCT_.push_back(ct);
	
	spriteParticleActionTime_.push_back(actionTime);

	spriteParticleType_.push_back(particleType);

	spriteRandRangeX_.push_back(randRengeX);
	spriteRandRangeY_.push_back(randRengeY);

	if (particleType == "BASIC")
	{
		spriteParticleTypeNum_.push_back(0);
	}
	else if (particleType == "Fall")
	{
		spriteParticleTypeNum_.push_back(1);
	}
	else
	{
		spriteParticleTypeNum_.push_back(0);
	}
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
			//デバック用のデータを削除するところ
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

	count = 0;
	for (std::unique_ptr<IEmitter>& emitter : emitters_)
	{
		if (emitter.get()->GetIsEnd())
		{
			//デバック用のデータを削除するところ
			for (auto posI = spritePos_.begin(); posI != spritePos_.end();)
			{
				posI += count;
				posI = spritePos_.erase(posI);
				break;
			}
			for (auto CTI = spriteMaxCT_.begin(); CTI != spriteMaxCT_.end();)
			{
				CTI += count;
				CTI = spriteMaxCT_.erase(CTI);
				break;
			}
			for (auto particleLiveTimeI = spriteParticleLiveTime_.begin(); particleLiveTimeI != spriteParticleLiveTime_.end();)
			{
				particleLiveTimeI += count;
				particleLiveTimeI = spriteParticleLiveTime_.erase(particleLiveTimeI);
				break;
			}
			for (auto activeTimeI = spriteActiveTime_.begin(); activeTimeI != spriteActiveTime_.end();)
			{
				activeTimeI += count;
				activeTimeI = spriteActiveTime_.erase(activeTimeI);
				break;
			}
			for (auto particleActionTimeI = spriteParticleActionTime_.begin(); particleActionTimeI != spriteParticleActionTime_.end();)
			{
				particleActionTimeI += count;
				particleActionTimeI = spriteParticleActionTime_.erase(particleActionTimeI);
				break;
			}
			for (auto particleTypeI = spriteParticleType_.begin(); particleTypeI != spriteParticleType_.end();)
			{
				particleTypeI += count;
				particleTypeI = spriteParticleType_.erase(particleTypeI);
				break;
			}
			for (auto particleTypeNumI = spriteParticleTypeNum_.begin(); particleTypeNumI != spriteParticleTypeNum_.end();)
			{
				particleTypeNumI += count;
				particleTypeNumI = spriteParticleTypeNum_.erase(particleTypeNumI);
				break;
			}
			for (auto randRangeXI = spriteRandRangeX_.begin(); randRangeXI != spriteRandRangeX_.end();)
			{
				randRangeXI += count;
				randRangeXI = spriteRandRangeX_.erase(randRangeXI);
				break;
			}
			for (auto randRangeYI = spriteRandRangeY_.begin(); randRangeYI != spriteRandRangeY_.end();)
			{
				randRangeYI += count;
				randRangeYI = spriteRandRangeY_.erase(randRangeYI);
				break;
			}

		}
		count++;
	}

#endif

	for (std::unique_ptr<IObjEmitter>& objEmitter : objEmitters_)
	{
		objEmitter->Update();
	}

	for (std::unique_ptr<IEmitter>& emitter : emitters_)
	{
		emitter->Update();
	}

	objEmitters_.remove_if([](std::unique_ptr<IObjEmitter>& emitter)
	{
		return emitter->GetIsEnd();
	});

	emitters_.remove_if([](std::unique_ptr<IEmitter>& emitter)
	{
		return emitter->GetIsEnd();
	});

#ifdef _DEBUG


#pragma region effect

	ImGui::Begin("objEffect");

	//エフェクト追加するとこ
	ImGui::DragFloat3("pos", particlePos);

	ImGui::DragFloat2("RandX", effectTestRandX, 0.1f);
	ImGui::DragFloat2("RandY", effectTestRandY, 0.1f);
	ImGui::DragFloat2("RandZ", effectTestRandZ, 0.1f);

	ImGui::DragFloat("liveTime", &effectTestliveTime, 0.1f);
	ImGui::DragFloat("actionTime", &effectTestactionTime, 0.1f);
	ImGui::DragFloat("ct", &emitterCt, 0.1f);

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
	case 0:
		emitterType = "BASIC";
		break;
	default:
		emitterType = "BASIC";
		break;
	}

	switch (particleTypeNum)
	{
	case 0:
		particleType = "BASIC";
		break;
	case 1:
		particleType = "Cartridge";
		break;
	case 2:
		particleType = "Fall";
		break;

	default:
		particleType = "BASIC";
		break;
	}

	if (ImGui::Button("emittrpop"))
	{
		EmitterManager::GetInstance()->AddObjEmitter({ particlePos[0],particlePos[1],particlePos[2] }, emitterType, particleType, effectTestliveTime, effectTestactionTime, -1,emitterCt, { effectTestRandX[0],effectTestRandX[1] }, { effectTestRandY[0],effectTestRandY[1] }, { effectTestRandZ[0],effectTestRandZ[1] });
	}

	if (ImGui::Button("alldel"))
	{
		EmitterManager::GetInstance()->reset();
	}

	ImGui::End();

	ImGui::Begin("Effect");

	//エフェクト追加するとこ
	ImGui::DragFloat2("pos", spriteParticlePos);

	ImGui::DragFloat2("RandX", spriteEffectTestRandX, 0.1f);
	ImGui::DragFloat2("RandY", spriteEffectTestRandY, 0.1f);

	ImGui::DragFloat("liveTime", &spriteEffectTestliveTime, 0.1f);
	ImGui::DragFloat("actionTime", &spriteEffectTestactionTime, 0.1f);
	ImGui::DragFloat("ct", &spriteEmitterCt, 0.1f);

	const char* spriteEmittrChar[] = { "BASIC" };

	//intしか使えん許さん
	ImGui::Combo("emitterType", (int*)&spriteEmittrTypeNum, spriteEmittrChar, IM_ARRAYSIZE(spriteEmittrChar));

	const char* spriteParticleChar[] = { "BASIC","Fall" };

	//intしか使えん許さん
	ImGui::Combo("particleType", (int*)&spriteParticleTypeNum, spriteParticleChar, IM_ARRAYSIZE(spriteParticleChar));

	std::string spriteEmitterType;
	std::string spriteParticleType;

	switch (spriteEmittrTypeNum)
	{
	case 0:
		spriteEmitterType = "BASIC";
		break;
	default:
		spriteEmitterType = "BASIC";
		break;
	}

	switch (spriteParticleTypeNum)
	{
	case 0:
		spriteParticleType = "BASIC";
		break;
	case 1:
		spriteParticleType = "Fall";
		break;

	default:
		spriteParticleType = "BASIC";
		break;
	}

	if (ImGui::Button("emittrpop"))
	{
		EmitterManager::GetInstance()->AddSpriteEmitter({ spriteParticlePos[0],spriteParticlePos[1]}, emitterType, particleType, spriteEffectTestliveTime, spriteEffectTestactionTime, -1,spriteEmitterCt, { spriteEffectTestRandX[0],spriteEffectTestRandX[1] }, { spriteEffectTestRandY[0],spriteEffectTestRandY[1] });
	}

	if (ImGui::Button("alldel"))
	{
		EmitterManager::GetInstance()->reset();
	}

	ImGui::End();

#pragma endregion

#pragma region objEffect

	ImGui::Begin("objEffectList");

	uint32_t effectListCount = 0;

	for (auto i = objEmitters_.begin(); i != objEmitters_.end(); i++)
	{
		//中身いじるよ
		ImGui::Text("%02d:objemitter", effectListCount);
		float posbuff[3] = { pos_[effectListCount].x,pos_[effectListCount].y ,pos_[effectListCount].z };
		ImGui::Text("effectNum:%d", effectListCount);
		ImGui::DragFloat3("pos", posbuff, 0.1f);

		ImGui::DragFloat("particleLiveTime", &particleLiveTime_[effectListCount], 0.1f);
		ImGui::Text("activeTime:%0.3f", activeTime_[effectListCount]);
		ImGui::DragFloat("CT", &maxCT_[effectListCount], 0.1f);
		ImGui::DragFloat("particleActionTime", &particleActionTime_[effectListCount], 0.1f);

		//intしか使えん許さん
		ImGui::Combo("particleType", (int*)&particleTypeNum_[effectListCount], particleChar, IM_ARRAYSIZE(particleChar));

		switch (particleTypeNum_[effectListCount])
		{
		case 0:
			particleType_[effectListCount] = "BASIC";
			break;
		case 1:
			particleType_[effectListCount] = "Cartridge";
			break;
		case 2:
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
		//i->get()->SetActiveTime(activeTime_[effectListCount]);
		i->get()->SetParticleLiveTime(particleLiveTime_[effectListCount]);
		i->get()->SetCT(maxCT_[effectListCount]);
		i->get()->SetParticleType(particleType_[effectListCount]);
		i->get()->SetRandRangeX(randRangeX_[effectListCount]);
		i->get()->SetRandRangeY(randRangeY_[effectListCount]);
		i->get()->SetRandRangeZ(randRangeZ_[effectListCount]);
		i->get()->SetParticleactionTime(particleActionTime_[effectListCount]);

		effectListCount++;

		if (ImGui::Button("delete"))
		{
			i->get()->SetIsEnd(true);
		}
	
		ImGui::Text("\n");

	}

	ImGui::End();

	ImGui::Begin("effectList");

	uint32_t spriteEffectListCount = 0;

	for (auto i = emitters_.begin(); i != emitters_.end(); i++)
	{
		//中身いじるよ
		ImGui::Text("%02d:objemitter", spriteEffectListCount);
		float spriteposbuff[2] = { spritePos_[spriteEffectListCount].x,spritePos_[spriteEffectListCount].y};
		ImGui::Text("effectNum:%d", spriteEffectListCount);
		ImGui::DragFloat2("pos", spriteposbuff, 0.1f);

		ImGui::DragFloat("particleLiveTime", &spriteParticleLiveTime_[spriteEffectListCount], 0.1f);
		ImGui::Text("activeTime:%0.3f", spriteActiveTime_[spriteEffectListCount]);
		ImGui::DragFloat("CT", &spriteMaxCT_[spriteEffectListCount], 0.1f);
		ImGui::DragFloat("particleActionTime", &spriteParticleActionTime_[spriteEffectListCount], 0.1f);

		//intしか使えん許さん
		ImGui::Combo("particleType", (int*)&spriteParticleTypeNum_[spriteEffectListCount], spriteParticleChar, IM_ARRAYSIZE(spriteParticleChar));

		switch (spriteParticleTypeNum_[spriteEffectListCount])
		{
		case 0:
			spriteParticleType_[spriteEffectListCount] = "BASIC";
			break;
		case 1:
			spriteParticleType_[spriteEffectListCount] = "Fall";
			break;

		default:
			spriteParticleType_[spriteEffectListCount] = "BASIC";
			break;
		}

		float spriteRandRangeXbuff[2] = { spriteRandRangeX_[spriteEffectListCount].x,spriteRandRangeX_[spriteEffectListCount].y };
		float spriteRandRangeYbuff[2] = { spriteRandRangeY_[spriteEffectListCount].x,spriteRandRangeY_[spriteEffectListCount].y };
		ImGui::DragFloat2("randRangeX", spriteRandRangeXbuff, 0.1f);
		ImGui::DragFloat2("randRangeY", spriteRandRangeYbuff, 0.1f);


		spritePos_[spriteEffectListCount] = { spriteposbuff[0],spriteposbuff[1]};

		spriteRandRangeX_[spriteEffectListCount] = { spriteRandRangeXbuff[0],spriteRandRangeXbuff[1] };
		spriteRandRangeY_[spriteEffectListCount] = { spriteRandRangeYbuff[0],spriteRandRangeYbuff[1] };

		i->get()->SetPos(spritePos_[spriteEffectListCount]);
		//i->get()->SetActiveTime(activeTime_[effectListCount]);
		i->get()->SetParticleLiveTime(spriteParticleLiveTime_[spriteEffectListCount]);
		i->get()->SetCT(spriteMaxCT_[spriteEffectListCount]);
		i->get()->SetParticleType(spriteParticleType_[spriteEffectListCount]);
		i->get()->SetRandRangeX(spriteRandRangeX_[spriteEffectListCount]);
		i->get()->SetRandRangeY(spriteRandRangeY_[spriteEffectListCount]);
		i->get()->SetParticleactionTime(spriteParticleActionTime_[spriteEffectListCount]);

		if (ImGui::Button("delete"))
		{
			i->get()->SetIsEnd(true);
		}

		spriteEffectListCount++;

		ImGui::Text("\n");

	}

	ImGui::End();

#pragma endregion

#endif


}

void EmitterManager::Draw()
{
	for (std::unique_ptr<IObjEmitter>& objEmitter : objEmitters_)
	{
		objEmitter->Draw();
	}

	for (std::unique_ptr<IEmitter>& emitter : emitters_)
	{
		emitter->Draw();
	}
}

void EmitterManager::reset() 
{ 

	objEmitters_.clear(); 
	emitters_.clear();

#ifdef  _DEBUG
	pos_.clear();

	particleLiveTime_.clear();
	activeTime_.clear();
	maxCT_.clear();
	particleActionTime_.clear();
	particleTypeNum_.clear();

	particleType_.clear();

	randRangeX_.clear();
	randRangeY_.clear();
	randRangeZ_.clear();

	spritePos_.clear();

	spriteMaxCT_.clear();
	spriteParticleLiveTime_.clear();
	spriteActiveTime_.clear();
	spriteParticleActionTime_.clear();

	spriteParticleType_.clear();
	spriteParticleTypeNum_.clear();

	spriteRandRangeX_.clear();
	spriteRandRangeY_.clear();
#endif //  _DEBUG


}