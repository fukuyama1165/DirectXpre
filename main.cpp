#include<Windows.h>

//D3D�R���p�C��
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include<vector>
#include<string>

#include<d3d12.h>
#include<dxgi1_6.h>

//���w���C�u����(�œK�����ꂷ���Ďg���ɂ����炵���̂ł��ƂŎ��삵�����)
#include<DirectXMath.h>
using namespace DirectX;

//#pragma comment�Ƃ́A�I�u�W�F�N�g�t�@�C���ɁA
//�����J�Ń����N���郉�C�u�����̖��O���L�q�������
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

//�L�[�{�[�h��R���g���[���[�Ȃǂ̓��͂���w�b�_�ƃ��C�u�����̃����N
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

const float PI = 3.141592653589f;

#include <DirectXTex.h>

#include "DrawingObj.h"

#include "DirectXInit.h"

#include <dxgidebug.h>

#pragma region �E�B���h�E�v���V�[�W��


LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY://�E�B���h�E���j�����ꂽ
		PostQuitMessage(0);//OS�ɑ΂��āA�I��������Ƃ�`����
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//�W���̏���������
}

#pragma endregion


//�f�o�C�X�������̎��s�����
BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	return DIENUM_CONTINUE;
}


/// <summary>
/// �񎟌��̃A�t�B���ϊ�������֐�
/// </summary>
/// <param name="box">���_�f�[�^(X,Y)</param>
/// <param name="box">�}�`�̒��̌��_(X,Y)</param>
/// <param name="moveX">x�����̈ړ���</param>
/// <param name="moveY">y�����̈ړ���</param>
/// <param name="rotate">��]�p�x(�x���@)</param>
/// <param name="scaleX">x�����̊g�嗦</param>
/// <param name="scaleY">y�����̊g�嗦</param>
XMFLOAT3 Afin(XMFLOAT3 box, XMFLOAT3 box2, float moveX, float moveY, float rotate, float scaleX, float scaleY);

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

#pragma region �E�B���h�E�̏���������
	const int window_width = 1280;
	const int window_height = 720;


	WNDCLASSEX w = {};//�E�B���h�E�N���X�̐ݒ�

	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)windowProc;//�E�B���h�E�v���V�[�W��
	w.lpszClassName = L"DirectXGame";//�E�B���h�E�N���X��
	w.hInstance = GetModuleHandle(nullptr);//�E�B���h�E�n���h��
	w.hCursor = LoadCursor(NULL, IDC_ARROW);//�J�[�\���w��

	//�E�B���h�E�N���X��OS�ɓo�^
	RegisterClassEx(&w);

	//�E�B���h�E�̃T�C�Y�̍\����{x���W,y���W,����,�c��}
	RECT wrc = { 0,0,window_width,window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//�����ŃT�C�Y�␳

	//�E�B���h�E�̍\���v�f�H
	HWND hwnd = CreateWindow(w.lpszClassName,//�N���X��
		L"DirectXGame",//�^�C�g���o�[�̖��O
		WS_OVERLAPPEDWINDOW,//�W���I�ȃE�C���h�E�X�^�C��
		CW_USEDEFAULT,//x���W(OS�ɔC����)
		CW_USEDEFAULT,//y���W(OS�ɔC����)
		wrc.right - wrc.left,//�E�B���h�E����
		wrc.bottom - wrc.top,//�E�B���h�E�c��
		nullptr,//�e�E�B���h�E�n���h��
		nullptr,//���j���[�n���h��
		w.hInstance,//�Ăяo���A�v���P�[�V�����n���h��
		nullptr);//�I�v�V����

	//�E�B���h�E�\������
	ShowWindow(hwnd, SW_SHOW);

	//����������Ȃ��ƃE�B���h�E����u�ŏ����邽�ߋL�q
	MSG msg{};

	

#pragma endregion
	
	DirectXInit* directXinit;

	directXinit = DirectXInit::GetInstance();

	directXinit->Init(w, hwnd,window_width,window_height);

	DrawingObj charactorObj(window_width, window_height);


	charactorObj.basicInit(directXinit->Getdev().Get());

	//�p�C�v���C���X�e�[�g�؂�ւ��p�t���O
	bool PipeLineRuleFlag = true;

	//�l�p�`�ɕύX����Ƃ��̃t���O
	bool ChangeSquareFlag = true;

	//�S�L�[�̓��͏����擾����ׂ̕ϐ�
	/*BYTE key[256] = {};
	BYTE oldKey[256] = {};*/

	
	float Red = 1.0f;
	float Green = 1.0f;
	float Blue = 1.0f;

	float X1 = 0.0f;
	float X2 = 0.0f;
	float Y1 = 0.0f;
	float Y2 = 0.0f;
	float rotate = 0;
	float scaleX = 1;
	float scaleY = 1;

	float angle = 0.0f;//�J�����̉�]�p
	float angleY = 0.0f;//�J�����̉�]�p

	//���W
	Float3 pos={};


	//�Q�[�����[�v
	while (true)
	{
		//���b�Z�[�W������H
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);//�L�[���̓��b�Z�[�W�̏���
			DispatchMessage(&msg);//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
		}

		//�~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
		if (msg.message == WM_QUIT)
		{
			break;
		}

#pragma region DirectX���t���[������

		//DirectX���t���[�������@��������
		
#pragma region �L�[�{�[�h���̎擾

		////�L�[�{�[�h���̎擾�J�n
		//directXinit.GetKeyBoard()->Acquire();

		////�O�t���[���̃L�[�{�[�h���͂�ۑ�
		//for (int i = 0; i < 256; i++)
		//{
		//	oldKey[i] = key[i];
		//}

		////�S�L�[�̓��͏����擾����
		//directXinit.GetKeyBoard()->GetDeviceState(sizeof(key), key);

#pragma endregion

		directXinit->DrawStart();

#pragma region �`��R�}���h

		// 4.�`��R�}���h��������
		
		//�r���[�|�[�g�ݒ�
		//�������̃r���[�|�[�g��p��(���ɂ����̂Ō��炵��)
		D3D12_VIEWPORT viewport{};

		viewport.Width = window_width;//����
		viewport.Height = window_height;//�c��
		viewport.TopLeftX = 0;//����X
		viewport.TopLeftY = 0;//����Y
		viewport.MinDepth = 0.1f;//�ŏ��[�x(0�ł悢)
		viewport.MaxDepth = 1.0f;//�ő�[�x(�P�ł悢)

		//�R�}���h���X�g�ɒǉ�
		directXinit->GetcmdList()->RSSetViewports(1, &viewport);
		

		//�V�U�[��`�ݒ�
		D3D12_RECT scissorrect{};

		scissorrect.left = 0;//�؂蔲�����W��
		scissorrect.right = scissorrect.left + window_width;//�؂蔲�����W�E
		scissorrect.top = 0;//�؂蔲�����W��
		scissorrect.bottom = scissorrect.top + window_height;//�؂蔲�����W��

		directXinit->GetcmdList()->RSSetScissorRects(1, &scissorrect);

		//�v���~�e�B�u�`��(�O�p�`���X�g)
		directXinit->GetcmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		
		charactorObj.Draw(directXinit->GetcmdList().Get(), PipeLineRuleFlag, ChangeSquareFlag);
		
		// 4.�`��R�}���h�����܂�

#pragma endregion

		directXinit->DrawEnd();

		//DirectX���t���[�������@�����܂�

#pragma endregion

#pragma region �X�V����

		if (directXinit->TriggerKey(DIK_2))
		{
			PipeLineRuleFlag = !PipeLineRuleFlag;
		}

		if (directXinit->TriggerKey(DIK_1))
		{
			ChangeSquareFlag = !ChangeSquareFlag;
		}


		if (directXinit->PushKey(DIK_D) or directXinit->PushKey(DIK_A)or directXinit->PushKey(DIK_W)or directXinit->PushKey(DIK_S))
		{
			if (directXinit->PushKey(DIK_D))
			{
				angle += XMConvertToRadians(1.0f);
			}
			else if (directXinit->PushKey(DIK_A))
			{
				angle -= XMConvertToRadians(1.0f);
			}
			else if (directXinit->PushKey(DIK_W))
			{
				angleY += XMConvertToRadians(500.0f);
			}
			else if (directXinit->PushKey(DIK_S))
			{
				angleY -= XMConvertToRadians(500.0f);
			}

			charactorObj.matViewUpdata({ -400 * sinf(angle),angleY,-400 * cosf(angle) }, { 0,0,0 }, { 0,1,0 });
			

		}

		if (directXinit->PushKey(DIK_UP) or directXinit->PushKey(DIK_DOWN) or directXinit->PushKey(DIK_RIGHT) or directXinit->PushKey(DIK_LEFT))
		{
			if (directXinit->PushKey(DIK_UP))
			{
				pos.z += 1.0f;
			}
			else if (directXinit->PushKey(DIK_DOWN))
			{
				pos.z -= 1.0f;
			}
			else if (directXinit->PushKey(DIK_RIGHT))
			{
				pos.x += 1.0f;
			}
			else if (directXinit->PushKey(DIK_LEFT))
			{
				pos.x -= 1.0f;
			}

			charactorObj.SetTrans(pos);

			charactorObj.matWorldUpdata();

		}


		//�F�ύX
		/*if (Red > 0 and Blue <= 0)
		{
			Red -= 0.05f;
			Green += 0.05f;
		}
		if (Green > 0 and Red <= 0)
		{
			Green -= 0.05f;
			Blue += 0.05f;
		}
		if (Blue > 0 and Green <= 0)
		{
			Blue -= 0.05f;
			Red += 0.05f;
		}*/

#pragma endregion

#pragma region �`�揈��

		

#pragma endregion

		

	}
	
	// �E�B���h�E�N���X��o�^����
	UnregisterClass(w.lpszClassName, w.hInstance);

	directXinit->instanceDelete();

	return 0;

	
}
