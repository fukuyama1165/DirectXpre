#include "WinApp.h"
#pragma comment(lib,"winmm.lib")


WinApp::WinApp()
{

}
WinApp::~WinApp()
{

	// ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);

}

void WinApp::initialize()
{

#pragma region ウィンドウの初期化部分
	

	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = static_cast<WNDPROC>(windowProc);//ウィンドウプロシージャ
	w.lpszClassName = L"DirectXGame";//ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr);//ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW);//カーソル指定

	//ウィンドウクラスをOSに登録
	RegisterClassEx(&w);

	//ウィンドウのサイズの構造体{x座標,y座標,横幅,縦幅}
	wrc = { 0,0,windowWidth,windowHeight };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//自動でサイズ補正

	//ウィンドウの構成要素？
	hwnd = CreateWindow(w.lpszClassName,//クラス名
		L"DirectXGame",//タイトルバーの名前
		WS_OVERLAPPEDWINDOW,//標準的なウインドウスタイル
		CW_USEDEFAULT,//x座標(OSに任せる)
		CW_USEDEFAULT,//y座標(OSに任せる)
		wrc.right - wrc.left,//ウィンドウ横幅
		wrc.bottom - wrc.top,//ウィンドウ縦幅
		nullptr,//親ウィンドウハンドル
		nullptr,//メニューハンドル
		w.hInstance,//呼び出しアプリケーションハンドル
		nullptr);//オプション

	//ウィンドウ表示部分
	ShowWindow(hwnd, SW_SHOW);

	//システムタイマーの分解能を上げる
	timeBeginPeriod(1);

}


void WinApp::updare()
{

}

bool WinApp::processMassage()
{

	//メッセージがある？
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);//キー入力メッセージの処理
		DispatchMessage(&msg);//プロシージャにメッセージを送る
	}

	//×ボタンで終了メッセージが来たらゲームループを抜ける
	if (msg.message == WM_QUIT)
	{
		return true;
	}

	return false;

}

WNDCLASSEX WinApp::getW()
{

	return w;

}

RECT WinApp::getWrc()
{

	return wrc;

}

HWND WinApp::getHwnd()
{

	return hwnd;

}


int WinApp::getWindowSizeWidth()
{

	return windowWidth;

}


int WinApp::getWindowSizeHeight()
{

	return windowHeight;

}

#pragma region ウィンドウプロシージャ


LRESULT WinApp::windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY://ウィンドウが破棄された
		PostQuitMessage(0);//OSに対して、終わったことを伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//標準の処理をする
}

#pragma endregion