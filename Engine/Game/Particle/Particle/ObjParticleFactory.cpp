#include "ObjParticleFactory.h"
#include "BasicParticle.h"
#include "CartridgeParticle.h"
#include "FallParticle.h"

ObjParticleFactory::ObjParticleFactory()
{
}

ObjParticleFactory::~ObjParticleFactory()
{
}

std::unique_ptr<IObjParticle> ObjParticleFactory::CreateObjParticle(const std::string& objParticleName)
{

	std::unique_ptr<IObjParticle> newParticle = nullptr;

	if (objParticleName == "BASIC")
	{
		newParticle = std::make_unique<BasicParticle>();
	}
	else if (objParticleName == "Cartridge")
	{
		newParticle = std::make_unique<CartridgeParticle>();
	}
	else if (objParticleName == "Fall")
	{
		newParticle = std::make_unique<FallParticle>();
	}
	else if (objParticleName == "")
	{
		assert(0);
	}
	else
	{
		newParticle = std::make_unique<BasicParticle>();
	}

	return std::move(newParticle);

}