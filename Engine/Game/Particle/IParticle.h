#pragma once

class IParticle
{
public:
	virtual ~IParticle() {};

	//初期化
	virtual void Initialize() {};

	//終了処理
	virtual void Finalize() {};

	//毎フレーム更新
	virtual void Update() {};

	//描画
	virtual void Draw() {};
};

