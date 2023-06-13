#include "LightGroup.h"
#include "DirectXInit.h"

using namespace DirectX;

Microsoft::WRL::ComPtr<ID3D12Device> LightGroup::dev = nullptr;

void LightGroup::Staticlnitialize()
{

	LightGroup::dev = DirectXInit::GetInstance()->Getdev();

}

//LightGroup* LightGroup::Create()
//{
//
//	LightGroup* instance = new LightGroup();
//
//	instance->Init();
//
//	return instance;
//
//}

void LightGroup::Init()
{
	DefaultLightSetting();

	constantBuffGeneration();

	TransferConstBuffer();

}

void LightGroup::Update()
{

	if (dirty_)
	{

		TransferConstBuffer();
		dirty_ = false;

	}

}

void LightGroup::Draw(uint32_t rootParameterIndex)
{

	DirectXInit::GetInstance()->GetcmdList()->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff_->GetGPUVirtualAddress());

}

void LightGroup::constantBuffGeneration()
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

D3D12_RESOURCE_DESC LightGroup::constBuffResourceGeneration(uint32_t size)
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

void LightGroup::TransferConstBuffer()
{

	//�}�b�s���O����Ƃ��̃|�C���^
	ConstBufferData* constMapData = nullptr;

	result_ = constBuff_->Map(0, nullptr, (void**)&constMapData);
	if (SUCCEEDED(result_))
	{
		constMapData->ambientColor = ambientColor_;

		for (uint16_t i = 0; i < SDirLightNum_; i++)
		{

			if (dirLights_[i].GetIsActive())
			{
				constMapData->dirLights[i].active = 1;
				constMapData->dirLights[i].lightv = -dirLights_[i].GetLightDir();
				constMapData->dirLights[i].lightColor = dirLights_[i].GetLightColor();
			}
			else
			{

				constMapData->dirLights[i].active = 0;

			}

		}

		constBuff_->Unmap(0, nullptr);
	}

}

void LightGroup::SetAmbientColor(const XMFLOAT3& color)
{

	ambientColor_ = color;
	dirty_ = true;

}

void LightGroup::SetDirLightActive(uint32_t index, bool active)
{

	assert(0 <= index and index < SDirLightNum_);

	dirLights_[index].SetIsActive(active);

}

void LightGroup::SetDirLightDir(uint32_t index, const XMVECTOR& lightdir)
{

	assert(0 <= index and index < SDirLightNum_);

	dirLights_[index].SetLightDir(lightdir);

	dirty_ = true;

}

void LightGroup::SetDirLightColor(uint32_t index, const XMFLOAT3& lightcolor)
{

	assert(0 <= index and index < SDirLightNum_);

	dirLights_[index].SetLightColor(lightcolor);

	dirty_ = true;

}

void LightGroup::DefaultLightSetting()
{

	dirLights_[0].SetIsActive(true);
	dirLights_[0].SetLightColor({1.0f,1.0f,1.0f});
	dirLights_[0].SetLightDir({0.0f,-1.0f,0.0f,0});

	dirLights_[1].SetIsActive(true);
	dirLights_[1].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights_[1].SetLightDir({ +0.5f,+0.1f,+0.2f,0 });

	dirLights_[2].SetIsActive(true);
	dirLights_[2].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights_[2].SetLightDir({ -0.5f,+0.1f,-0.2f,0 });

}