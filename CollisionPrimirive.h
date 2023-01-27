#pragma once

#include "Vector4.h"

//球
struct Sphere
{

	//中心座標
	Vector4 center = { 0,0,0,1 };

	//半径
	float radius = 1.0f;

};

//平面
struct Plane
{

	//法線ベクトル
	Vector4 normal = { 0,1,0,0 };

	//原点(0,0,0)からの距離
	float distance = 0.0f;

};

class Triangle
{
public:
	//頂点座標3つ
	Vector4 p0;
	Vector4 p1;
	Vector4 p2;

	//法線ベクトル
	Vector4 normal;

private:

};

struct Ray
{

	//視点座標
	Vector4 start = { 0,0,0,1 };

	//方向
	Vector4 dir = { 1,0,0,0 };

};



class CollisionPrimirive
{
};

