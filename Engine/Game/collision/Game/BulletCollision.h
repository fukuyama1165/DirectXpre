#pragma once

#include "ICollisionObj.h"
#include "FBXLoader.h"

class BulletCollision:public ICollisionObj
{
public:
	BulletCollision();
	BulletCollision(std::string tag);
	~BulletCollision();

	//������
	void Initialize()override;

	//�I������
	void Finalize()override;

	//���t���[���X�V
	void Update(const Vector3 pos)override;

	//�`��
	void Draw(AnimationModel* model);

	void OnCollision(const CollisionInfo& info);

	void SetIsHit(bool flag) { isHit = flag; };

	bool isHit = false;

private:

};

