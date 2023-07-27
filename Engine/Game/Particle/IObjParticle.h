#pragma once
#include "FBXLoader.h"
#include "camera.h"

class IObjParticle
{
public:
	virtual ~IObjParticle() {};

	//������
	virtual void Initialize() {};

	//�I������
	virtual void Finalize() {};

	//���t���[���X�V
	virtual void Update(const Camera& camera) = 0;

	//�`��
	virtual void Draw(AnimationModel* model) = 0;

	

	virtual void SetliveTime(float time) =0;

	virtual float GetliveTime() = 0;

	virtual void SetobjColor(Vector4 color) =0;



};