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

const float PI = 3.141592653589;

#include <DirectXTex.h>

#include "DrawingObj.h"


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

//�w�i�F�ύX���邽�߂ɊO�ɔz�u
float clearColor[] = { 0.1f,0.25f,0.5f,0.0f };//���ۂ��F(��ʃN���A����Ƃ��̐F)

//��ʂ̃N���A�J���[�ύX�֐�
void clearColorChange(float R, float G, float B, float A);

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


#pragma region DirectX����������
//��������	
#pragma region �f�o�b�N���C���[�̗L����



#ifdef _DEBUG

	//�f�o�b�N���C���[���I����
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}

#endif // DEBUG

#pragma endregion

#pragma region �������p�ϐ��錾

	HRESULT result;
	ID3D12Device* dev = nullptr;
	IDXGIFactory6* dxgiFactory = nullptr;
	IDXGISwapChain4* swapchain = nullptr;
	ID3D12CommandAllocator* cmdAllocator = nullptr;
	ID3D12GraphicsCommandList* cmdList = nullptr;
	ID3D12CommandQueue* cmdQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeaps = nullptr;

#pragma endregion

#pragma region �O���t�B�b�N�A�_�v�^�[�ݒ�


	
	//DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

	//�A�_�v�^�[�̗񋓗p
	std::vector<IDXGIAdapter1*> adapters;

	//�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	IDXGIAdapter1* tmpAdapter = nullptr;
	for (int i = 0; dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);//���I�z��ɒǉ�����
	}

	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adapters[i]->GetDesc1(&adesc);//�A�_�v�^�[�̏����擾

		//�\�t�g�E�F�A�f�o�C�X�����
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}

		std::wstring strDesc = adesc.Description;//�A�_�v�^�[��

		//Intel UHD Graphice(�I���{�[�h�O���t�B�b�N)�����
		if (strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i];//�̗p
			break;
		}

	}

	//�̗p�����O���t�B�b�N�f�o�C�X�𑀍삷�邽�߂̃I�u�W�F�N�g�𐶐�
	//�����ʂ͈�������Ȃ��̂ŃN���X��������ۂɕ����������Ȃ��悤��

	//�Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (int i = 0; i < _countof(levels); i++)
	{
		//�̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter, levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)//���傭���傭result�̒��g��S_OK�ł��邱�Ƃ��m�F���邱��
		{
			//�f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}

#pragma endregion ����̃N���X���ŕ��������ɒ���

#pragma region �R�}���h���X�g��

	//�R�}���h���X�g�𐶐�����ɂ̓R�}���h�A���P�[�^���K�v
	//�Ȃ̂Ő�ɃR�}���h�A���P�[�^�𐶐�

	//�R�}���h�A���P�[�^�𐶐�
	result = dev->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator)
	);

	//�R�}���h���X�g�𐶐�
	result = dev->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator,
		nullptr,
		IID_PPV_ARGS(&cmdList)
	);

#pragma endregion

#pragma region �R�}���h�L���[��

	//�W���ݒ�ŃR�}���h�L���[�𐶐�
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};//�\���̂̒��g��{}�łO�ŃN���A���Ă���(microsoft�̕W����0�̂���)

	dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));

#pragma endregion

#pragma region �X���b�v�`�F�[����

	//�e��ݒ�����ăX���b�v�`�F�[���𐶐�
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = 1280;
	swapchainDesc.Height = 720;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//�F���̏���
	swapchainDesc.SampleDesc.Count = 1;//�}���`�T���v�����Ȃ�
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;//�o�b�N�o�b�t�@�p
	swapchainDesc.BufferCount = 2;//�o�b�t�@����2�ɐݒ�
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//�t���b�v��͔j��
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue,
		hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)&swapchain
	);

#pragma endregion

#pragma region �����_�[�^�[�Q�b�g�r���[��

	//�����_�[�^�[�Q�b�g�r���[�̓f�X�N���v�^�q�[�v���琶�������̂�
	//�f�X�N���v�^�q�[�v���ɐ�������

	//�e��ݒ�����ăf�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//�����_�[�^�[�Q�b�g�r���[
	heapDesc.NumDescriptors = 2;//���\�̂Q��
	dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));

	//�����_�[�^�[�Q�b�g�r���[�̐�������

	//���\��2���ɂ���
	std::vector<ID3D12Resource*> backBuffers(2);
	for (int i = 0; i < 2; i++)
	{
		//�X���b�v�`�F�[������o�b�t�@���擾
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		//�f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
		//�����\���ŃA�h���X�������
		handle.ptr += i * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
		//�����_�[�^�[�Q�b�g�r���[�̐���
		dev->CreateRenderTargetView(
			backBuffers[i],
			nullptr,
			handle
		);

	}

#pragma endregion �o�b�N�o�b�t�@��`��L�����o�X�Ƃ��Ĉ����ׂ̃I�u�W�F�N�g

#pragma region �t�F���X��

	//�t�F���X�𐶐�
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVel = 0;

	result = dev->CreateFence(fenceVel, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

#pragma endregion CPU��GPU�œ������Ƃ邽�߂�DirectX�̎d�g��

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
	IDirectInputDevice8* keyboard = nullptr;
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


	//DirectX�����������@�����܂�

#pragma endregion
	


	DrawingObj charactorObj(window_width, window_height, { 0.0f,100.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 100.0f,100.0f,0.0f }, { 100.0f,0.0f,0.0f });


	charactorObj.basicInit(dev);



	//�p�C�v���C���X�e�[�g�؂�ւ��p�t���O
	bool PipeLineRuleFlag = true;

	//�l�p�`�ɕύX����Ƃ��̃t���O
	bool ChangeSquareFlag = true;

	//�S�L�[�̓��͏����擾����ׂ̕ϐ�
	BYTE key[256] = {};
	BYTE oldKey[256] = {};

	
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

		

		//���\�[�X�o���A��

		//�o�b�N�o�b�t�@�̔ԍ����擾(�Q�Ȃ̂�0�Ԃ�1��)
		UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

		// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = backBuffers[bbIndex];//�o�b�N�o�b�t�@���w��
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;//�\������
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;//�`��
		cmdList->ResourceBarrier(1, &barrierDesc);

		// 2.�`���w��
		//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
		rtvH.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
		cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);

		// 3.��ʃN���A
		
		cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

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
		cmdList->RSSetViewports(1, &viewport);
		

		//�V�U�[��`�ݒ�
		D3D12_RECT scissorrect{};

		scissorrect.left = 0;//�؂蔲�����W��
		scissorrect.right = scissorrect.left + window_width;//�؂蔲�����W�E
		scissorrect.top = 0;//�؂蔲�����W��
		scissorrect.bottom = scissorrect.top + window_height;//�؂蔲�����W��

		cmdList->RSSetScissorRects(1, &scissorrect);

		//�v���~�e�B�u�`��(�O�p�`���X�g)
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		
		charactorObj.Draw(cmdList, PipeLineRuleFlag, ChangeSquareFlag);
		
		// 4.�`��R�}���h�����܂�

#pragma endregion

		// 5.���\�[�X�o���A��߂�(�������߂��Ԃ��ƕ`��ł��Ȃ��̂ŕ`��ł���悤�ɂ���)
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;//�`��
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;//�\����
		cmdList->ResourceBarrier(1, &barrierDesc);

		//���߂̃N���[�Y(�����ŕ`��̖��߂͂�������̂Ŏ��s�ɂ���)
		cmdList->Close();
		//�R�}���h���X�g�̎��s
		ID3D12CommandList* cmdLists[] = { cmdList };
		cmdQueue->ExecuteCommandLists(1, cmdLists);

		//��ʂɕ\������o�b�t�@���t���b�v(���\�̓���ւ�)
		swapchain->Present(1, 0);

		//�R�}���h�����҂�(����������Ȃ��Ƃ��������Ɉڍs���Ă��܂�����)
		//�R�}���h�̎��s������҂�
		cmdQueue->Signal(fence, ++fenceVel);
		if (fence->GetCompletedValue() != fenceVel)
		{
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			fence->SetEventOnCompletion(fenceVel, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}

		cmdAllocator->Reset();
		cmdList->Reset(cmdAllocator, nullptr);


		//DirectX���t���[�������@�����܂�

#pragma endregion

#pragma region �X�V����

		X1 = 0.0f;
		Y1 = 0.0f;
		rotate = 0.0f;
		scaleX = 1.0f;
		scaleY = 1.0f;

		if (key[DIK_D])
		{
			OutputDebugStringA("Hit D\n");
		}

		if (key[DIK_2] and oldKey[DIK_2] == 0)
		{
			PipeLineRuleFlag = !PipeLineRuleFlag;
			OutputDebugStringA("Hit W\n");
		}

		if (key[DIK_W] )
		{
			X2 += 0.001f;
		}

		if (key[DIK_S] )
		{
			X2 -= 0.001f;
		}

		if (key[DIK_SPACE])
		{
			clearColorChange(0.9f, 0.2f, 0.5f, 0.0f);
		}
		else
		{
			clearColorChange(0.1f, 0.25f, 0.5f, 0.0f);
		}

		if (key[DIK_1] and oldKey[DIK_1] == 0)
		{
			ChangeSquareFlag = !ChangeSquareFlag;
		}

		if (key[DIK_Z] )
		{
			
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

	//�E�B���h�E�N���X��o�^����
	UnregisterClass(w.lpszClassName, w.hInstance);

	

	return 0;

	
}

void clearColorChange(float R, float G, float B, float A)
{
	clearColor[0] = R;
	clearColor[1] = G;
	clearColor[2] = B;
	clearColor[3] = A;
}

