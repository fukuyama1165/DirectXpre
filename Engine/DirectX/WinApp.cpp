#include "WinApp.h"

#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, uint32_t msg, WPARAM wParam, LPARAM lParam);


WinApp* WinApp::GetInstance()
{
	static WinApp instance;
	return &instance;
}

WinApp::~WinApp()
{

	// ウィンドウクラスを登録解除
	UnregisterClass(w_.lpszClassName, w_.hInstance);

}

void WinApp::initialize()
{

#pragma region ウィンドウの初期化部分


	w_.cbSize = sizeof(WNDCLASSEX);
	w_.lpfnWndProc = static_cast<WNDPROC>(windowProc);//ウィンドウプロシージャ
	w_.lpszClassName = L"ハイドシューター";//ウィンドウクラス名
	w_.hInstance = GetModuleHandle(nullptr);//ウィンドウハンドル
	w_.hCursor = LoadCursor(NULL, IDC_ARROW);//カーソル指定

	//ウィンドウクラスをOSに登録
	RegisterClassEx(&w_);

	//ウィンドウのサイズの構造体{x座標,y座標,横幅,縦幅}
	wrc_ = { 0,0,(LONG)SWindowWidth_,(LONG)SWindowHeight_ };
	AdjustWindowRect(&wrc_, WS_OVERLAPPEDWINDOW, false);//自動でサイズ補正

	//ウィンドウの構成要素？
	hwnd_ = CreateWindow(w_.lpszClassName,//クラス名
		L"ハイドシューター",//タイトルバーの名前
		WS_OVERLAPPEDWINDOW,//標準的なウインドウスタイル
		CW_USEDEFAULT,//x座標(OSに任せる)
		CW_USEDEFAULT,//y座標(OSに任せる)
		wrc_.right - wrc_.left,//ウィンドウ横幅
		wrc_.bottom - wrc_.top,//ウィンドウ縦幅
		nullptr,//親ウィンドウハンドル
		nullptr,//メニューハンドル
		w_.hInstance,//呼び出しアプリケーションハンドル
		nullptr);//オプション

	//ウィンドウ表示部分
	ShowWindow(hwnd_, SW_SHOW);

	//システムタイマーの分解能を上げる
	timeBeginPeriod(1);

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

const WNDCLASSEX& WinApp::getW()const
{

	return w_;

}

const RECT& WinApp::getWrc() const
{

	return wrc_;

}

HWND WinApp::getHwnd() const
{

	return hwnd_;

}


uint32_t WinApp::getWindowSizeWidth()
{

	return SWindowWidth_;

}

float WinApp::getWindowSizeWidthF()
{

	return SWindowWidthF_;

}


uint32_t WinApp::getWindowSizeHeight()
{

	return SWindowHeight_;

}

float WinApp::getWindowSizeHeightF()
{

	return SWindowHeightF_;

}

Vector2 WinApp::GetMousePos()
{

	//マウス座標を取得
	POINT p;
	GetCursorPos(&p);

	ScreenToClient(getHwnd(), &p);

	return Vector2((float)p.x, (float)p.y);

}

void WinApp::SetMousePos(int32_t posX, int32_t posY)
{
	int32_t x = 0;
	int32_t y = 0;

	WINDOWINFO windowInfo{};

	//ウィンドウの位置を取得
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(getHwnd(), &windowInfo);

	//マウスの位置をセット(ウインドウの位置を足すことでだいたいウインドウの位置を動かせるように)
	x = posX + windowInfo.rcWindow.left;
	y = posY + windowInfo.rcWindow.top + 35;//タイトルバーが35あるらしいのでプラス

	SetCursorPos(x, y);

}

void WinApp::SetMonitorMousePos(int32_t posX, int32_t posY)
{
	SetCursorPos(posX, posY);
}

#pragma region ウィンドウプロシージャ


LRESULT WinApp::windowProc(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}

	switch (msg)
	{
	case WM_DESTROY://ウィンドウが破棄された
		PostQuitMessage(0);//OSに対して、終わったことを伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//標準の処理をする
}

#pragma endregion