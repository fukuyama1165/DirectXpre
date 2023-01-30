#include "DirectionalLight.h"

using namespace DirectX;

//�ÓI�����o�ϐ��̎���
ID3D12Device* DirectionalLight::dev = nullptr;

void DirectionalLight::StaticInitialize(ID3D12Device* device)
{

	//�ď������`�F�b�N
	assert(!DirectionalLight::dev);
	//nullptr�`�F�b�N
	assert(device);
	//�ÓI�����o�ϐ��̃Z�b�g
	DirectionalLight::dev = device;

}

DirectionalLight* DirectionalLight::Create()
{

	DirectionalLight* instance = new DirectionalLight();

	//������
	instance->Init();

	//���������C���X�^���X��Ԃ�
	return instance;

}

void DirectionalLight::Init()
{

	constantBuffGeneration(dev);

	TransferConstBuffer();

}

void DirectionalLight::Update()
{


	//�l�̍X�V���������������萔�o�b�t�@�ɓ]������
	if (dirty)
	{
		TransferConstBuffer();
		dirty = false;
	}

}

void DirectionalLight::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{

	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff->GetGPUVirtualAddress());

}

void DirectionalLight::SetLightDir(const XMVECTOR& lightdir)
{

	this->lightDir = XMVector3Normalize(lightdir);
	dirty = true;

}

void DirectionalLight::SetLightColor(const XMFLOAT3& lightcolor)
{

	this->lightColor = lightcolor;
	dirty = true;

}

void DirectionalLight::constantBuffGeneration(ID3D12Device* dev)
{
#pragma region �萔�o�b�t�@

	//�萔�o�b�t�@�̐����p�̐ݒ�

	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p

	cbResourceDesc = constBuffResourceGeneration(sizeof(ConstBufferData));

	result = dev->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	

#pragma endregion
}

D3D12_RESOURCE_DESC DirectionalLight::constBuffResourceGeneration(int size)
{
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (size + 0xff) & ~0xff;//256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	return cbResourceDesc;
}

void DirectionalLight::TransferConstBuffer()
{

	//�}�b�s���O����Ƃ��̃|�C���^
	ConstBufferData* constMapData = nullptr;

	result = constBuff->Map(0, nullptr, (void**)&constMapData);
	if (SUCCEEDED(result))
	{
		constMapData->lightv = -lightDir;
		constMapData->lightColor = lightColor;
		constBuff->Unmap(0, nullptr);
	}

}