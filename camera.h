#pragma once
#include "matrix4x4.h"
#include "Float3.h"
#include "Float4.h"
#include <math.h>


class Camera
{
public:
	Camera(float win_width,float win_height);
	~Camera();

	void upDate();

private:
	Matrix4x4 matViewGeneration(Float3 eye, Float3 target, Float3 up);

	Matrix4x4 perspectiveProjectionGeneration(float FovAngleY, float NearZ, float FarZ);

	/// <summary>
	/// sin��cos��Ԃ��֐�
	/// </summary>
	/// <param name="Sin">sin�̒l���������</param>
	/// <param name="Cos">cos�̒l���������</param>
	/// <param name="angle">���̒l</param>
	void sinCos(float& Sin, float& Cos, float angle);

	Float3 float3Dat(Float3 A, Float3 B);

public:


	//�������e�s��
	Matrix4x4 matProjection;

	//�r���[�ϊ��s��
	Matrix4x4 matView;
	Float3 eye_ = {};//���_���W
	Float3 target_ = {};//�����_���W
	Float3 up_= { 0, 1, 0 };//������x�N�g��

private:
	//��ʃT�C�Y
	float Win_width;
	float Win_height;
};

