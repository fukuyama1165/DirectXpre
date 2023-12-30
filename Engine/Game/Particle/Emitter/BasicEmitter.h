#pragma once

/**
 * @file BasicEmitter.h
 * @brief 基礎のスプライトのエミッター
 * @author フクヤマ
 * @date 2023_12/29
 */
#include "IEmitter.h"
#include "ParticleFactory.h"

class BasicEmitter:public IEmitter
{
public:
	BasicEmitter();
	~BasicEmitter()override;

	//初期化
	void Initialize()override;

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
	void Initialize(const Vector2& pos, std::string particleType, const float& liveTime = 10, const float& actionMaxTime = 10, const float& ActiveTime = -1, float ct = 1, const Vector2& randRengeX = { -1.0f,1.0f }, const Vector2& randRengeY = { -1.0f,1.0f }, Vector2 startScale = { 1,1 }, Vector2 endScale = { 1,1 }, std::string particleTexture = "basketballMan", std::string emitterTexture = "white1x1")override;

	//終了処理
	void Finalize()override;

	//毎フレーム更新
	void Update()override;

	//描画
	void Draw()override;

	void SetIsActive(bool flag)override { isActive_ = flag; };
	void SetIsEnd(bool flag)override { isEnd_ = flag; };


	void SetPos(Vector2 pos)override { emitter_.pos_ = pos; };

	bool GetIsActive()override { return isActive_; };
	bool GetIsEnd()override { return isEnd_; };

	uint32_t GetParticleNum()override { return (uint32_t)particles_.size(); };

	void SetCT(float ct)override { maxCT_ = ct; };

	void SetParticleLiveTime(float livetime)override { particleLiveTime_ = livetime; };

	void SetParticleType(std::string type)override { particleType_ = type; };

	void SetIsDraw(bool flag)override { isDraw = flag; };

	void SetActiveTime(float time)override { activeTime_ = time; };

	void SetRandRangeX(Vector2 Range)override { randRangeX_ = Range; };
	void SetRandRangeY(Vector2 Range)override { randRangeY_ = Range; };

	void SetParticleactionTime(float time)override { particleactionTime_ = time; };

	void ChengParticleTexture(std::string textureName)override { particleTexture_ = textureName; };

private:

	bool isActive_ = true;
	bool isEnd_ = false;

	std::list<std::unique_ptr<IParticle>> particles_;

	std::unique_ptr<ParticleFactory> particleFactory_ = nullptr;

	float CT_ = 0;

	float maxCT_ = 10;

	float particleLiveTime_ = 10;

	std::string particleType_ = "";

	Sprite emitter_;

	bool isDraw = false;

	float activeTime_ = 0;

	Vector2 randRangeX_ = { -1,1 };
	Vector2 randRangeY_ = { -1,1 };

	Vector2 startScale_ = { 1,1 };
	Vector2 endScale_ = { 1,1 };

	float particleactionTime_ = 10;

	std::string particleTexture_;
};