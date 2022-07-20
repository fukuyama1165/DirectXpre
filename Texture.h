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




//const float PI = 3.141592653589;

#include <DirectXTex.h>

//ComPtr�p�C���N���[�h
#include <wrl.h>

class Texture
{
public:
	Texture();
	~Texture();

	void Init(ID3D12Device* dev);

	void Draw(ID3D12GraphicsCommandList* cmdList);

	//�摜�C���[�W�f�[�^
	void imageDataGeneration();

	//�e�N�X�`���o�b�t�@
	void textureBuffGeneraion(ID3D12Device* dev);

	//�V�F�[�_���\�[�X�r���[
	void SRVGeneraion(ID3D12Device* dev);

public:

	

private:

	HRESULT result;

	UINT incremantSize;

	//�摜�f�[�^��
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	TexMetadata metadata2{};
	ScratchImage scratchImg2{};

	D3D12_RESOURCE_DESC textureResourceDesc{};
	D3D12_RESOURCE_DESC textureResourceDesc2{};

	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff2 = nullptr;

	
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> srvHeap = nullptr;
};

