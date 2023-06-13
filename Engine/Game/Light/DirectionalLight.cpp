#include "DirectionalLight.h"
#include "DirectXInit.h"

using namespace DirectX;

//�ÓI�����o�ϐ��̎���
Microsoft::WRL::ComPtr<ID3D12Device> DirectionalLight::dev = nullptr;

void DirectionalLight::StaticInitialize()
{

	dev = DirectXInit::GetInstance()->Getdev();

}

//DirectionalLight* DirectionalLight::Create()
//{
//
//	DirectionalLight* instance = new DirectionalLight();
//
//	//������
//	instance->Init();
//
//	//���������C���X�^���X��Ԃ�
//	return instance;
//
//}

void DirectionalLight::Init()
{

	constantBuffGeneration();

	TransferConstBuffer();

}

void DirectionalLight::Update()
{


	//�l�̍X�V���������������萔�o�b�t�@�ɓ]������
	if (dirty_)
	{
		TransferConstBuffer();
		dirty_ = false;
	}

}

void DirectionalLight::Draw(uint32_t rootParameterIndex)
{

	DirectXInit::GetInstance()->GetcmdList()->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff_->GetGPUVirtualAddress());

}

void DirectionalLight::SetLightDir(const XMVECTOR& lightdir)
{

	lightDir_ = XMVector3Normalize(lightdir);
	dirty_ = true;

}

void DirectionalLight::SetLightColor(const XMFLOAT3& lightcolor)
{

	lightColor_ = lightcolor;
	dirty_ = true;

}

void DirectionalLight::constantBuffGeneration()
{
#pragma region �萔�o�b�t�@

	//�萔�o�b�t�@�̐����p�̐ݒ�

	cbHeapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p

	cbResourceDesc_ = constBuffResourceGeneration(sizeof(ConstBufferData));

	result_ = dev->CreateCommittedResource(
		&cbHeapProp_,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result_));

	

#pragma endregion
}

D3D12_RESOURCE_DESC DirectionalLight::constBuffResourceGeneration(uint32_t size)
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

	result_ = constBuff_->Map(0, nullptr, (void**)&constMapData);
	if (SUCCEEDED(result_))
	{
		constMapData->lightv = -lightDir_;
		constMapData->lightColor = lightColor_;
		constBuff_->Unmap(0, nullptr);
	}

}