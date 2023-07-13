#pragma once
//D3Dコンパイラ
#include<d3dcompiler.h>

#include<vector>
#include<string>

#include<d3d12.h>
#include<dxgi1_6.h>

//assertを使うために必要
#include <cassert>
//ComPtr用インクルード
#include <wrl.h>

#include <cstdint>

//キーボードやコントローラーなどの入力するヘッダとライブラリのリンク
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

//コントローラーを使うならこれが簡単らしい
#include <Xinput.h>

#include "Vector2.h"
#include "Vector3.h"

class Input
{
public:
	static Input* GetInstance();

	void init();

	void update();

	//キーボードのキーを押したら反応する関数(長押しも反応)
	bool PushKey(const BYTE& CheckKey);

	//キーボードのキーを押した瞬間に反応する関数(長押し反応しない)
	bool TriggerKey(const BYTE& CheckKey);

	//キーボードのキーを離した瞬間に反応する関数(長押し反応しない)
	bool ReleaseKey(const BYTE& CheckKey);

	/// <summary>
	/// クリックしているかどうかを取得
	/// </summary>
	/// <param name="button">0なら左1なら右の入力をチェック</param>
	/// <returns></returns>
	bool GetMouseButton(uint32_t button);

	/// <summary>
	/// クリックした瞬間を取得
	/// </summary>
	/// <param name="button">0なら左1なら右の入力をチェック</param>
	/// <returns></returns>
	bool GetMouseButtonDown(uint32_t button);

	/// <summary>
	/// クリックを離した瞬間を取得
	/// </summary>
	/// <param name="button">0なら左1なら右の入力をチェック</param>
	/// <returns></returns>
	bool GetMouseButtonUp(uint32_t button);

	//マウスの位置取得
	Vector2 GetMousePos();

	//マウスの一フレーム前の位置取得
	Vector2 GetOldMousePos();

	//マウスの移動量取得(zはホイールの移動量)
	Vector3 GetMouseMove();

	//ゲームパッドのボタンを押したかどうか
	bool GetGamePadButton(uint32_t button);

	//ゲームパッドのボタンが押された瞬間かどうか
	bool GetGamePadButtonDown(uint32_t button);

	//ゲームパッドのボタンが離された瞬間かどうか
	bool GetGamePadButtonUp(uint32_t button);

	//現在ゲームパッドがつながっているか
	bool GetIsUseGamePad();

	//ゲームパッドの左トリガーが押されたか
	bool GetGamePadLTrigger();
	//ゲームパッドの右トリガーが押されたか
	bool GetGamePadRTrigger();

	//ゲームパッドの左トリガーが押された瞬間かどうか
	bool GetGamePadLTriggerDown();
	//ゲームパッドの右トリガーが押された瞬間かどうか
	bool GetGamePadRTriggerDown();

	//ゲームパッドの左トリガーが離された瞬間かどうか
	bool GetGamePadLTriggerUp();
	//ゲームパッドの右トリガーが離された瞬間かどうか
	bool GetGamePadRTriggerUp();

	//ゲームパッドの左スティック情報取得
	Vector2 GetGamePadLStick();
	//ゲームパッドの右スティック情報取得
	Vector2 GetGamePadRStick();

	//便利だったのでアイデアをもらった
	
	/// <summary>
	/// 左のスティックの情報をキーボードとまとめて取得できる関数
	/// </summary>
	/// <param name="useWASD">WASDキーをスティックと同じ入力として扱うか</param>
	/// <param name="useArrow">矢印キーをスティックと同じ入力として扱うか</param>
	/// <returns></returns>
	Vector2 GetLStick(const bool& useWASD, const bool& useArrow);

	/// <summary>
	/// 右のスティックの情報をキーボードとまとめて取得できる関数
	/// </summary>
	/// <param name="useWASD">WASDキーをスティックと同じ入力として扱うか</param>
	/// <param name="useArrow">矢印キーをスティックと同じ入力として扱うか</param>
	/// <returns></returns>
	Vector2 GetRStick(const bool& useWASD, const bool& useArrow);

private:
	Input() = default;
	~Input();

	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

	//static Input* instance_;

	HRESULT result_ =S_OK;

	//全キーの入力情報を取得する為の変数
	BYTE key_[256] = {};
	BYTE oldKey_[256] = {};

	//マウスの情報
	DIMOUSESTATE mouseState_ = {};
	DIMOUSESTATE oldMouseState_ = {};

	Vector2 mousePos_;
	Vector2 oldMousePos_;

	//コントローラーの情報
	XINPUT_STATE gamePadState_;
	XINPUT_STATE oldGamePadState_;

	bool IsUseGamePad_ = false;

	//inputの本体
	Microsoft::WRL::ComPtr <IDirectInput8> directInput = nullptr;

	//キーボードデバイス
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard_ = nullptr;
	//マウスデバイス
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouse_ = nullptr;



};