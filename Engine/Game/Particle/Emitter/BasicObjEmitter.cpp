#include "BasicObjEmitter.h"
#include "Util.h"
#include "ModelManager.h"

BasicObjEmitter::BasicObjEmitter()
{
}

BasicObjEmitter::~BasicObjEmitter()
{
}

void BasicObjEmitter::Initialize()
{
	particleModel_ = ModelManager::GetInstance()->SearchModelData("whiteBox");

	emitterModel_ = ModelManager::GetInstance()->SearchModelData("whiteBox");

	obj_.FBXInit();

	obj_.SetPos({0,0,0});

	activeTime_ = -1;

	particleFactory_ = std::make_unique<ParticleFactory>();

	particleType_ = "BASIC";
}

void BasicObjEmitter::Initialize(const Vector3& pos, std::string particleType,const float& liveTime, const float& actionMaxTime, const float& ActiveTime, float ct, const Vector2& randRengeX, const Vector2& randRengeY, const Vector2& randRengeZ, Vector3 startScale, Vector3 endScale, std::string particleModel, std::string emitterModel)
{

	particleModel_ = ModelManager::GetInstance()->SearchModelData(particleModel);

	emitterModel_ = ModelManager::GetInstance()->SearchModelData(emitterModel);

	obj_.FBXInit();

	obj_.SetPos(pos);

	activeTime_ = ActiveTime;

	particleFactory_ = std::make_unique<ParticleFactory>();

	particleType_ = particleType;

	particleLiveTime_ = liveTime;

	randRangeX_ = randRengeX;
	randRangeY_ = randRengeY;
	randRangeZ_ = randRengeZ;

	particleactionTime_ = actionMaxTime;

	startScale_ = startScale;
	endScale_ = endScale;

	CT_ = ct;
	maxCT_ = ct;

}

void BasicObjEmitter::Initialize(const Vector3& pos, const ParticleData& particleData, std::string particleModel, std::string emitterModel)
{
	particleModel_ = ModelManager::GetInstance()->SearchModelData(particleModel);

	emitterModel_ = ModelManager::GetInstance()->SearchModelData(emitterModel);

	obj_.FBXInit();

	obj_.SetPos(pos);

	activeTime_ = particleData.ActiveTime;

	particleFactory_ = std::make_unique<ParticleFactory>();

	particleType_ = particleData.Name;

	particleLiveTime_ = particleData.liveTime;

	randRangeX_ = particleData.randRengeX;
	randRangeY_ = particleData.randRengeY;
	randRangeZ_ = particleData.randRengeZ;

	particleactionTime_ = particleData.actionTime;

	startScale_ = particleData.startScale;
	endScale_ = particleData.endScale;

	CT_ = particleData.ct;
	maxCT_ = particleData.ct;
}


void BasicObjEmitter::Finalize()
{

}

void BasicObjEmitter::Update()
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

	if (!isActive_ && particles_.size() <= 0) isEnd_ = true;

	if (!isActive_)return;

	if (CT_ <= 0)
	{
		Vector3 velo(DirectXpre::Util::Rand(randRangeX_.x, randRangeX_.y), DirectXpre::Util::Rand(randRangeY_.x, randRangeY_.y), DirectXpre::Util::Rand(randRangeZ_.x, randRangeZ_.y));

		//velo.normalize();

		std::unique_ptr<IObjParticle> newParticle = std::move(particleFactory_->CreateObjParticle(particleType_));

		newParticle->Initialize(obj_.GetWorldPos(), velo, particleLiveTime_, particleactionTime_,startScale_,endScale_);

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

void BasicObjEmitter::Draw()
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