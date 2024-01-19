#pragma once

/**
 * @file IEmitterFactory.h
 * @brief エミッター工場の根底クラス
 * @author フクヤマ
 * @date 2023_12/29
 */

#include "IEmitter.h"
#include <string>
#include <memory>
#include "IObjEmitter.h"

class IEmitterFactory
{
public:
	virtual ~IEmitterFactory() = default;

	//シーン生成
	virtual std::unique_ptr<IObjEmitter> CreateObjEmitter(const std::string& emitterName) = 0;
	virtual std::unique_ptr<IEmitter> CreateEmitter(const std::string& emitterName) = 0;

private:

};