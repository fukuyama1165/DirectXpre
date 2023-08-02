#include "BasicEmitter.h"
#include "PI.h"
#include "ModelManager.h"

BasicEmitter::BasicEmitter()
{
}

BasicEmitter::~BasicEmitter()
{
}

void BasicEmitter::Initialize(const Vector3& pos, float ActiveTime)
{
	/*particleModel_ = std::make_unique<AnimationModel>();

	particleModel_->Load("testFBX", "gltf", "white1x1");

	emitterModel_ = std::make_unique<AnimationModel>();

	emitterModel_->Load("testFBX", "gltf", "white1x1");*/

	particleModel_ = ModelManager::GetInstance()->SearchModelData("whiteBox");

	emitterModel_ = ModelManager::GetInstance()->SearchModelData("whiteBox");

	obj_.FBXInit();

	obj_.SetPos(pos);

	activeTime_ = ActiveTime;

}

void BasicEmitter::Finalize()
{

}

void BasicEmitter::Update(const Camera& camera)
{
	

	particles_.remove_if([](std::unique_ptr<BasicParticle>& particle)
	{
			return particle->GetliveTime() <= 0;
	});

	if (!isActive_)return;

	if (CT_ <= 0)
	{
		Vector3 velo(Util::Rand(-100.0f, 100.0f), Util::Rand(-100.0f, 100.0f), Util::Rand(-100.0f, 100.0f));

		velo.normalize();

		std::unique_ptr<BasicParticle> newParticle = std::make_unique<BasicParticle>();

		newParticle->Initialize(obj_.GetWorldPos(), velo, particleLiveTime_);

		particles_.push_back(std::move(newParticle));

		CT_ = maxCT_;

	}

	for (std::unique_ptr<BasicParticle>& particle : particles_)
	{
		particle->Update(camera);
	}

	obj_.Update(camera);

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

	if (!isActive_)return;

	for (std::unique_ptr<BasicParticle>& particle : particles_)
	{
		particle->Draw(particleModel_);
	}
}