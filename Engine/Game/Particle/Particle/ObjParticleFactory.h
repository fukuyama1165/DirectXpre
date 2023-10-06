#pragma once

#include "IObjParticleFactory.h"


class ObjParticleFactory :public IObjParticleFactory
{
public:
	ObjParticleFactory();
	~ObjParticleFactory();

	/// <summary>
	/// パーティクル生成
	/// </summary>
	/// <param name="sceneName">生成したいパーティクル</param>
	/// <returns>生成したパーティクル</returns>
	std::unique_ptr<IObjParticle> CreateObjParticle(const std::string& objParticleName)override;

private:

};