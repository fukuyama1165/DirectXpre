#pragma once

/**
 * @file IEmitter.h
 * @brief エミッターの根底クラス
 * @author フクヤマ
 * @date 2023_12/29
 */

#include "Vector2.h"
#include <string>
#include "Sprite.h"

class IEmitter
{
public:
	virtual ~IEmitter() = default;

	//初期化
	virtual void Initialize() = 0;
	/// <summary>
	/// 出すパーティクル等の設定と初期化
	/// </summary>
	/// <param name="pos">出てくる位置</param>
	/// <param name="particleType">出てくるパーティクルの種類</param>
	/// <param name="liveTime">パーティクルの生存時間</param>
	/// <param name="actionMaxTime">パーティクルの行動するときの時間</param>
	/// <param name="ActiveTime">エミッターの動く時間(-1で無限)</param>
	/// <param name="randRengeX">パーティクルの出てくるX範囲</param>
	/// <param name="randRengeY">パーティクルの出てくるY範囲</param>
	/// <param name="startScale">開始時のサイズ</param>
	/// <param name="endScale">終わるときのサイズ</param>
	/// <param name="particleTexture">パーティクルの画像設定</param>
	/// <param name="emitterTexture">エミッターの画像設定(普段は描画してない)</param>
	virtual void Initialize(const Vector2& pos, std::string particleType, const float& liveTime = 10, const float& actionMaxTime = 10, const float& ActiveTime = -1, float ct = 1, const Vector2& randRengeX = { -1.0f,1.0f }, const Vector2& randRengeY = { -1.0f,1.0f }, Vector2 startScale = { 1,1 }, Vector2 endScale = { 1,1 }, std::string particleTexture = "basketballMan", std::string emitterTexture = "white1x1") = 0;

	//終了処理
	virtual void Finalize() = 0;

	//毎フレーム更新
	virtual void Update() = 0;

	//描画
	virtual void Draw() = 0;

	virtual void SetIsActive(bool flag) = 0;
	virtual void SetIsEnd(bool flag) = 0;

	virtual void SetPos(Vector2 pos) = 0;

	virtual bool GetIsActive() = 0;
	virtual bool GetIsEnd() = 0;

	virtual uint32_t GetParticleNum() = 0;

	virtual void SetCT(float ct) = 0;

	virtual void SetParticleLiveTime(float livetime) = 0;

	virtual void SetParticleType(std::string type) = 0;

	virtual void SetIsDraw(bool flag) = 0;

	virtual void SetActiveTime(float time) = 0;

	virtual void SetRandRangeX(Vector2 Range) = 0;
	virtual void SetRandRangeY(Vector2 Range) = 0;

	virtual void SetParticleactionTime(float time) = 0;

	virtual void ChengParticleTexture(std::string textureName) = 0;

private:

	bool isActive_ = true;
	bool isEnd_ = false;

	float CT_ = 0;

	float maxCT_ = 10;

	float particleLiveTime_ = 10;

	std::string particleType_ = "";

	Sprite emitter_;

	std::string particleTexture_;

	bool isDraw = false;

	float activeTime_ = 0;

	Vector2 randRangeX_ = { -1,1 };
	Vector2 randRangeY_ = { -1,1 };

	float particleactionTime_ = 10;
};