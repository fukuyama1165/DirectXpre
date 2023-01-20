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
	/// sin��cos��Ԃ��֐�
	/// </summary>
	/// <param name="Sin">sin�̒l���������</param>
	/// <param name="Cos">cos�̒l���������</param>
	/// <param name="angle">���̒l</param>
	void sinCos(float& Sin, float& Cos, float angle);

	Vector3 float3Dat(Vector3 A, Vector3 B);

public:


	//�������e�s��
	Matrix4x4 matProjection;

	//�r���[�ϊ��s��
	Matrix4x4 matView;
	Vector3 eye_ = {};//���_���W
	Vector3 target_ = {};//�����_���W
	Vector3 up_= { 0, 1, 0 };//������x�N�g��

private:
	//��ʃT�C�Y
	float Win_width;
	float Win_height;
};

