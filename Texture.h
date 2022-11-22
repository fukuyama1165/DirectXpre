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


#include <vector>

//const float PI = 3.141592653589;

#include <DirectXTex.h>

//ComPtr�p�C���N���[�h
#include <wrl.h>

class Texture
{
public:
	
	static Texture* GetInstance();

	void Init(ID3D12Device* dev);

	/// <summary>
	/// �e�N�X�`���[�`��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	/// <param name="tex">loadTexture�ŋA���Ă����l</param>
	void Draw(ID3D12GraphicsCommandList* cmdList,int tex);

	//�摜�C���[�W�f�[�^
	void imageDataGeneration(const char filename[]);

	//�e�N�X�`���o�b�t�@
	void textureBuffGeneraion(ID3D12Device* dev);

	//�V�F�[�_���\�[�X�r���[
	void SRVGeneraion(ID3D12Device* dev);

	void instanceDelete();

	int loadTexture(const char filename[]);

private:

	Texture() = default;
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	static Texture* instance;

public:

	

private:

	HRESULT result;

	UINT incremantSize;

	Microsoft::WRL::ComPtr <ID3D12Device> dev;

	//�摜�f�[�^��
	std::vector<TexMetadata> metadata{};
	std::vector<ScratchImage> scratchImg{};
	TexMetadata metadata2{};
	ScratchImage scratchImg2{};

	std::vector < D3D12_RESOURCE_DESC> textureResourceDesc{};
	D3D12_RESOURCE_DESC textureResourceDesc2{};

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> texBuff;
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff2 = nullptr;

	
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> srvHeap = nullptr;
};

