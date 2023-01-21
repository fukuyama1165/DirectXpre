#include "Collison.h"

float abs(float a)
{
	if (a < 0)
	{
		a = -a;
	}

	return a;
}


bool Collison::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector4* inter)
{

	float distV = Vector4::dot(sphere.center, plane.normal);

	float dist = distV - plane.distance;

	if (abs(dist) > sphere.radius) return false;

	if (inter)
	{

		*inter = -dist * plane.normal + sphere.center;

	}

	return true;
	
}

void Collison::ClosestPtPoint2Triangle(const Vector4& point, const Triangle& triangle, Vector4* closest)
{

	//point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
#pragma region p0�`�F�b�N

	//triangle��p0��p1�̃x�N�g��
	Vector4 p0p1 = triangle.p1 - triangle.p0;

	//triangle��p0��p2�̃x�N�g��
	Vector4 p0p2 = triangle.p2 - triangle.p0;

	//triangle��p0��Point�̃x�N�g��
	Vector4 p0Po = point - triangle.p0;

	//triangle��p0��p1�̃x�N�g����triangle��p0��Point�̃x�N�g���̓���
	float d1 = Vector4::dot(p0p1, p0Po);

	//triangle��p0��p2�̃x�N�g����triangle��p0��Point�̃x�N�g���̓���
	float d2 = Vector4::dot(p0p2, p0Po);

	//2�̓��ς����̐��Ȃ�p0����ԋ߂�
	if (d1 <= 0.0f and d2 <= 0.0f)
	{

		*closest = triangle.p0;
		return;

	}

#pragma endregion

#pragma region p1�`�F�b�N

	//point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	
	//triangle��p1��Point�̃x�N�g��
	Vector4 p1Po = point - triangle.p1;

	//triangle��p0��p1�̃x�N�g����triangle��p1��Point�̃x�N�g���̓���
	float d3 = Vector4::dot(p0p1, p1Po);

	//triangle��p0��p2�̃x�N�g����triangle��p1��Point�̃x�N�g���̓���
	float d4 = Vector4::dot(p0p2, p1Po);


	
	if (d3 >= 0.0f and d4 <= d3)
	{

		*closest = triangle.p1;
		return;

	}


	// point��p0p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0p1��ɑ΂���ˉe��Ԃ�

	//�̈���`�F�b�N�̂��߂̌v�Z���ꂪ���̐��Ȃ�O�p�`�̊O�ɂ���
	float p0p1Projection = d1 * d4 - d3 * d2;

	if (p0p1Projection <= 0.0f and d1 >= 0.0f and d3 <= 0.0f)
	{
		*closest = triangle.p0 + ((d1 / (d1 - d3)) * p0p1);
		return;
	}

#pragma endregion

	//point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N

#pragma region p2�`�F�b�N

	//triangle��p2��Point�̃x�N�g��
	Vector4 p2Po = point - triangle.p2;

	//triangle��p0��p1�̃x�N�g����triangle��p2��Point�̃x�N�g���̓���
	float d5 = Vector4::dot(p0p1, p2Po);

	//triangle��p0��p2�̃x�N�g����triangle��p2��Point�̃x�N�g���̓���
	float d6 = Vector4::dot(p0p2, p2Po);


	
	if (d6 >= 0.0f and d5 <= d6)
	{

		*closest = triangle.p2;
		return;

	}


	// point��p0p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0p2��ɑ΂���ˉe��Ԃ�

	//�̈���`�F�b�N�̂��߂̌v�Z���ꂪ���̐��Ȃ�O�p�`�̊O�ɂ���
	float p0p2Projection = d2 * d5 - d6 * d1;

	if (p0p2Projection <= 0.0f and d2 >= 0.0f and d6 <= 0.0f)
	{
		*closest = triangle.p0 + ((d2 / (d2 - d6)) * p0p2);
		return;
	}

	// point��p1p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1p2��ɑ΂���ˉe��Ԃ�

	//�̈���`�F�b�N�̂��߂̌v�Z���ꂪ���̐��Ȃ�O�p�`�̊O�ɂ���
	float p1p2Projection = d3 * d6 - d5 * d4;

	if (p1p2Projection <= 0.0f and (d4-d3) >= 0.0f and (d5-d6) >= 0.0f)
	{

		*closest = triangle.p1 + (((d4 - d3) / ((d4 - d3) + (d5 - d6))) * (triangle.p2-triangle.p1));
		return;

	}

#pragma endregion

	//�O�p�`�̒��ɂ���ꍇ
	float denom = 1.0f / (p1p2Projection + p0p2Projection + p0p1Projection);
	float v = p0p2Projection * denom;
	float w = p0p1Projection * denom;

	*closest = triangle.p0 + p0p1 * v + p0p2 * w;

}

bool Collison::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector4* inter)
{

	Vector4 p;

	Collison::ClosestPtPoint2Triangle(sphere.center, triangle, &p);

	Vector4 v = p - sphere.center;

	float vDot = Vector4::dot(v,v);

	if (vDot > sphere.radius * sphere.radius)
	{
		return false;
	}

	if (inter)
	{
		*inter = p;
	}

	return true;
}
