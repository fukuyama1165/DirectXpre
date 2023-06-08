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

//コントローラーを使うならこれが簡単らしい
#include <Xinput.h>
#pragma comment(lib,"Xinput.lib")

class Input
{
public:
	static Input* GetInstance();

	void init(WNDCLASSEX w, HWND hwnd);

	void update();

	//キーボードのキーを押したら反応する関数(長押しも反応)
	bool PushKey(BYTE CheckKey);

	//キーボードのキーを押した瞬間に反応する関数(長押し反応しない)
	bool TriggerKey(BYTE CheckKey);


	void instanceDelete();
private:
	Input() = default;
	~Input();

	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

	static Input* instance_;

	HRESULT result_ =S_OK;

	//全キーの入力情報を取得する為の変数
	static BYTE key_[256];
	static BYTE oldKey_[256];

	//キーボードデバイス
	IDirectInputDevice8* keyboard_ = nullptr;

};