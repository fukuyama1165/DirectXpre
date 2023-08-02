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

	void setDefCamera();

private:
	Matrix4x4 matViewGeneration(const Vector3& eye,const Vector3& target,const Vector3& up);

	Matrix4x4 perspectiveProjectionGeneration(float FovAngleY,float NearZ,float FarZ);

	/// <summary>
	/// sin��cos��Ԃ��֐�
	/// </summary>
	/// <param name="Sin">sin�̒l���������</param>
	/// <param name="Cos">cos�̒l���������</param>
	/// <param name="angle">���̒l</param>
	void sinCos(float& Sin, float& Cos,const float& angle);

	Vector3 float3Dat(const Vector3& A, const Vector3& B);

public:


	//�������e�s��
	Matrix4x4 matProjection_;

	//�r���[�ϊ��s��
	Matrix4x4 matView_;
	Vector3 eye_ = {};//���_���W
	Vector3 target_ = {};//�����_���W
	Vector3 up_= { 0, 1, 0 };//������x�N�g��

	Vector3 forward_ = { 0,0,1 };//����

	Vector3 rightDirection = { 1,0,0 };

	static Camera* nowCamera;

private:
	//��ʃT�C�Y
	float Win_width_;
	float Win_height_;

};

