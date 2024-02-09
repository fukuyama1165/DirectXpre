#pragma once
/**
 * @file SpotLight.h
 * @brief スポットライト
 * @author フクヤマ
 * @date 2023_12/29
 */


#include "Vector3.h"
#include "Vector2.h"
#include "Util.h"
#include <math.h>

#include <cstdint>

class SpotLight
{
public:
	SpotLight();
	~SpotLight();

	struct ConstBufferData
	{
		//GPUに送るためにこの順番になり
		//間にpadが入っている
		float active;
		float pad01;
		float pad02;
		float pad03;
		Vector3 lightV;
		float pad0;
		Vector3 lightPos;
		float pad1;
		Vector3 lightColor;
		float pad2;
		Vector3 lightAtten;
		float pad3;	
		Vector3 lightFactorAhgleCos;
		float pad4;
		
		
		

	};

	void SetLightDir(const Vector3& LightDir) { lightDir_ = Vector3::normalize(LightDir); }

	const Vector3& GetLightDir() { return lightDir_; }

	void SetLightPos(const Vector3& LightPos) { lightPos_ = LightPos; }

	const Vector3& GetLightPos() { return lightPos_; }

	void SetLightColor(const Vector3& LightColor) { lightColor_ = LightColor; }

	const Vector3& GetLightColor() { return lightColor_; }

	void SetLightAtten(const Vector3& LightAtten) { lightAtten_ = LightAtten; }

	const Vector3& GetLightAtten() { return lightAtten_; }

	void SetLightFactorAhgle(const Vector2& LightFactorAhgle) { lightFactorAngleCos_.x = cosf(DirectXpre::Util::AngleToRadian(LightFactorAhgle.x)), lightFactorAngleCos_.y = cosf(DirectXpre::Util::AngleToRadian(LightFactorAhgle.y)); };

	const Vector2& GetLightFactorAhgleCos() { return lightFactorAngleCos_; }

	void SetIsActive(bool IsActive) { active_ = IsActive; }

	bool IsActive() { return active_; }

private:

	//ライト方向
	Vector3 lightDir_ = { 0,0,0 };

	//ライト座標
	Vector3 lightPos_ = { 0,0,0 };

	//ライトの色
	Vector3 lightColor_ = { 1,1,1 };

	//ライト距離減衰係数
	Vector3 lightAtten_ = { 1.0f,1.0f,1.0f };

	//ライト減衰角度(開始角度、終了角度)
	Vector2 lightFactorAngleCos_ = { 0.5f,0.2f };

	//有効フラグ
	bool active_ = false;

};

