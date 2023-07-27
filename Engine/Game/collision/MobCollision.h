#pragma once

#include "ICollisionObj.h"
#include "FBXLoader.h"

class MobCollision :public ICollisionObj
{
public:
	MobCollision();
	MobCollision(std::string tag);
	~MobCollision();

	//������
	void Initialize()override;

	//�I������
	void Finalize()override;

	//���t���[���X�V
	void Update(const Camera& camera, const Vector3 pos)override;

	//�`��
	void Draw(AnimationModel* model);

	void OnCollision(const CollisionInfo& info);

	bool isHit = false;

private:

};