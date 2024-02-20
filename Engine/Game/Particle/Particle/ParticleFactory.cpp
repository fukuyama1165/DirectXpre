#include "ParticleFactory.h"
#include "BasicParticle.h"
#include "BasicObjParticle.h"
#include "CartridgeParticle.h"
#include "FallParticle.h"
#include "FallSpriteParticle.h"
#include "ExplosionParticle.h"

ParticleFactory::ParticleFactory()
{
}

ParticleFactory::~ParticleFactory()
{
}

std::unique_ptr<IParticle> ParticleFactory::CreateParticle(const std::string& ParticleName)
{

	std::unique_ptr<IParticle> newParticle = nullptr;

	if (ParticleName == "BASIC")
	{
		newParticle = std::make_unique<BasicParticle>();
	}
	else if (ParticleName == "Fall")
	{
		newParticle = std::make_unique<FallSpriteParticle>();
	}
	else if (ParticleName == "")
	{
		assert(0);
	}
	else
	{
		newParticle = std::make_unique<BasicParticle>();
	}

	return std::move(newParticle);

}

std::unique_ptr<IObjParticle> ParticleFactory::CreateObjParticle(const std::string& objParticleName)
{

	std::unique_ptr<IObjParticle> newParticle = nullptr;

	if (objParticleName == "BASIC")
	{
		newParticle = std::make_unique<BasicObjParticle>();
	}
	else if (objParticleName == "Cartridge")
	{
		newParticle = std::make_unique<CartridgeParticle>();
	}
	else if (objParticleName == "Fall")
	{
		newParticle = std::make_unique<FallParticle>();
	}
	else if (objParticleName == "Explosion")
	{
		newParticle = std::make_unique<ExplosionParticle>();
	}
	else if (objParticleName == "")
	{
		assert(0);
	}
	else
	{
		newParticle = std::make_unique<BasicObjParticle>();
	}

	return std::move(newParticle);

}