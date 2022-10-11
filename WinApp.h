#pragma once

#include<Windows.h>

//WindouwsAPI
class WinApp
{

public:

	WinApp();
	~WinApp();

	//初期化
	void initialize();

	//更新(中身なし)
	void updare();

	//メッセージの処理
	bool processMassage();

	//ウインドウクラスの設定を返す関数
	WNDCLASSEX getW();

	//画面サイズを持っている変数を返す関数
	RECT getWrc();

	//HWNDを返す関数
	HWND getHwnd();

	//ウインドウの横幅を返す関数
	int getWindowSizeWidth();

	//ウインドウの縦幅を返す関数
	int getWindowSizeHeight();

	//静的メンバ関数
public:
	//ウインドウプロシージャ
	static LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:

	const int windowWidth = 1280;
	const int windowHeight = 720;

	WNDCLASSEX w = {};//ウィンドウクラスの設定

	RECT wrc = {};

	HWND hwnd;

	//これを書かないとウィンドウが一瞬で消えるため記述
	MSG msg{};

};

