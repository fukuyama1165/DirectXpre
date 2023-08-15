#pragma once

#include "Vector3.h"

//��
struct Sphere
{

	//���S���W
	Vector3 center_ = { 0,0,0};

	//���a
	float radius_ = 1.0f;

};

//��������
struct Plane
{

	//�@���x�N�g��
	Vector3 normal_ = { 0,1,0 };

	//���_(0,0,0)����̋���
	float distance_ = 0.0f;

};

//�O�p�`
class Triangle
{
public:
	//���_���W3��
	Vector3 p0_;
	Vector3 p1_;
	Vector3 p2_;

	//�@���x�N�g��
	Vector3 normal_;

	//�@���x�N�g���̎Z�o
	void ComputeNormal();

private:

};

//���C
struct Ray
{

	//���_���W
	Vector3 start_ = { 0,0,0};

	//����
	Vector3 dir_ = { 1,0,0};

};

//������
struct Cube
{
	//���S���W
	Vector3 center_ = { 0,0,0 };

	//��������̋���
	Vector3 size_ = { 1,1,1 };
};

//������
struct Rectangular
{
	//���S���W
	Vector3 center_ = { 0,0,0 };

	//��������̍ŏ�����
	Vector3 sizeMin_ = { 1,1,1 };

	//��������̍ő勗��
	Vector3 sizeMax_ = { 1,1,1 };
};


//�ꉞ�ŗp�ӂ������
class CollisionPrimirive
{
};

