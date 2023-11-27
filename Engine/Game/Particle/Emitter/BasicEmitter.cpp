#include "BasicEmitter.h"
#include "Util.h"
#include "ModelManager.h"

BasicEmitter::BasicEmitter()
{
}

BasicEmitter::~BasicEmitter()
{
}

void BasicEmitter::Initialize()
{
	emitter_.initialize("white1x1");

	particleTexture_ = "basketballMan";

	emitter_.pos_ = { 0,0 };

	activeTime_ = -1;

	particleFactory_ = std::make_unique<ParticleFactory>();

	particleType_ = "BASIC";
}

void BasicEmitter::Initialize(const Vector2& pos, std::string particleType, const float& liveTime, const float& actionMaxTime, const float& ActiveTime, float ct, const Vector2& randRengeX, const Vector2& randRengeY, Vector2 startScale, Vector2 endScale, std::string particleTexture, std::string emitterTexture)
{

	emitter_.initialize(emitterTexture);

	particleTexture_ = particleTexture;

	
	emitter_.pos_ = pos;

	activeTime_ = ActiveTime;

	particleFactory_ = std::make_unique<ParticleFactory>();

	particleType_ = particleType;

	particleLiveTime_ = liveTime;

	randRangeX_ = randRengeX;
	randRangeY_ = randRengeY;

	startScale_ = startScale;
	endScale_ = endScale;

	particleactionTime_ = actionMaxTime;

	CT_ = ct;
	maxCT_ = ct;
}

void BasicEmitter::Finalize()
{

}

void BasicEmitter::Update()
{
	//終わっているので消す
	if (isEnd_)return;

	//終わったパーティクルを消す
	particles_.remove_if([](std::unique_ptr<IParticle>& particle)
	{
		return particle->GetliveTime() <= 0;
	});

	//パーティクル生成(isactiveが条件式にあるのはパーティクルのUpdateの前に処理したかったから)
	if (CT_ <= 0 and isActive_)
	{
		Vector2 velo(Util::Rand(randRangeX_.x, randRangeX_.y), Util::Rand(randRangeY_.x, randRangeY_.y));

		//velo.normalize();

		std::unique_ptr<IParticle> newParticle = std::move(particleFactory_->CreateParticle(particleType_));

		newParticle->Initialize(emitter_.pos_, velo, particleLiveTime_, particleactionTime_, startScale_, endScale_, particleTexture_);

		particles_.push_back(std::move(newParticle));

		CT_ = maxCT_;

	}

	for (std::unique_ptr<IParticle>& particle : particles_)
	{
		particle->Update();
	}
	

	//起動していなくてパーティクルが一つも無かったら死んだことにする
	if (!isActive_ and particles_.size() <= 0) isEnd_ = true;

	//機能してないなら動かさない
	if (!isActive_)return;

	emitter_.Update();

	

	if (CT_ > 0)
	{
		CT_--;
	}

	if (activeTime_ > 0)
	{
		activeTime_--;
	}
	else if (activeTime_ == 0)
	{
		isActive_ = false;
	}


}

void BasicEmitter::Draw()
{


	if (isDraw)
	{
		emitter_.Draw();
	}

	for (std::unique_ptr<IParticle>& particle : particles_)
	{
		particle->Draw();
	}
}