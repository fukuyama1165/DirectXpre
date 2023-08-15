#include "EmitterFactory.h"
#include "BasicEmitter.h"

EmitterFactory::EmitterFactory()
{
}

EmitterFactory::~EmitterFactory()
{
}

std::unique_ptr<IObjEmitter> EmitterFactory::CreateObjEmitter(const std::string& emitterName)
{

	std::unique_ptr<IObjEmitter> newEmitter = nullptr;

	if (emitterName == "BASIC")
	{
		newEmitter = std::make_unique<BasicEmitter>();
	}
	else if (emitterName == "")
	{
		assert(0);
	}
	else
	{
		newEmitter = std::make_unique<BasicEmitter>();
	}

	return std::move(newEmitter);

}

std::unique_ptr<IEmitter> EmitterFactory::CreateEmitter(const std::string& emitterName)
{
	std::unique_ptr<IEmitter> newEmitter = nullptr;

	if (emitterName == "BASIC")
	{
		newEmitter = nullptr;
	}
	else if (emitterName == "")
	{
		assert(0);
	}
	else
	{
		newEmitter = nullptr;
	}

	return std::move(newEmitter);
}

