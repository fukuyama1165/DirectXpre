#pragma once

#include "Vector4.h"

//��
struct Sphere
{

	//���S���W
	Vector4 center = { 0,0,0,1 };

	//���a
	float radius = 1.0f;

};

//����
struct Plane
{

	//�@���x�N�g��
	Vector4 normal = { 0,1,0,0 };

	//���_(0,0,0)����̋���
	float distance = 0.0f;

};

class Triangle
{
public:
	//���_���W3��
	Vector4 p0;
	Vector4 p1;
	Vector4 p2;

	//�@���x�N�g��
	Vector4 normal;

private:

};

struct Ray
{

	//���_���W
	Vector4 start = { 0,0,0,1 };

	//����
	Vector4 dir = { 1,0,0,0 };

};



class CollisionPrimirive
{
};

