#pragma once
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

#include <DirectXTex.h>

#include "DrawingObj.h"

//ComPtr�p�C���N���[�h
#include <wrl.h>

#include <dxgidebug.h>


class DirectXInit
{
public:

	static DirectXInit* GetInstance();

	/*static void Create();

	static void Destroy()*/;
	
	//������
	
	void Init(WNDCLASSEX w, HWND hwnd,const int win_width, const int win_height);

	//�O���t�B�b�N�A�_�v�^�[
	void GraphicAdapterGeneration();

	//�R�}���h���X�g
	void CommandListGeneration();

	//�R�}���h�L���[
	void CommandQueueGeneration();

	//�X���b�v�`�F�[��
	void SwapChainGeneration(HWND hwnd, const int win_width, const int win_height);

	//�[�x�o�b�t�@
	void DepthBuffGeneration(const int win_width, const int win_height);

	//�����_�[�^�[�Q�b�g�r���[
	void RTVGeneration();

	//�t�F���X
	void FenceGeneration();

	//DirectInput(�ł������p�N���X�����)
	void DirectInputGeneration(WNDCLASSEX w, HWND hwnd);

	//dev��Ԃ��֐�
	Microsoft::WRL::ComPtr<ID3D12Device> Getdev();

	//keyboard��Ԃ��֐�
	IDirectInputDevice8* GetKeyBoard();

	//cmdList��Ԃ��֐�
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetcmdList();

	//�`��̏��߂̕���
	void DrawStart();

	//�`��I���̕���
	void DrawEnd();


	//��ʂ̃N���A�J���[�ύX�֐�
	void clearColorChange(float R, float G, float B, float A);

	//input�ł̓��͔����֐�(inputClass��������炱����ړ�����)

	//�L�[�{�[�h�̃L�[���������甽������֐�(������������)
	bool PushKey(BYTE CheckKey);

	//�L�[�{�[�h�̃L�[���������u�Ԃɔ�������֐�(�������������Ȃ�)
	bool TriggerKey(BYTE CheckKey);

	void instanceDelete();

private:

	DirectXInit()=default;
	~DirectXInit();

	DirectXInit(const DirectXInit&) = delete;
	DirectXInit& operator=(const DirectXInit&) = delete;

	static DirectXInit* instance;

private:

	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12Device> dev = nullptr;
	Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapchain = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator = nullptr;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmdQueue = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeaps = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff = nullptr;

	//�o�b�N�o�b�t�@
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>backBuffers{2};

	//�f�X�N���v�^�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};

	//�L�[�{�[�h�f�o�C�X
	IDirectInputDevice8* keyboard = nullptr;

	//�t�F���X
	Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;
	UINT64 fenceVel = 0;

	//�S�L�[�̓��͏����擾����ׂ̕ϐ�
	BYTE key[256] = {};
	BYTE oldKey[256] = {};

	//�w�i�F�ύX���邽�߂ɊO�ɔz�u
	float clearColor[4]={0.1f,0.25f,0.5f,0.0f};//���ۂ��F(��ʃN���A����Ƃ��̐F)

	//���\�[�X�o���A
	D3D12_RESOURCE_BARRIER barrierDesc{};

	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> dsvHeap = nullptr;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

	IDXGIDebug* pDxgiDebug;

	Microsoft::WRL::ComPtr<ID3D12Debug> pD3dDebug;

};

