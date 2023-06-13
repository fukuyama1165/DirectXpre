#include "Input.h"
#include "WinApp.h"

BYTE Input::key_[256] = {};
BYTE Input::oldKey_[256] = {};

//Input* Input::instance_ = nullptr;

Input::~Input()
{

}

void Input::instanceDelete()
{
	//delete(instance_);
}

Input* Input::GetInstance()
{
	
	static Input instance;
	return &instance;
}

void Input::init()
{

#pragma region DirectInoutオブジェクトの生成

	//DirectInputの初期化
	IDirectInput8* directInput = nullptr;
	result_ = DirectInput8Create(
		WinApp::GetInstance()->getW().hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&directInput,
		nullptr
	);
	assert(SUCCEEDED(result_));

#pragma endregion 全体の初期化であり入力デバイスが増えてもこのオブジェクトは増やさなくてもいい

#pragma region キーボードデバイスの生成

	//キーボードデバイスの生成

	result_ = directInput->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(result_));

#pragma endregion

#pragma region ゲームパッドの入力

	//DIDEVICEINSTANCE* gamePadDID = {};
	//result = directInput->EnumDevices(
	//	DI8DEVTYPE_JOYSTICK,//JOYSTICKだと認識しないことがあるらしい(確認できるなら確認すること)
	//	DeviceFindCallBack,
	//	&gamePadDID,
	//	DIEDFL_ATTACHEDONLY
	//);
	//assert(SUCCEEDED(result));

	//IDirectInputDevice8* gamePadDirct = nullptr;
	//result = directInput->CreateDevice(gamePadDID->guidInstance, &gamePadDirct, NULL);
	//assert(SUCCEEDED(result));


#pragma endregion(中止)

#pragma region 入力データ形式のセット

	result_ = keyboard_->SetDataFormat(&c_dfDIKeyboard);//標準形式
	assert(SUCCEEDED(result_));

#pragma endregion

#pragma region 排他制御レベルのセット

	result_ = keyboard_->SetCooperativeLevel(
		WinApp::GetInstance()->getHwnd(),
		DISCL_FOREGROUND/*画面が手前にある場合のみ入力を受け付ける*/ |
		DISCL_NONEXCLUSIVE/*デバイスをこのアプリだけで占有しない*/ |
		DISCL_NOWINKEY/*windowキーを無効にする*/);
	assert(SUCCEEDED(result_));

#pragma endregion

}

void Input::update()
{

#pragma region キーボード情報の取得

	//前フレームのキーボード入力を保存
	for (uint16_t i = 0; i < 256; i++)
	{
		oldKey_[i] = key_[i];
	}

	//キーボード情報の取得開始
	keyboard_->Acquire();

	//全キーの入力情報を取得する
	keyboard_->GetDeviceState(sizeof(key_), key_);

#pragma endregion

}

bool Input::PushKey(const BYTE& CheckKey)
{
	return key_[CheckKey];
}

bool Input::TriggerKey(const BYTE& CheckKey)
{
	return key_[CheckKey] and oldKey_[CheckKey] == 0;
}