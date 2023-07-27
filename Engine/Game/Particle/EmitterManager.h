#pragma once
#include "BasicEmitter.h"

class EmitterManager
{
public:
	static EmitterManager* GetInstance();

	void AddEmitter(const Vector3& pos, float ActiveTime = -1);

	//���t���[���X�V
	void Update(const Camera& camera);

	//�`��
	void Draw();

private:

	EmitterManager() = default;
	~EmitterManager();

	EmitterManager(const EmitterManager&) = delete;
	EmitterManager& operator=(const EmitterManager&) = delete;


private:
	std::list<std::unique_ptr<BasicEmitter>> emitters_;

};

