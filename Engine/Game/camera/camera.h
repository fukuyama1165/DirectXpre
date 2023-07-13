#pragma once
#include "matrix4x4.h"
#include "Vector3.h"
#include "Vector4.h"
#include <math.h>
#include "PI.h"
#include "WinApp.h"


class Camera
{
public:
	Camera();
	Camera(float win_width,float win_height);
	~Camera();

	void upDate();

private:
	Matrix4x4 matViewGeneration(const Vector3& eye,const Vector3& target,const Vector3& up);

	Matrix4x4 perspectiveProjectionGeneration(float FovAngleY,float NearZ,float FarZ);

	/// <summary>
	/// sinとcosを返す関数
	/// </summary>
	/// <param name="Sin">sinの値が入る引数</param>
	/// <param name="Cos">cosの値が入る引数</param>
	/// <param name="angle">Θの値</param>
	void sinCos(float& Sin, float& Cos,const float& angle);

	Vector3 float3Dat(const Vector3& A, const Vector3& B);

public:


	//透視投影行列
	Matrix4x4 matProjection_;

	//ビュー変換行列
	Matrix4x4 matView_;
	Vector3 eye_ = {};//視点座標
	Vector3 target_ = {};//注視点座標
	Vector3 up_= { 0, 1, 0 };//上方向ベクトル

	Vector3 forward_ = { 0,0,1 };//正面

private:
	//画面サイズ
	float Win_width_;
	float Win_height_;
};

