#pragma once
#include "ICollisionObj.h"
#include "Vector3.h"
#include "FBXLoader.h"

class BulletCollision:public ICollisionObj
{
public:
	BulletCollision(std::string tag="");
	~BulletCollision();

	//������
	void Initialize();

	//�I������
	void Finalize();

	//���t���[���X�V
	void Update(const Camera& camera,const Vector3 pos);

	//�`��
	void Draw(AnimationModel* model);

	void OnCollision(const CollisionInfo& info);

	Object3D collisionObj;

	bool isDraw = false;

	bool isHit = false;

private:

};

