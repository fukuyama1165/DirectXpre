#pragma once
#include "IObjParticle.h"
#include <string>
#include <memory>

class IObjParticleFactory
{
public:
	virtual ~IObjParticleFactory() = default;

	//ÉVÅ[Éìê∂ê¨
	virtual std::unique_ptr<IObjParticle> CreateObjParticle(const std::string& objParticleName) = 0;

private:

};