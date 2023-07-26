#pragma once
#include "BasicEmitter.h"

class EmitterManager
{
public:
	static EmitterManager* GetInstance();

	void AddEmitter(const Vector3& pos, float ActiveTime = -1);

	//毎フレーム更新
	void Update(const Camera& camera);

	//描画
	void Draw();

private:

	EmitterManager() = default;
	~EmitterManager();

	EmitterManager(const EmitterManager&) = delete;
	EmitterManager& operator=(const EmitterManager&) = delete;


private:
	std::list<std::unique_ptr<BasicEmitter>> emitters_;

};

