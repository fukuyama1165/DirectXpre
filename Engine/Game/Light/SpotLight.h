#pragma once

#include "Vector3.h"
#include "Vector2.h"
#include "PI.h"
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
		
		Vector3 lightV;
		Vector3 lightPos;
		float pad1;
		Vector3 lightColor;
		float pad2;
		Vector3 lightAtten;
		float pad3;	
		Vector3 lightFactorAhgleCos;
		float pad4;
		
		
		

	};

	inline void SetLightDir(const Vector3& LightDir) { lightDir_ = Vector3::normalize(LightDir); }

	inline const Vector3& GetLightDir() { return lightDir_; }

	inline void SetLightPos(const Vector3& LightPos) { lightPos_ = LightPos; }

	inline const Vector3& GetLightPos() { return lightPos_; }

	inline void SetLightColor(const Vector3& LightColor) { lightColor_ = LightColor; }

	inline const Vector3& GetLightColor() { return lightColor_; }

	inline void SetLightAtten(const Vector3& LightAtten) { lightAtten_ = LightAtten; }

	inline const Vector3& GetLightAtten() { return lightAtten_; }

	inline void SetLightFactorAhgle(const Vector2& LightFactorAhgle) { lightFactorAngleCos_.x = cosf(AngleToRadian(LightFactorAhgle.x)), lightFactorAngleCos_.y = cosf(AngleToRadian(LightFactorAhgle.y)); };

	inline const Vector2& GetLightFactorAhgleCos() { return lightFactorAngleCos_; }

	inline void SetIsActive(bool IsActive) { active_ = IsActive; }

	inline bool IsActive() { return active_; }

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

