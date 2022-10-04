#include "Input.h"

BYTE Input::key[256] = {};
BYTE Input::oldKey[256] = {};

void Input::init(WNDCLASSEX w, HWND hwnd)
{

#pragma region DirectInout�I�u�W�F�N�g�̐���

	//DirectInput�̏�����
	IDirectInput8* directInput = nullptr;
	result = DirectInput8Create(
		w.hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&directInput,
		nullptr
	);
	assert(SUCCEEDED(result));

#pragma endregion �S�̂̏������ł�����̓f�o�C�X�������Ă����̃I�u�W�F�N�g�͑��₳�Ȃ��Ă�����

#pragma region �L�[�{�[�h�f�o�C�X�̐���

	//�L�[�{�[�h�f�o�C�X�̐���

	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region �Q�[���p�b�h�̓���

	//DIDEVICEINSTANCE* gamePadDID = {};
	//result = directInput->EnumDevices(
	//	DI8DEVTYPE_JOYSTICK,//JOYSTICK���ƔF�����Ȃ����Ƃ�����炵��(�m�F�ł���Ȃ�m�F���邱��)
	//	DeviceFindCallBack,
	//	&gamePadDID,
	//	DIEDFL_ATTACHEDONLY
	//);
	//assert(SUCCEEDED(result));

	//IDirectInputDevice8* gamePadDirct = nullptr;
	//result = directInput->CreateDevice(gamePadDID->guidInstance, &gamePadDirct, NULL);
	//assert(SUCCEEDED(result));


#pragma endregion(���~)

#pragma region ���̓f�[�^�`���̃Z�b�g

	result = keyboard->SetDataFormat(&c_dfDIKeyboard);//�W���`��
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region �r�����䃌�x���̃Z�b�g

	result = keyboard->SetCooperativeLevel(
		hwnd,
		DISCL_FOREGROUND/*��ʂ���O�ɂ���ꍇ�̂ݓ��͂��󂯕t����*/ |
		DISCL_NONEXCLUSIVE/*�f�o�C�X�����̃A�v�������Ő�L���Ȃ�*/ |
		DISCL_NOWINKEY/*window�L�[�𖳌��ɂ���*/);
	assert(SUCCEEDED(result));

#pragma endregion

}

void Input::update()
{

#pragma region �L�[�{�[�h���̎擾

	//�L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();

	//�O�t���[���̃L�[�{�[�h���͂�ۑ�
	for (int i = 0; i < 256; i++)
	{
		oldKey[i] = key[i];
	}

	//�S�L�[�̓��͏����擾����
	keyboard->GetDeviceState(sizeof(key), key);

#pragma endregion

}

bool Input::PushKey(BYTE CheckKey)
{
	return key[CheckKey];
}

bool Input::TriggerKey(BYTE CheckKey)
{
	return key[CheckKey] and oldKey[CheckKey] == 0;
}