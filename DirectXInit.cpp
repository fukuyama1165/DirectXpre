#include "DirectXInit.h"

DirectXInit* DirectXInit::instance = nullptr;
using namespace Microsoft::WRL;


DirectXInit::~DirectXInit()
{
}

DirectXInit* DirectXInit::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new DirectXInit();
	}
	return instance;
}

void DirectXInit::instanceDelete()
{
	delete(instance);
}

void DirectXInit::Init(WNDCLASSEX w, HWND hwnd, const int win_width, const int win_height)
{

#pragma region �f�o�b�N���C���[�̗L����



#ifdef _DEBUG

	//�f�o�b�N���C���[���I����
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}

#endif // DEBUG

#pragma endregion

	
	

	//�O���t�B�b�N�A�_�v�^�[
	GraphicAdapterGeneration();

	//�R�}���h���X�g
	CommandListGeneration();

	//�R�}���h�L���[
	CommandQueueGeneration();

	//�X���b�v�`�F�[��
	SwapChainGeneration(hwnd,win_width,win_height);

	DepthBuffGeneration(win_width, win_height);

	//�����_�[�^�[�Q�b�g�r���[
	RTVGeneration();

	//�t�F���X
	FenceGeneration();

	//DirectInput(�ł������p�N���X�����)
	DirectInputGeneration(w, hwnd);
}

void DirectXInit::GraphicAdapterGeneration()
{
#pragma region �O���t�B�b�N�A�_�v�^�[�ݒ�

	//DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

	//�A�_�v�^�[�̗񋓗p
	std::vector<ComPtr<IDXGIAdapter1>> adapters;

	//�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	ComPtr<IDXGIAdapter1> tmpAdapter = nullptr;
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
			tmpAdapter = adapters[i].Get();//�̗p
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
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)//���傭���傭result�̒��g��S_OK�ł��邱�Ƃ��m�F���邱��
		{
			//�f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}

#pragma endregion ����̃N���X���ŕ��������ɒ���
}

void DirectXInit::CommandListGeneration()
{
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
		cmdAllocator.Get(),
		nullptr,
		IID_PPV_ARGS(&cmdList)
	);

#pragma endregion
}

void DirectXInit::CommandQueueGeneration()
{
#pragma region �R�}���h�L���[��

	//�W���ݒ�ŃR�}���h�L���[�𐶐�
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};//�\���̂̒��g��{}�łO�ŃN���A���Ă���(microsoft�̕W����0�̂���)

	dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));

#pragma endregion
}

void DirectXInit::SwapChainGeneration(HWND hwnd, const int win_width, const int win_height)
{
#pragma region �X���b�v�`�F�[����

	//�e��ݒ�����ăX���b�v�`�F�[���𐶐�
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = win_width;
	swapchainDesc.Height = win_height;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//�F���̏���
	swapchainDesc.SampleDesc.Count = 1;//�}���`�T���v�����Ȃ�
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;//�o�b�N�o�b�t�@�p
	swapchainDesc.BufferCount = 2;//�o�b�t�@����2�ɐݒ�
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//�t���b�v��͔j��
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//comptr�̕ϊ��p�ϐ����`
	ComPtr<IDXGISwapChain1> swapChain1;

	dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapChain1
	);

	//��������IDXGISwapChain1�̃I�u�W�F�N�g��IDXGISwapChain4�ɕϊ�
	swapChain1.As(&swapchain);

#pragma endregion
}

void DirectXInit::DepthBuffGeneration(const int win_width, const int win_height)
{

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC depthResourceDesc{};

	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = win_width;//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.Height = win_height;//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;//�f�v�X�e���V��

	//�[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	//�[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;//�[�x�l1.0f(�ő�l)�ŃN���A
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g

	//���\�[�X����
	
	result = dev->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//�[�x�l�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff)
	);

	//�[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//�[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//�f�v�X�X�e���V���r���[
	result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	//�[�x�r���[����
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);

}

void DirectXInit::RTVGeneration()
{
#pragma region �����_�[�^�[�Q�b�g�r���[��

	//�����_�[�^�[�Q�b�g�r���[�̓f�X�N���v�^�q�[�v���琶�������̂�
	//�f�X�N���v�^�q�[�v���ɐ�������

	//�e��ݒ�����ăf�X�N���v�^�q�[�v�𐶐�
	
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//�����_�[�^�[�Q�b�g�r���[
	heapDesc.NumDescriptors = 2;//���\�̂Q��
	dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));

	//�����_�[�^�[�Q�b�g�r���[�̐�������


	//���\��2���ɂ���
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
			backBuffers[i].Get(),
			nullptr,
			handle
		);

	}

#pragma endregion �o�b�N�o�b�t�@��`��L�����o�X�Ƃ��Ĉ����ׂ̃I�u�W�F�N�g
}


void DirectXInit::DrawStart()
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



	//���\�[�X�o���A��

	//�o�b�N�o�b�t�@�̔ԍ����擾(�Q�Ȃ̂�0�Ԃ�1��)
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get();//�o�b�N�o�b�t�@���w��
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;//�\������
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;//�`��
	cmdList->ResourceBarrier(1, &barrierDesc);

	// 2.�`���w��
	//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
	//cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvHandle);

	// 3.��ʃN���A

	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}


void DirectXInit::DrawEnd()
{
	// 5.���\�[�X�o���A��߂�(�������߂��Ԃ��ƕ`��ł��Ȃ��̂ŕ`��ł���悤�ɂ���)
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;//�`��
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;//�\����
	cmdList->ResourceBarrier(1, &barrierDesc);

	//���߂̃N���[�Y(�����ŕ`��̖��߂͂�������̂Ŏ��s�ɂ���)
	cmdList->Close();
	//�R�}���h���X�g�̎��s
	ID3D12CommandList* cmdLists[] = { cmdList.Get()};
	cmdQueue->ExecuteCommandLists(1, cmdLists);

	//��ʂɕ\������o�b�t�@���t���b�v(���\�̓���ւ�)
	swapchain->Present(1, 0);

	//�R�}���h�����҂�(����������Ȃ��Ƃ��������Ɉڍs���Ă��܂�����)
	//�R�}���h�̎��s������҂�
	cmdQueue->Signal(fence.Get(), ++fenceVel);
	if (fence->GetCompletedValue() != fenceVel)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVel, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	cmdAllocator->Reset();
	cmdList->Reset(cmdAllocator.Get(), nullptr);
}

void DirectXInit::clearColorChange(float R, float G, float B, float A)
{
	clearColor[0] = R;
	clearColor[1] = G;
	clearColor[2] = B;
	clearColor[3] = A;
}

bool DirectXInit::PushKey(BYTE CheckKey)
{
	return key[CheckKey];
}

bool DirectXInit::TriggerKey(BYTE CheckKey)
{
	return key[CheckKey] and oldKey[CheckKey] == 0;
}


void DirectXInit::FenceGeneration()
{
#pragma region �t�F���X��
	//�t�F���X�𐶐�
	result = dev->CreateFence(fenceVel, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

#pragma endregion CPU��GPU�œ������Ƃ邽�߂�DirectX�̎d�g��
}

void DirectXInit::DirectInputGeneration(WNDCLASSEX w, HWND hwnd)
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

//get�֐��Q
ComPtr<ID3D12Device> DirectXInit::Getdev()
{
	return dev;
}

IDirectInputDevice8* DirectXInit::GetKeyBoard()
{
	return keyboard;
}

ComPtr<ID3D12GraphicsCommandList> DirectXInit::GetcmdList()
{
	return cmdList;
}
