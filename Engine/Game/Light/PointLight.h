#pragma once
#include "Vector3.h"

//uint�n
#include <cstdint>

class PointLight
{
public:
	PointLight();
	~PointLight();

	struct ConstBufferData
	{

		//�ʒu
		Vector3 lightPos;
		float pad1;
		//�F
		Vector3 lightColor;
		float pad2;
		//���������W��
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

	//���C�g���W
	Vector3 lightPos_ = { 0,0,0 };

	//���C�g�̐F
	Vector3 lightColor_ = { 1,1,1 };

	//���C�g���������W��
	Vector3 lightAtten_ = { 1.0f,1.0f,1.0f };

	//�L���t���O
	bool active_ = false;

};