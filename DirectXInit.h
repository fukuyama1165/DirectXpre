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



class DirectXInit
{
public:
	DirectXInit();
	~DirectXInit();

	void Init(WNDCLASSEX w, HWND hwnd);

	//�O���t�B�b�N�A�_�v�^�[
	void GraphicAdapterGeneration();

	//�R�}���h���X�g
	void CommandListGeneration();

	//�R�}���h�L���[
	void CommandQueueGeneration();

	//�X���b�v�`�F�[��
	void SwapChainGeneration(HWND hwnd);

	//�����_�[�^�[�Q�b�g�r���[
	void RTVGeneration();

	//�t�F���X
	void FenceGeneration();

	//DirectInput(�ł������p�N���X�����)
	void DirectInputGeneration(WNDCLASSEX w, HWND hwnd);

	//dev��Ԃ��֐�
	ID3D12Device* Getdev();

	//keyboard��Ԃ��֐�
	IDirectInputDevice8* GetKeyBoard();

	ID3D12GraphicsCommandList* GetcmdList();

	void DrawStart();

	void DrawEnd();


	//��ʂ̃N���A�J���[�ύX�֐�
	void clearColorChange(float R, float G, float B, float A);

	//input�ł̓��͔����֐�(inputClass��������炱����ړ�����)

	bool PushKey(BYTE CheckKey);

	bool TriggerKey(BYTE CheckKey);

private:

	HRESULT result;
	ID3D12Device* dev = nullptr;
	IDXGIFactory6* dxgiFactory = nullptr;
	IDXGISwapChain4* swapchain = nullptr;
	ID3D12CommandAllocator* cmdAllocator = nullptr;
	ID3D12GraphicsCommandList* cmdList = nullptr;
	ID3D12CommandQueue* cmdQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeaps = nullptr;

	

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};

	IDirectInputDevice8* keyboard = nullptr;

	
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVel = 0;

	//�S�L�[�̓��͏����擾����ׂ̕ϐ�
	BYTE key[256] = {};
	BYTE oldKey[256] = {};

	//�w�i�F�ύX���邽�߂ɊO�ɔz�u
	float clearColor[4];//���ۂ��F(��ʃN���A����Ƃ��̐F)

	D3D12_RESOURCE_BARRIER barrierDesc{};

};

