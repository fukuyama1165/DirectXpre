#pragma once
//D3D�R���p�C��
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include<vector>
#include<string>

#include<d3d12.h>
#include<dxgi1_6.h>

//assert���g�����߂ɕK�v
#include <cassert>


//#pragma comment�Ƃ́A�I�u�W�F�N�g�t�@�C���ɁA
//�����J�Ń����N���郉�C�u�����̖��O���L�q�������
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")


//�L�[�{�[�h��R���g���[���[�Ȃǂ̓��͂���w�b�_�ƃ��C�u�����̃����N
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")


class Input
{
public:
	void init(WNDCLASSEX w, HWND hwnd);

	void update();

	//�L�[�{�[�h�̃L�[���������甽������֐�(������������)
	bool PushKey(BYTE CheckKey);

	//�L�[�{�[�h�̃L�[���������u�Ԃɔ�������֐�(�������������Ȃ�)
	bool TriggerKey(BYTE CheckKey);

private:
	HRESULT result;

	//�S�L�[�̓��͏����擾����ׂ̕ϐ�
	static BYTE key[256];
	static BYTE oldKey[256];

	//�L�[�{�[�h�f�o�C�X
	IDirectInputDevice8* keyboard = nullptr;

};

