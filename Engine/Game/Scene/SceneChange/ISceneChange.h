#pragma once

class ISceneChange
{
public:
	virtual ~ISceneChange() {};

	//初期化
	virtual void Initialize() {};

	//終了処理
	virtual void Finalize() {};

	//毎フレーム更新
	virtual void Update() {};

	//描画
	virtual void Draw() {};

	virtual bool GetIsStart() { return isStart_; };
	virtual void SetIsStart(bool flag) { isStart_ = flag; };

	virtual bool GetIsClose() { return isClose_; };
	virtual void SetIsClose(bool flag) { isClose_ = flag; };

	virtual bool GetIsOpenStart() { return isOpenStart_; };
	virtual void SetIsOpenStart(bool flag) { isOpenStart_ = flag; };

	virtual bool GetIsEnd() { return isEnd_; };
	virtual void SetIsEnd(bool flag) { isEnd_ = flag; };

private:

	bool isStart_ = false;

	bool isClose_ = false;

	bool isOpenStart_ = false;

	bool isEnd_ = false;

};

