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

//#include "DrawingObj.h"

#include "WinApp.h"

#include "DirectXInit.h"

#include <dxgidebug.h>

#include "Input.h"

//#include "DrawOBJ.h"
#include "Object3D.h"

#pragma region �E�B���h�E�v���V�[�W��


//LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
//{
//	switch (msg)
//	{
//	case WM_DESTROY://�E�B���h�E���j�����ꂽ
//		PostQuitMessage(0);//OS�ɑ΂��āA�I��������Ƃ�`����
//		return 0;
//	}
//	return DefWindowProc(hwnd, msg, wparam, lparam);//�W���̏���������
//}

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
	//windowAPI
	WinApp* winApp;

	winApp = new WinApp();

	winApp->initialize();
	
	//directX�̏�����
	DirectXInit* directXinit;

	directXinit = DirectXInit::GetInstance();

	directXinit->Init(winApp->getW(), winApp->getHwnd(), winApp->getWindowSizeWidth(), winApp->getWindowSizeHeight());

	//���͂̏�����
	Input* input=new Input();

	input->init(winApp->getW(), winApp->getHwnd());

	//obj
	Object3D charactorObj;
	Object3D charactorObj2;

	//.obj�̃I�u�W�F�N�g
	//DrawOBJ test(winApp->getWindowSizeWidth(), winApp->getWindowSizeHeight());


	charactorObj.colorChangeInit(directXinit->Getdev().Get(), winApp->getWindowSizeWidth(), winApp->getWindowSizeHeight());
	charactorObj2.basicInit(directXinit->Getdev().Get(), winApp->getWindowSizeWidth(), winApp->getWindowSizeHeight());

	//test.basicInit((directXinit->Getdev().Get()));

	//�p�C�v���C���X�e�[�g�؂�ւ��p�t���O
	bool PipeLineRuleFlag = true;

	//�l�p�`�ɕύX����Ƃ��̃t���O
	bool ChangeSquareFlag = false;

	bool ChangeTexure = false;

	
	float Red = 1.0f;
	float Green = 0.0f;
	float Blue = 0.0f;

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

	//������̂������������̂�
	/*test.SetScale({ 20,20,0 });
	test.obj3DUpdate();*/


	//�Q�[�����[�v
	while (true)
	{

		if (winApp->processMassage() or input->TriggerKey(DIK_ESCAPE))
		{

			break;

		}

#pragma region DirectX���t���[������

		//DirectX���t���[�������@��������
		
#pragma region �L�[�{�[�h���̎擾

		input->update();

#pragma endregion

		directXinit->DrawStart();

#pragma region �`��R�}���h

		// 4.�`��R�}���h��������
		
		//�r���[�|�[�g�ݒ�
		//�������̃r���[�|�[�g��p��(���ɂ����̂Ō��炵��)
		D3D12_VIEWPORT viewport{};

		viewport.Width = winApp->getWindowSizeWidth();//����
		viewport.Height = winApp->getWindowSizeHeight();//�c��
		viewport.TopLeftX = 0;//����X
		viewport.TopLeftY = 0;//����Y
		viewport.MinDepth = 0.1f;//�ŏ��[�x(0�ł悢)
		viewport.MaxDepth = 1.0f;//�ő�[�x(�P�ł悢)

		//�R�}���h���X�g�ɒǉ�
		directXinit->GetcmdList()->RSSetViewports(1, &viewport);
		

		//�V�U�[��`�ݒ�
		D3D12_RECT scissorrect{};

		scissorrect.left = 0;//�؂蔲�����W��
		scissorrect.right = scissorrect.left + winApp->getWindowSizeWidth();//�؂蔲�����W�E
		scissorrect.top = 0;//�؂蔲�����W��
		scissorrect.bottom = scissorrect.top + winApp->getWindowSizeHeight();//�؂蔲�����W��

		directXinit->GetcmdList()->RSSetScissorRects(1, &scissorrect);

		//�v���~�e�B�u�`��(�O�p�`���X�g)
		directXinit->GetcmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		
		charactorObj.Draw(directXinit->GetcmdList().Get(), PipeLineRuleFlag, ChangeSquareFlag,true);
		charactorObj2.Draw(directXinit->GetcmdList().Get(), PipeLineRuleFlag, true,ChangeTexure);

		//test.Draw(directXinit->GetcmdList().Get(), PipeLineRuleFlag, true, true);
		
		// 4.�`��R�}���h�����܂�

#pragma endregion

		directXinit->DrawEnd();

		//DirectX���t���[�������@�����܂�

#pragma endregion

#pragma region �X�V����

		if (input->TriggerKey(DIK_2))
		{
			PipeLineRuleFlag = !PipeLineRuleFlag;
		}

		if (input->TriggerKey(DIK_1))
		{
			ChangeSquareFlag = !ChangeSquareFlag;
		}

		if (input->TriggerKey(DIK_3))
		{
			ChangeTexure = !ChangeTexure;
		}


		if (input->PushKey(DIK_D) or input->PushKey(DIK_A)or input->PushKey(DIK_W)or input->PushKey(DIK_S))
		{
			if (input->PushKey(DIK_D))
			{
				angle += XMConvertToRadians(1.0f);
			}
			else if (input->PushKey(DIK_A))
			{
				angle -= XMConvertToRadians(1.0f);
			}
			else if (input->PushKey(DIK_W))
			{
				angleY -= XMConvertToRadians(500.0f);
			}
			else if (input->PushKey(DIK_S))
			{
				angleY += XMConvertToRadians(500.0f);
			}

			//charactorObj.matViewUpdata({ -400 * sinf(angle),angleY,-400 * cosf(angle) }, { 0,0,0 }, { 0,1,0 });
			charactorObj.matViewUpdata({ -400 * sinf(angle),angleY,  -400 * cosf(angle)}, {0,0,0}, {0,1,0});
			charactorObj2.matViewUpdata({ -400 * sinf(angle),angleY,  -400 * cosf(angle)}, {0,0,0}, {0,1,0});

		}

		if (input->PushKey(DIK_UP) or input->PushKey(DIK_DOWN) or input->PushKey(DIK_RIGHT) or input->PushKey(DIK_LEFT))
		{
			if (input->PushKey(DIK_UP))
			{
				pos.z += 1.0f;
			}
			else if (input->PushKey(DIK_DOWN))
			{
				pos.z -= 1.0f;
			}
			else if (input->PushKey(DIK_RIGHT))
			{
				pos.x += 1.0f;
			}
			else if (input->PushKey(DIK_LEFT))
			{
				pos.x -= 1.0f;
			}

			charactorObj.SetPos(pos);

			charactorObj.Update();

		}

		


		//�F�ύX
		if (Red > 0 and Blue <= 0)
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
		}

#pragma endregion

#pragma region �`�揈��

		//charactorObj.colorMap(Red, Green, Blue);

#pragma endregion

		

	}
	
	delete winApp;

	//texture.h��ǂݍ���ł���objct�������Ă�delte����͈̂��ɂ��邱��
	charactorObj.deleteTexture();

	delete input;

	directXinit->instanceDelete();

	return 0;

	
}
