#pragma once
#include "Vector3.h"

//uint系
#include <cstdint>

class PointLight
{
public:
	PointLight();
	~PointLight();

	struct ConstBufferData
	{

		//位置
		Vector3 lightPos;
		float pad1;
		//色
		Vector3 lightColor;
		float pad2;
		//距離減衰係数
		Vector3 lightAtten;
		float pad3;
		float active;

		float pad00;
		float pad01;
		float pad02;
		
		

	};

	void SetLightPos(const Vector3& LightPos) { lightPos_ = LightPos; }

	const Vector3& GetLightPos() { return lightPos_; }

	void SetLightColor(const Vector3& LightColor) { lightColor_ = LightColor; }

	const Vector3& GetLightColor() { return lightColor_; }

	void SetLightAtten(const Vector3& LightAtten) { lightAtten_ = LightAtten; }

	const Vector3& GetLightAtten() { return lightAtten_; }

	void SetIsActive(bool IsActive) { active_ = IsActive; }

	bool IsActive() { return active_; }

private:

	//ライト座標
	Vector3 lightPos_ = { 0,0,0 };

	//ライトの色
	Vector3 lightColor_ = { 1,1,1 };

	//ライト距離減衰係数
	Vector3 lightAtten_ = { 1.0f,1.0f,1.0f };

	//有効フラグ
	bool active_ = false;

};