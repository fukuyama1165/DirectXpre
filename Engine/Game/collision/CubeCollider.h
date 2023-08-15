#pragma once
#include "BaseCollider.h"
#include "CollisionPrimirive.h"

#include "Vector3.h"

//必ず最小でサイズで1,1,1の立方体を使うこと
class CubeCollider :public BaseCollider, public Cube
{

public:
	CubeCollider(Vector3 Size = {1,1,1}) :size_(Size)
	{

		//球形状をセット
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
	/// 更新
	/// </summary>
	void Update(const Vector3 pos)override;

	void SetSize_(Vector3 Size) { size_ = Size; };

public:

	//サイズ
	Vector3 size_ = { 1,1,1 };
};

