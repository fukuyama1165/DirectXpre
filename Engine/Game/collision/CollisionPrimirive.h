#pragma once

#include "Vector3.h"

//球
struct Sphere
{

	//中心座標
	Vector3 center_ = { 0,0,0};

	//半径
	float radius_ = 1.0f;

};

//無限平面
struct Plane
{

	//法線ベクトル
	Vector3 normal_ = { 0,1,0 };

	//原点(0,0,0)からの距離
	float distance_ = 0.0f;

};

//三角形
class Triangle
{
public:
	//頂点座標3つ
	Vector3 p0_;
	Vector3 p1_;
	Vector3 p2_;

	//法線ベクトル
	Vector3 normal_;

	//法線ベクトルの算出
	void ComputeNormal();

private:

};

//レイ
struct Ray
{

	//視点座標
	Vector3 start_ = { 0,0,0};

	//方向
	Vector3 dir_ = { 1,0,0};

};

//立方体
struct Cube
{
	//中心座標
	Vector3 center_ = { 0,0,0 };

	//中央からの距離
	Vector3 size_ = { 1,1,1 };
};

//直方体
struct Rectangular
{
	//中心座標
	Vector3 center_ = { 0,0,0 };

	//中央からの最小距離
	Vector3 sizeMin_ = { 1,1,1 };

	//中央からの最大距離
	Vector3 sizeMax_ = { 1,1,1 };
};


//一応で用意したやつ
class CollisionPrimirive
{
};

