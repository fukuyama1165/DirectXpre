#include "Collision.h"
#include <cmath>

//float abs(float a)
//{
//	if (a < 0)
//	{
//		a = -a;
//	}
//
//	return a;
//}


bool Collision::CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, Vector3* inter)
{

	float x = sphereB.center_.x - sphereA.center_.x;
	float y = sphereB.center_.y - sphereA.center_.y;
	float z = sphereB.center_.z - sphereA.center_.z;
	float r = sphereA.radius_ + sphereB.radius_;

	//�����ɊY�����ĂȂ��Ȃ瓖�����ĂȂ�
	if ((x * x) + (y * y) + (z * z) > (r * r))
	{
		return false;
	}

	if (inter)
	{
		//�ړ_�͒��S�ƒ��S�̃x�N�g���̔����i�񂾋����ɂ���
		Vector3 vec = sphereB.center_ - sphereA.center_;

		*inter = sphereA.center_ + vec / 2;

	}

	//����������������
	return true;

}

bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter)
{

	float distV = Vector3::dot(sphere.center_, plane.normal_);

	float dist = distV - plane.distance_;

	if (abs(dist) > sphere.radius_) return false;

	if (inter)
	{

		*inter = -dist * plane.normal_ + sphere.center_;

	}

	return true;
	
}

void Collision::ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest)
{

	//point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N

#pragma region p0�`�F�b�N

	//triangle��p0��p1�̃x�N�g��
	Vector3 p0p1 = triangle.p1_ - triangle.p0_;

	//triangle��p0��p2�̃x�N�g��
	Vector3 p0p2 = triangle.p2_ - triangle.p0_;

	//triangle��p0��Point�̃x�N�g��
	Vector3 p0Po = point - triangle.p0_;

	//triangle��p0��p1�̃x�N�g����triangle��p0��Point�̃x�N�g���̓���
	float d1 = Vector3::dot(p0p1, p0Po);

	//triangle��p0��p2�̃x�N�g����triangle��p0��Point�̃x�N�g���̓���
	float d2 = Vector3::dot(p0p2, p0Po);

	//2�̓��ς����̐��Ȃ�p0����ԋ߂�
	if (d1 <= 0.0f and d2 <= 0.0f)
	{

		*closest = triangle.p0_;
		return;

	}

#pragma endregion

	//point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N

#pragma region p1�`�F�b�N

	//triangle��p1��Point�̃x�N�g��
	Vector3 p1Po = point - triangle.p1_;

	//triangle��p0��p1�̃x�N�g����triangle��p1��Point�̃x�N�g���̓���
	float d3 = Vector3::dot(p0p1, p1Po);

	//triangle��p0��p2�̃x�N�g����triangle��p1��Point�̃x�N�g���̓���
	float d4 = Vector3::dot(p0p2, p1Po);


	
	if (d3 >= 0.0f and d4 <= d3)
	{

		*closest = triangle.p1_;
		return;

	}


	// point��p0p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0p1��ɑ΂���ˉe��Ԃ�

	//�̈���`�F�b�N�̂��߂̌v�Z���ꂪ���̐��Ȃ�O�p�`�̊O�ɂ���
	float p0p1Projection = d1 * d4 - d3 * d2;

	if (p0p1Projection <= 0.0f and d1 >= 0.0f and d3 <= 0.0f)
	{
		*closest = triangle.p0_ + ((d1 / (d1 - d3)) * p0p1);
		return;
	}

#pragma endregion

	//point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N

#pragma region p2�`�F�b�N

	//triangle��p2��Point�̃x�N�g��
	Vector3 p2Po = point - triangle.p2_;

	//triangle��p0��p1�̃x�N�g����triangle��p2��Point�̃x�N�g���̓���
	float d5 = Vector3::dot(p0p1, p2Po);

	//triangle��p0��p2�̃x�N�g����triangle��p2��Point�̃x�N�g���̓���
	float d6 = Vector3::dot(p0p2, p2Po);


	
	if (d6 >= 0.0f and d5 <= d6)
	{

		*closest = triangle.p2_;
		return;

	}


	// point��p0p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0p2��ɑ΂���ˉe��Ԃ�

	//�̈���`�F�b�N�̂��߂̌v�Z���ꂪ���̐��Ȃ�O�p�`�̊O�ɂ���
	float p0p2Projection = d2 * d5 - d6 * d1;

	if (p0p2Projection <= 0.0f and d2 >= 0.0f and d6 <= 0.0f)
	{
		*closest = triangle.p0_ + ((d2 / (d2 - d6)) * p0p2);
		return;
	}

	// point��p1p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1p2��ɑ΂���ˉe��Ԃ�

	//�̈���`�F�b�N�̂��߂̌v�Z���ꂪ���̐��Ȃ�O�p�`�̊O�ɂ���
	float p1p2Projection = d3 * d6 - d5 * d4;

	if (p1p2Projection <= 0.0f and (d4-d3) >= 0.0f and (d5-d6) >= 0.0f)
	{

		*closest = triangle.p1_ + (((d4 - d3) / ((d4 - d3) + (d5 - d6))) * (triangle.p2_ -triangle.p1_));
		return;

	}

#pragma endregion

	//�O�p�`�̒��ɂ���ꍇ
	float denom = 1.0f / (p1p2Projection + p0p2Projection + p0p1Projection);
	float v = p0p2Projection * denom;
	float w = p0p1Projection * denom;

	*closest = triangle.p0_ + p0p1 * v + p0p2 * w;

}

bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter)
{

	Vector3 p;

	Collision::ClosestPtPoint2Triangle(sphere.center_, triangle, &p);

	Vector3 v = p - sphere.center_;

	float vDot = Vector3::dot(v,v);

	if (vDot > sphere.radius_ * sphere.radius_)
	{
		return false;
	}

	if (inter)
	{
		*inter = p;
	}

	return true;
}

bool Collision::CheckCube2CubeAABB(const Cube& cube1, const Cube& cube2)
{

	Vector3 min1 = cube1.center_ - cube1.size_;
	Vector3 max1 = cube1.center_ + cube1.size_;

	Vector3 min2 = cube2.center_ - cube2.size_;
	Vector3 max2 = cube2.center_ + cube2.size_;

	//��������T�C�Y���̈ʒu���ɂ����Е������邩
	if ((min1 <= max2) and (max1 >= min2))
	{
		//�������Ă܂���
		return true;
	}

	//�������ĂȂ���
	return false;

}

bool Collision::CheckCube2BoxAABB(const Cube& cube, const Rectangular& Box)
{
	Vector3 min1 = cube.center_ - cube.size_ / 2;
	Vector3 max1 = cube.center_ + cube.size_ / 2;

	Vector3 min2 = Box.center_ - Box.sizeMin_;
	Vector3 max2 = Box.center_ + Box.sizeMax_;

	//��������T�C�Y���̈ʒu���ɂ����Е������邩
	if ((min1 <= max2) and (max1 >= min2))
	{
		//�������Ă܂���
		return true;
	}

	//�������ĂȂ���
	return false;
}

bool Collision::CheckCube2BoxAABB(const Rectangular& Box, const Cube& cube)
{
	return CheckCube2BoxAABB(cube, Box);
}

bool Collision::CheckBox2BoxAABB(const Rectangular& Box1, const Rectangular& Box2)
{
	Vector3 min1 = Box1.center_ - Box1.sizeMin_;
	Vector3 max1 = Box1.center_ + Box1.sizeMax_;

	Vector3 min2 = Box2.center_ - Box2.sizeMin_;
	Vector3 max2 = Box2.center_ + Box2.sizeMax_;

	//��������T�C�Y���̈ʒu���ɂ����Е������邩
	if ((min1 <= max2) and (max1 >= min2))
	{
		//�������Ă܂���
		return true;
	}

	//�������ĂȂ���
	return false;
}

bool Collision::CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance, Vector3* inter)
{

	//�덷���B�邽�߂̂��
	const float epsilon = 1.0e-5f;

	//����
	float d1 = Vector3::dot(plane.normal_, ray.dir_);

	//�����������Ȃ̂œ������ĂȂ�
	if (d1 > -epsilon)
	{
		return false;
	}

	//�ˉe�����߂�
	float d2 = Vector3::dot(plane.normal_, ray.start_);

	//�n�_�ƕ��ʂ̋���
	float dist = d2 - plane.distance_;

	float t = dist / -d1/*(-Vector3::dot(plane.normal, ray.dir))*/;

	if (t < 0)
	{
		return false;
	}

	if (distance != nullptr)
	{

		*distance = t;

	}

	if (inter != nullptr)
	{
		*inter = ray.start_ + t * ray.dir_;
	}

	return true;


}

bool Collision::CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance, Vector3* inter)
{

	//�܂��O�p�`�������镽�ʂƂ̓����蔻����Ƃ�
	Plane plane;

	Vector3 planeInter;

	//�@���͎O�p�`�Ɠ���
	plane.normal_ = triangle.normal_;

	//�����͑�\�̈ʒu�x�N�g���Ɩ@���ŋ��߂���
	plane.distance_ = Vector3::dot(triangle.normal_,triangle.p0_);

	//���ʂɂ��瓖�����Ă��Ȃ��Ȃ炻�������������ĂȂ�
	if (!CheckRay2Plane(ray, plane, distance, &planeInter))
	{
		return false;
	}

	//�덷���B�邽�߂̂��
	const float epsilon = 1.0e-5f;

	Vector3 p0p1 = triangle.p1_ - triangle.p0_;
	Vector3 interp0 = triangle.p0_ - planeInter;

	Vector3 m0 = Vector3::cross(interp0, p0p1);

	//�����Ȃ�@���Ɠ��������������Ă���̂ŕʂ̕����Ȃ瓖�����ĂȂ�
	if (Vector3::dot(m0, triangle.normal_) <  -epsilon)
	{
		return false;
	}

	Vector3 p1p2 = triangle.p2_ - triangle.p1_;
	Vector3 interp1 = triangle.p1_ - planeInter;

	Vector3 m1 = Vector3::cross(interp1, p1p2);

	//�@���Ƌt�����������Ă���Ȃ瓖�����ĂȂ�
	if (Vector3::dot(m1, triangle.normal_) < -epsilon)
	{
		return false;
	}

	Vector3 p2p0 = triangle.p0_ - triangle.p2_;
	Vector3 interp2 = triangle.p2_ - planeInter;

	Vector3 m2 = Vector3::cross(interp2, p2p0);

	//�@���Ƌt�����������Ă���Ȃ瓖�����ĂȂ�
	if (Vector3::dot(m2, triangle.normal_) < -epsilon)
	{
		return false;
	}

	if (inter != nullptr)
	{

		*inter = planeInter;

	}

	return true;

}

bool Collision::CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance, Vector3* inter)
{

	//���̒��S���烌�C�̎n�_�ւ̃x�N�g�������
	Vector3 m = ray.start_ - sphere.center_;
	float b = Vector3::dot(m, ray.dir_);
	float c = Vector3::dot(m, m) - (sphere.radius_ * sphere.radius_);

	//ray�̎n�_��sphere�̊O���ɂ���(c>0)�A
	//ray��sphere���痣��Ă����������w���Ă���ꍇ(b>0)�A
	//������Ȃ�
	if (c > 0.0f and b > 0.0f)
	{

		return false;

	}

	float d = (b * b) - c;

	//��_�����݂��Ȃ��̂œ������ĂȂ�
	if (d < 0.0f)
	{
		return false;
	}

	//��_�̈�ԋ߂��Ƃ�����v�Z(���ł͂Ȃ��Ȃ�2���邽��)
	float t = -b - sqrtf(d);

	//t�����̐��Ȃ烌�C�����̓����ɂ���炵���̂�0�ɂ���
	if (t < 0.0f)
	{
		t = 0.0f;
	}

	if (distance != nullptr)
	{

		*distance = t;

	}

	if (inter != nullptr)
	{

		*inter = ray.start_ + t * ray.dir_;

	}

	return true;

}