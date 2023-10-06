#include "BasicEmitter.h"
#include "PI.h"
#include "ModelManager.h"

BasicEmitter::BasicEmitter()
{
}

BasicEmitter::~BasicEmitter()
{
}

void BasicEmitter::Initialize()
{
	particleModel_ = ModelManager::GetInstance()->SearchModelData("whiteBox");

	emitterModel_ = ModelManager::GetInstance()->SearchModelData("whiteBox");

	obj_.FBXInit();

	obj_.SetPos({0,0,0});

	activeTime_ = -1;

	particleFactory_ = std::make_unique<ObjParticleFactory>();

	particleType_ = "BASIC";
}

void BasicEmitter::Initialize(const Vector3& pos,std::string particleType, std::string particleModel, std::string emitterModel, float ActiveTime)
{

	particleModel_ = ModelManager::GetInstance()->SearchModelData(particleModel);

	emitterModel_ = ModelManager::GetInstance()->SearchModelData(emitterModel);

	obj_.FBXInit();

	obj_.SetPos(pos);

	activeTime_ = ActiveTime;

	particleFactory_ = std::make_unique<ObjParticleFactory>();

	particleType_ = particleType;

}

void BasicEmitter::Finalize()
{

}

void BasicEmitter::Update()
{
	

	particles_.remove_if([](std::unique_ptr<IObjParticle>& particle)
	{
			return particle->GetliveTime() <= 0;
	});

	for (std::unique_ptr<IObjParticle>& particle : particles_)
	{
		particle->Update();
	}

	if (!isActive_)return;

	if (CT_ <= 0)
	{
		Vector3 velo(Util::Rand(-100.0f, 100.0f), Util::Rand(-100.0f, 100.0f), Util::Rand(-100.0f, 100.0f));

		velo.normalize();

		std::unique_ptr<IObjParticle> newParticle = std::move(particleFactory_->CreateObjParticle(particleType_));

		newParticle->Initialize(obj_.GetWorldPos(), velo, particleLiveTime_);

		particles_.push_back(std::move(newParticle));

		CT_ = maxCT_;

	}

	

	obj_.Update();

	if (CT_ > 0)
	{
		CT_--;
	}

	if (activeTime_ > 0)
	{
		activeTime_--;
	}
	else if(activeTime_ == 0)
	{
		isActive_ = false;
	}


}

void BasicEmitter::Draw()
{
	

	if (isDraw)
	{
		obj_.FBXDraw(*emitterModel_);
	}

	for (std::unique_ptr<IObjParticle>& particle : particles_)
	{
		particle->Draw(particleModel_);
	}
}