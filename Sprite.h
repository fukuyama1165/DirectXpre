#pragma once
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include<d3d12.h>
#include<dxgi1_6.h>

//���w���C�u����(�œK�����ꂷ���Ďg���ɂ����炵���̂ł��ƂŎ��삵�����)
#include<DirectXMath.h>
using namespace DirectX;

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "matrix4x4.h"
#include "Float3.h"

//ComPtr�p�C���N���[�h
#include <wrl.h>

#include "SpriteCommon.h"

class Sprite
{
public:
	void initialize(ID3D12Device* dev);

	void vertexBuffGeneration(ID3D12Device* dev);

	void Draw(ID3D12GraphicsCommandList* cmdList,SpriteCommon& spriteCommon, bool PipeLineRuleFlag);

private:

	XMFLOAT3 vertices[3];

	UINT sizeVB;

	HRESULT result;

	//���_�o�b�t�@�p�ϐ�
	D3D12_HEAP_PROPERTIES heapprop{};//�q�[�v�ݒ�
	D3D12_RESOURCE_DESC resDesc{};//���\�[�X�ݒ�
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
	XMFLOAT3* vertMap = nullptr;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};

};

