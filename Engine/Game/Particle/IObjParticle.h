#pragma once
#include "FBXLoader.h"
#include "camera.h"

class IObjParticle
{
public:
	virtual ~IObjParticle() {};

	//初期化
	virtual void Initialize() {};

	//終了処理
	virtual void Finalize() {};

	//毎フレーム更新
	virtual void Update(const Camera& camera) = 0;

	//描画
	virtual void Draw(AnimationModel* model) = 0;

	

	virtual void SetliveTime(float time) =0;

	virtual float GetliveTime() = 0;

	virtual void SetobjColor(Vector4 color) =0;



};