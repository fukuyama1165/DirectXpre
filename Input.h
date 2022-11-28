#pragma once
//D3Dコンパイラ
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include<vector>
#include<string>

#include<d3d12.h>
#include<dxgi1_6.h>

//assertを使うために必要
#include <cassert>


//#pragma commentとは、オブジェクトファイルに、
//リンカでリンクするライブラリの名前を記述するもの
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")


//キーボードやコントローラーなどの入力するヘッダとライブラリのリンク
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")


class Input
{
public:
	void init(WNDCLASSEX w, HWND hwnd);

	void update();

	//キーボードのキーを押したら反応する関数(長押しも反応)
	bool PushKey(BYTE CheckKey);

	//キーボードのキーを押した瞬間に反応する関数(長押し反応しない)
	bool TriggerKey(BYTE CheckKey);

private:
	HRESULT result;

	//全キーの入力情報を取得する為の変数
	static BYTE key[256];
	static BYTE oldKey[256];

	//キーボードデバイス
	IDirectInputDevice8* keyboard = nullptr;

};