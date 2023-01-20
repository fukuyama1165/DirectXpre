#pragma once
#include "matrix4x4.h"
#include "Vector3.h"
#include "Vector4.h"
#include <math.h>
#include "PI.h"


class Camera
{
public:
	Camera();
	Camera(float win_width,float win_height);
	~Camera();

	void upDate();

private:
	Matrix4x4 matViewGeneration(Vector3 eye, Vector3 target, Vector3 up);

	Matrix4x4 perspectiveProjectionGeneration(float FovAngleY, float NearZ, float FarZ);

	/// <summary>
	/// sinとcosを返す関数
	/// </summary>
	/// <param name="Sin">sinの値が入る引数</param>
	/// <param name="Cos">cosの値が入る引数</param>
	/// <param name="angle">Θの値</param>
	void sinCos(float& Sin, float& Cos, float angle);

	Vector3 float3Dat(Vector3 A, Vector3 B);

public:


	//透視投影行列
	Matrix4x4 matProjection;

	//ビュー変換行列
	Matrix4x4 matView;
	Vector3 eye_ = {};//視点座標
	Vector3 target_ = {};//注視点座標
	Vector3 up_= { 0, 1, 0 };//上方向ベクトル

private:
	//画面サイズ
	float Win_width;
	float Win_height;
};

