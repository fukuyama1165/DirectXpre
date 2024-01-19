#pragma once

/**
 * @file ParticleFactory.h
 * @brief パーティクル工場
 * @author フクヤマ
 * @date 2023_12/29
 */

#include "IParticleFactory.h"


class ParticleFactory :public IParticleFactory
{
public:
	ParticleFactory();
	~ParticleFactory();

	/// <summary>
	/// パーティクル生成
	/// </summary>
	/// <param name="sceneName">生成したいパーティクル</param>
	/// <returns>生成したパーティクル</returns>
	std::unique_ptr<IParticle> CreateParticle(const std::string& ParticleName)override;

	/// <summary>
	/// パーティクル生成
	/// </summary>
	/// <param name="sceneName">生成したいパーティクル</param>
	/// <returns>生成したパーティクル</returns>
	std::unique_ptr<IObjParticle> CreateObjParticle(const std::string& objParticleName)override;

private:

};