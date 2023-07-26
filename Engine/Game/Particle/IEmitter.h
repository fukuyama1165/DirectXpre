#pragma once

class IEmitter
{
public:
	virtual ~IEmitter() {};

	//初期化
	virtual void Initialize() {};

	//終了処理
	virtual void Finalize() {};

	//毎フレーム更新
	virtual void Update() {};

	//描画
	virtual void Draw() {};
};