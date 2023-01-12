#pragma once
#include "Object3D.h"
#include"Input.h"
#include "cameraObj.h"
class player
{
public:
	player();
	~player();

	void Init(ID3D12Device* dev, const std::string directoryPath, const char filename[]);
	void Update(cameraObj camera);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void Attack();

	bool GetAttackFlag() { return attackFlag; };

	Object3D playerObj;

	Object3D attackObj;

private:

	

	

	Input* input=nullptr;

	Float3 pos = {};

	Float3 moveVec;

	float moveSpeed = 5;

	bool attackFlag = false;

	float attackTime = 3;
};

