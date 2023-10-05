#pragma once
#include "IEmitter.h"
#include <string>
#include <memory>
#include "IObjEmitter.h"

class IEmitterFactory
{
public:
	virtual ~IEmitterFactory() = default;

	//ƒV[ƒ“¶¬
	virtual std::unique_ptr<IObjEmitter> CreateObjEmitter(const std::string& emitterName) = 0;
	virtual std::unique_ptr<IEmitter> CreateEmitter(const std::string& emitterName) = 0;

private:

};