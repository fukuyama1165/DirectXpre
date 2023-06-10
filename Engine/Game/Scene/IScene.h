#pragma once
#include <memory>

class IScene
{
public:

	virtual ~IScene() = default;
	
	//初期化
	virtual void Initialize();

	//終了処理
	virtual void Finalize();

	//毎フレーム更新
	virtual void Update();

	//描画
	virtual void Draw() = 0;

	//終了チェック
	virtual bool IsEndRequst() { return endRequst_; };

	//シーンの一連を実行
	void Ran();


	//ゲームを終わらせるためのフラグ
	bool endRequst_ = false;

private:

	

};
