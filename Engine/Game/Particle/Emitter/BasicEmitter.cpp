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

void BasicEmitter::Initialize(const Vector3& pos, std::string particleType,const float& liveTime, const float& ActiveTime, const float& actionMaxTime, const Vector2& randRengeX, const Vector2& randRengeY, const Vector2& randRengeZ, std::string particleModel, std::string emitterModel)
{

	particleModel_ = ModelManager::GetInstance()->SearchModelData(particleModel);

	emitterModel_ = ModelManager::GetInstance()->SearchModelData(emitterModel);

	obj_.FBXInit();

	obj_.SetPos(pos);

	activeTime_ = ActiveTime;

	particleFactory_ = std::make_unique<ObjParticleFactory>();

	particleType_ = particleType;

	particleLiveTime_ = liveTime;

	randRangeX_ = randRengeX;
	randRangeY_ = randRengeY;
	randRangeZ_ = randRengeZ;

	particleactionTime_ = actionMaxTime;

}

void BasicEmitter::Finalize()
{

}

void BasicEmitter::Update()
{
	if (isEnd_)return;

	particles_.remove_if([](std::unique_ptr<IObjParticle>& particle)
	{
			return particle->GetliveTime() <= 0;
	});

	for (std::unique_ptr<IObjParticle>& particle : particles_)
	{
		particle->Update();
	}

	if (!isActive_ and particles_.size() <= 0) isEnd_ = true;

	if (!isActive_)return;

	if (CT_ <= 0)
	{
		Vector3 velo(Util::Rand(randRangeX_.x, randRangeX_.y), Util::Rand(randRangeY_.x, randRangeY_.y), Util::Rand(randRangeZ_.x, randRangeZ_.y));

		//velo.normalize();

		std::unique_ptr<IObjParticle> newParticle = std::move(particleFactory_->CreateObjParticle(particleType_));

		newParticle->Initialize(obj_.GetWorldPos(), velo, particleLiveTime_, particleactionTime_);

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