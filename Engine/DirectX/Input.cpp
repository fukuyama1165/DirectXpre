#include "Input.h"
#include "WinApp.h"
#include <imgui.h>

Input::~Input()
{

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

#pragma region マウスデバイスの生成

	//マウスデバイスの生成

	result_ = directInput->CreateDevice(GUID_SysMouse, &mouse_, NULL);
	assert(SUCCEEDED(result_));

#pragma endregion

#pragma region 入力データ形式のセット

	result_ = keyboard_->SetDataFormat(&c_dfDIKeyboard);//標準形式
	assert(SUCCEEDED(result_));

	result_ = mouse_->SetDataFormat(&c_dfDIMouse);//標準形式
	assert(SUCCEEDED(result_));

#pragma endregion

#pragma region 排他制御レベルのセット

	result_ = keyboard_->SetCooperativeLevel(
		WinApp::GetInstance()->getHwnd(),
		DISCL_FOREGROUND/*画面が手前にある場合のみ入力を受け付ける*/ |
		DISCL_NONEXCLUSIVE/*デバイスをこのアプリだけで占有しない*/ |
		DISCL_NOWINKEY/*windowキーを無効にする*/);
	assert(SUCCEEDED(result_));

	result_ = mouse_->SetCooperativeLevel(
		WinApp::GetInstance()->getHwnd(),
		DISCL_FOREGROUND/*画面が手前にある場合のみ入力を受け付ける*/ |
		DISCL_NONEXCLUSIVE/*デバイスをこのアプリだけで占有しない*/);
	assert(SUCCEEDED(result_));

#pragma endregion

	//一応の初期化
	ZeroMemory(&gamePadState_, sizeof(XINPUT_STATE));
	DWORD dwResult = XInputGetState(0, &gamePadState_);

	//コントローラー繋いでいるかチェック
	if (dwResult == ERROR_SUCCESS)
	{
		IsUseGamePad_ = true;
	}
	else
	{
		IsUseGamePad_ = false;
	}

}

void Input::update()
{

#pragma region キーボード情報の取得

	//前フレームのキーボード入力を保存
	for (uint16_t i = 0; i < 256; i++)
	{
		oldKey_[i] = key_[i];
	}

	//過去情報を保存
	oldMouseState_ = mouseState_;

	oldMousePos_ = mousePos_;

	oldGamePadState_ = gamePadState_;

	//キーボード情報の取得開始
	keyboard_->Acquire();

	//マウスの情報取得準備
	mouse_->Acquire();

	//全キーの入力情報を取得する
	keyboard_->GetDeviceState(sizeof(key_), key_);

	//マウスの情報取得
	mouse_->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState_);

	mousePos_ = WinApp::GetInstance()->GetMousePos();

	//ゲームパッドの入力を取得

	DWORD dwResult = XInputGetState(0, &gamePadState_);

	//コントローラー繋いでいるかチェック
	if (dwResult == ERROR_SUCCESS)
	{
		IsUseGamePad_ = true;
	}
	else
	{
		IsUseGamePad_ = false;
	}

	//デットゾーン確認
	if ((gamePadState_.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		gamePadState_.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(gamePadState_.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			gamePadState_.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		gamePadState_.Gamepad.sThumbLX = 0;
		gamePadState_.Gamepad.sThumbLY = 0;
	}

	if ((gamePadState_.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		gamePadState_.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(gamePadState_.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			gamePadState_.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		gamePadState_.Gamepad.sThumbRX = 0;
		gamePadState_.Gamepad.sThumbRY = 0;
	}


	if (AllKeyCheck() || GetMouseInput())
	{
		isUseKeybord_ = true;
	}
	else if (GetGamePadInput())
	{
		isUseKeybord_ = false;
	}

#pragma endregion

}

bool Input::PushKey(const BYTE& CheckKey, bool useImgui)
{
	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow) && CheckKey != DIK_ESCAPE && !useImgui)return false;
	return key_[CheckKey];
}

bool Input::TriggerKey(const BYTE& CheckKey, bool useImgui)
{
	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow) && CheckKey != DIK_ESCAPE && !useImgui)return false;
	return key_[CheckKey] && oldKey_[CheckKey] == 0;
}

bool Input::ReleaseKey(const BYTE& CheckKey, bool useImgui)
{
	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow) && !useImgui)return false;
	return key_[CheckKey] == 0 && oldKey_[CheckKey];
}

bool Input::AllKeyCheck()
{
	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))return false;
	for (uint16_t i = 0; i < 256; i++)
	{
		if (key_[i])
		{
			return true;
		}
	}

	return false;
}

bool Input::GetMouseButton(uint32_t button, bool useImgui)
{
	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow) && !useImgui)return false;
	return(mouseState_.rgbButtons[button] & (0x80)) != 0;
}

bool Input::GetMouseButtonDown(uint32_t button, bool useImgui)
{
	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow) && !useImgui)return false;
	return((mouseState_.rgbButtons[button] & (0x80)) != 0) && ((oldMouseState_.rgbButtons[button] & (0x80)) == 0);
}

bool Input::GetMouseButtonUp(uint32_t button, bool useImgui)
{
	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow) && !useImgui)return false;
	return((mouseState_.rgbButtons[button] & (0x80)) == 0) && ((oldMouseState_.rgbButtons[button] & (0x80)) != 0);
}

Vector2 Input::GetMousePos()
{
	return mousePos_;
}

Vector2 Input::GetOldMousePos()
{
	return oldMousePos_;
}

Vector3 Input::GetMouseMove()
{
	return Vector3((float)mouseState_.lX, (float)mouseState_.lY, (float)mouseState_.lZ);
}

bool Input::GetMouseInput()
{
	if (GetMouseButton(0) || GetMouseButton(1) || GetMouseMove().x > 0.2f || GetMouseMove().y > 0.2f || GetMouseMove().z > 0.2f)
	{
		return true;
	}

	return false;
}

bool Input::GetMouseInWindow()
{

	if ((mousePos_.x >= 0 && mousePos_.y >= 0) && (mousePos_.x <= WinApp::SWindowWidthF_ && mousePos_.y <= WinApp::SWindowHeightF_))
	{
		return true;
	}

	return false;
}

bool Input::GetIsUseGamePad()
{
	return IsUseGamePad_;
}

bool Input::GetGamePadButton(uint32_t button)
{
	return (gamePadState_.Gamepad.wButtons & button) != 0;
}

bool Input::GetGamePadButtonDown(uint32_t button)
{
	return ((gamePadState_.Gamepad.wButtons & button) != 0) && (((oldGamePadState_.Gamepad.wButtons & button) == 0));
}

bool Input::GetGamePadButtonUp(uint32_t button)
{
	return ((gamePadState_.Gamepad.wButtons & button) == 0) && (((oldGamePadState_.Gamepad.wButtons & button) != 0));
}

bool Input::GetGamePadAllButton()
{
	return (((gamePadState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0) ||
		((gamePadState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0) ||
		((gamePadState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0) ||
		((gamePadState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0) ||
		((gamePadState_.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0) ||
		((gamePadState_.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0) ||
		((gamePadState_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0) ||
		((gamePadState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0) ||
		((gamePadState_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0) ||
		((gamePadState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0) ||
		((gamePadState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0) ||
		((gamePadState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0) ||
		((gamePadState_.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0) ||
		((gamePadState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0));
}

bool Input::GetGamePadLTrigger()
{
	return (gamePadState_.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
}

bool Input::GetGamePadRTrigger()
{
	return (gamePadState_.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
}

bool Input::GetGamePadLTriggerDown()
{
	return (gamePadState_.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) && (oldGamePadState_.Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
}

bool Input::GetGamePadRTriggerDown()
{
	return (gamePadState_.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) && (oldGamePadState_.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
}

bool Input::GetGamePadLTriggerUp()
{
	return (gamePadState_.Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) && (oldGamePadState_.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
}

bool Input::GetGamePadRTriggerUp()
{
	return (gamePadState_.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) && (oldGamePadState_.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
}

Vector2 Input::GetGamePadLStick()
{
	//スティックの情報が-32768～32767の状態で持っているためそのままの値で渡すと大変なことになるため割っている
	return Vector2(static_cast<float>(gamePadState_.Gamepad.sThumbLX) / 32767.0f, static_cast<float>(gamePadState_.Gamepad.sThumbLY) / 32767.0f);
}

Vector2 Input::GetGamePadRStick()
{
	//スティックの情報が-32768～32767の状態で持っているためそのままの値で渡すと大変なことになるため割っている
	return Vector2(static_cast<float>(gamePadState_.Gamepad.sThumbRX) / 32767.0f, static_cast<float>(gamePadState_.Gamepad.sThumbRY) / 32767.0f);
}

bool Input::GetGamePadStickInput()
{
	return (GetGamePadLStick().x > 0.1f || GetGamePadLStick().x < -0.1f) || (GetGamePadLStick().y > 0.1f || GetGamePadLStick().y < -0.1f) || (GetGamePadRStick().x > 0.1f || GetGamePadRStick().x < -0.1f) || (GetGamePadRStick().y > 0.1f || GetGamePadRStick().y < -0.1f);
}

bool Input::GetGamePadInput()
{
	if (!IsUseGamePad_)
	{
		return false;
	}

	return GetGamePadAllButton() || GetGamePadLTrigger() || GetGamePadRTrigger() || GetGamePadStickInput();
}

Vector2 Input::GetLStick(const bool& useWASD, const bool& useArrow)
{
	Vector2 pad(static_cast<float>(gamePadState_.Gamepad.sThumbLX) / 32767.0f, static_cast<float>(gamePadState_.Gamepad.sThumbLY) / 32767.0f);

	Vector2 wasd;
	if (useWASD)
	{
		wasd.x = static_cast<float>(key_[DIK_D] - key_[DIK_A]);
		wasd.y = static_cast<float>(key_[DIK_W] - key_[DIK_S]);
	}

	Vector2 arrow;
	if (useArrow)
	{
		arrow.x = static_cast<float>(key_[DIK_RIGHT] - key_[DIK_LEFT]);
		arrow.y = static_cast<float>(key_[DIK_UP] - key_[DIK_DOWN]);
	}

	Vector2 ans = pad + wasd + arrow;
	ans.normalize();

	return ans;


}

Vector2 Input::GetRStick(const bool& useWASD, const bool& useArrow)
{

	Vector2 pad(static_cast<float>(gamePadState_.Gamepad.sThumbRX) / 32767.0f, static_cast<float>(gamePadState_.Gamepad.sThumbRY) / 32767.0f);

	Vector2 wasd;
	if (useWASD)
	{
		wasd.x = static_cast<float>(key_[DIK_D] - key_[DIK_A]);
		wasd.y = static_cast<float>(key_[DIK_W] - key_[DIK_S]);
	}

	Vector2 arrow;
	if (useArrow)
	{
		arrow.x = static_cast<float>(key_[DIK_RIGHT] - key_[DIK_LEFT]);
		arrow.y = static_cast<float>(key_[DIK_UP] - key_[DIK_DOWN]);
	}

	Vector2 ans = pad + wasd + arrow;
	ans.normalize();

	return ans;

}