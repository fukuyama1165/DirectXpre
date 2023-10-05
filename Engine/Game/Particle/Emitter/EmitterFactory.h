#pragma once
#include "IEmitterFactory.h"
#include "IObjEmitter.h"
#include "IEmitter.h"
#include <string>
#include <memory>


class EmitterFactory :public IEmitterFactory
{
public:
	EmitterFactory();
	~EmitterFactory();

	/// <summary>
	/// �I�u�W�F�N�g�G�~�b�^�[����
	/// </summary>
	/// <param name="sceneName">�����������G�~�b�^�[</param>
	/// <returns>���������G�~�b�^�[</returns>
	std::unique_ptr<IObjEmitter> CreateObjEmitter(const std::string& emitterName)override;

	/// <summary>
	/// �G�~�b�^�[����(�܂������Ȃ��̂ŌĂяo���Ă��g���Ȃ���)
	/// </summary>
	/// <param name="sceneName">�����������G�~�b�^�[</param>
	/// <returns>���������G�~�b�^�[</returns>
	std::unique_ptr<IEmitter> CreateEmitter(const std::string& emitterName)override;
private:

};