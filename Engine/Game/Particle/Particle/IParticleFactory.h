#pragma once

/**
 * @file IParticleFactory.h
 * @brief パーティクル工場の基底クラス
 * @author フクヤマ
 * @date 2023_12/29
 */

#include "IParticle.h"
#include "IObjParticle.h"
#include <string>
#include <memory>

class IParticleFactory
{
public:
	virtual ~IParticleFactory() = default;

	//パーティクル生成
	virtual std::unique_ptr<IParticle> CreateParticle(const std::string& ParticleName) = 0;
	//オブジェクトパーティクル生成
	virtual std::unique_ptr<IObjParticle> CreateObjParticle(const std::string& objParticleName) = 0;

private:

};