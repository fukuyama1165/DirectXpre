#pragma once

#include<Windows.h>
#include <cstdint>
#include "Vector2.h"

//WindouwsAPI
class WinApp
{

public:

	static WinApp* GetInstance();

	//初期化
	void initialize();



	//メッセージの処理
	bool processMassage();

	//ウインドウクラスの設定を返す関数
	//構造体のgetterは下のようにする(書き換えを不可にしつつここにある構造体を見に来れるようにしている)
	const WNDCLASSEX& getW() const;

	//画面サイズを持っている変数を返す関数
	const RECT& getWrc() const;

	//HWNDを返す関数
	HWND getHwnd() const;

	//ウインドウの横幅を返す関数
	uint32_t getWindowSizeWidth();

	//ウインドウの縦幅を返す関数
	uint32_t getWindowSizeHeight();

	Vector2 GetWindowCenter() { return Vector2((float)SWindowWidth_ / 2, (float)SWindowHeight_ / 2); };

	//マウスの位置取得
	Vector2 GetMousePos();

	//マウスの位置をセット(ウインドウの位置 左上が(0,0))
	void SetMousePos(int32_t posX,int32_t posY);

	//マウスの位置をセット(モニターの位置 左上が(0,0))
	void SetMonitorMousePos(int32_t posX, int32_t posY);

	//静的メンバ関数
public:
	//ウインドウプロシージャ
	static LRESULT windowProc(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam);

private:

	WinApp() = default;
	~WinApp();

	WinApp(const WinApp&) = delete;
	WinApp& operator=(const WinApp&) = delete;

public:

	static const uint32_t SWindowWidth_ = 1280;
	static const uint32_t SWindowHeight_ = 720;

private:

	

	WNDCLASSEX w_ = {};//ウィンドウクラスの設定

	RECT wrc_ = {};

	HWND hwnd_;

	//これを書かないとウィンドウが一瞬で消えるため記述
	MSG msg{};

};