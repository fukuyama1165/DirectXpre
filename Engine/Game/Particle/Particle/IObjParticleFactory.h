#pragma once
#include "IObjParticle.h"
#include <string>
#include <memory>

class IObjParticleFactory
{
public:
	virtual ~IObjParticleFactory() = default;

	//シーン生成
	virtual std::unique_ptr<IObjParticle> CreateObjParticle(const std::string& objParticleName) = 0;

private:

};