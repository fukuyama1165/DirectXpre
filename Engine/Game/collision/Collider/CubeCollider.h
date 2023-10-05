#pragma once
#include "BaseCollider.h"
#include "CollisionPrimirive.h"

#include "Vector3.h"

//�K���ŏ��ŃT�C�Y��1,1,1�̗����̂��g������
class CubeCollider :public BaseCollider, public Cube
{

public:
	CubeCollider(Vector3 Size = {1,1,1}) :size_(Size)
	{

		//���`����Z�b�g
		shapeType_ = COLLISIONSHAPE_CUBE;


	}

	~CubeCollider()
	{
		if (collisionObject_ != nullptr)
		{
			collisionObject_->Finalize();
		}
	}

	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(const Vector3 pos)override;

	void SetSize_(Vector3 Size) { size_ = Size; };

public:

	//�T�C�Y
	Vector3 size_ = { 1,1,1 };
};

