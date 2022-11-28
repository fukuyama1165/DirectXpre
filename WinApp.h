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