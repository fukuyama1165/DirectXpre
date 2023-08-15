#pragma once

#include "IObjParticleFactory.h"


class ObjParticleFactory :public IObjParticleFactory
{
public:
	ObjParticleFactory();
	~ObjParticleFactory();

	/// <summary>
	/// �p�[�e�B�N������
	/// </summary>
	/// <param name="sceneName">�����������p�[�e�B�N��</param>
	/// <returns>���������p�[�e�B�N��</returns>
	std::unique_ptr<IObjParticle> CreateObjParticle(const std::string& objParticleName)override;

private:

};