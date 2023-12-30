#pragma once

/**
 * @file EmitterFactory.h
 * @brief エミッターを生産する工場
 * @author フクヤマ
 * @date 2023_12/29
 */

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
	/// オブジェクトエミッター生成
	/// </summary>
	/// <param name="sceneName">生成したいエミッター</param>
	/// <returns>生成したエミッター</returns>
	std::unique_ptr<IObjEmitter> CreateObjEmitter(const std::string& emitterName)override;

	/// <summary>
	/// エミッター生成(まだ何もないので呼び出しても使えないよ)
	/// </summary>
	/// <param name="sceneName">生成したいエミッター</param>
	/// <returns>生成したエミッター</returns>
	std::unique_ptr<IEmitter> CreateEmitter(const std::string& emitterName)override;
private:

};