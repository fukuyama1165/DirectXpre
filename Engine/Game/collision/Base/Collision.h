#pragma once
#include "CollisionPrimirive.h"

/**
 * @file Collision.h
 * @brief 当たり判定の処理が入っているクラス
 * @author フクヤマ
 * @date 2023_12/29
 */

//当たり判定実践クラス
class Collision
{

public:

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="sphereA">球A</param>
	/// <param name="sphereB">球B</param>
	/// <param name="inter">最近接点(中心と中心の距離の半分)</param>
	/// <returns>当たっているか否か</returns>
	static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, Vector3* inter = nullptr);

	/// <summary>
	/// 球と平面の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="plane">平面</param>
	/// <param name="inter"交点(平面上の最近接点)></param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);

	/// <summary>
	/// 点と三角形の最近接点を求める
	/// </summary>
	/// <param name="point">点</param>
	/// <param name="triangle">三角形</param>
	/// <param name="closest">最近接点(出力用)</param>
	static void ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest);

	/// <summary>
	/// 球と法線付き三角形の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">交点(三角形上の最近接点)</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr);

	/// <summary>
	/// 立方体と立方体の当たり判定(AABB)
	/// </summary>
	/// <param name="cube1">立方体</param>
	/// <param name="cube2">立方体</param>
	/// <returns>当たっているか</returns>
	static bool CheckCube2CubeAABB(const Cube& cube1, const Cube& cube2);

	/// <summary>
	/// 立方体と直方体の当たり判定
	/// </summary>
	/// <param name="cube">立方体</param>
	/// <param name="Box">直方体</param>
	/// <returns>あたっているか</returns>
	static bool CheckCube2BoxAABB(const Cube& cube, const Rectangular& Box);

	/// <summary>
	/// 立方体と直方体の当たり判定
	/// </summary>	
	/// <param name="Box">直方体</param>
	/// <param name="cube">立方体</param>
	/// <returns>あたっているか</returns>
	static bool CheckCube2BoxAABB(const Rectangular& Box, const Cube& cube);

	/// <summary>
	/// 直方体と直方体の当たり判定
	/// </summary>
	/// <param name="Box1">直方体</param>
	/// <param name="Box2">直方体</param>
	/// <returns>当たっているか</returns>
	static bool CheckBox2BoxAABB(const Rectangular& Box1, const Rectangular& Box2);

	/// <summary>
	/// レイと平面の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="plane">平面</param>
	/// <param name="distance">距離(出力用)</param>
	/// <param name="inter">交点(出力用)</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// レイと三角形の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="triangle">三角形</param>
	/// <param name="distance">距離(出力用)</param>
	/// <param name="inter">交点(出力用)</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// レイと球の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="sphere">球</param>
	/// <param name="distance">距離(出力用)</param>
	/// <param name="inter">交点(出力用)</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, Vector3* inter = nullptr);

private:

	
};


