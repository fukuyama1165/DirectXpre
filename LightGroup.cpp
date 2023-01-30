#include "LightGroup.h"

using namespace DirectX;

ID3D12Device* LightGroup::dev = nullptr;

void LightGroup::Staticlnitialize(ID3D12Device* device)
{

	assert(!LightGroup::dev);

	assert(device);

	LightGroup::dev = device;

}

LightGroup* LightGroup::Create()
{

	LightGroup* instance = new LightGroup();

	instance->Init();

	return instance;

}

void LightGroup::Init()
{
	DefaultLightSetting();

	constantBuffGeneration(dev);

	TransferConstBuffer();

}

void LightGroup::Update()
{

	if (dirty)
	{

		TransferConstBuffer();
		dirty = false;

	}

}

void LightGroup::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{

	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff->GetGPUVirtualAddress());

}

void LightGroup::constantBuffGeneration(ID3D12Device* dev)
{
#pragma region 定数バッファ

	//定数バッファの生成用の設定

	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用

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

D3D12_RESOURCE_DESC LightGroup::constBuffResourceGeneration(int size)
{
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (size + 0xff) & ~0xff;//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	return cbResourceDesc;
}

void LightGroup::TransferConstBuffer()
{

	//マッピングするときのポインタ
	ConstBufferData* constMapData = nullptr;

	result = constBuff->Map(0, nullptr, (void**)&constMapData);
	if (SUCCEEDED(result))
	{
		constMapData->ambientColor = ambientColor;

		for (int i = 0; i < DirLightNum; i++)
		{

			if (dirLights[i].GetIsActive())
			{
				constMapData->dirLights[i].active = 1;
				constMapData->dirLights[i].lightv = -dirLights[i].GetLightDir();
				constMapData->dirLights[i].lightColor = dirLights[i].GetLightColor();
			}
			else
			{

				constMapData->dirLights[i].active = 0;

			}

		}

		constBuff->Unmap(0, nullptr);
	}

}

void LightGroup::SetAmbientColor(const XMFLOAT3& color)
{

	ambientColor = color;
	dirty = true;

}

void LightGroup::SetDirLightActive(int index, bool active)
{

	assert(0 <= index and index < DirLightNum);

	dirLights[index].SetIsActive(active);

}

void LightGroup::SetDirLightDir(int index, const XMVECTOR& lightdir)
{

	assert(0 <= index and index < DirLightNum);

	dirLights[index].SetLightDir(lightdir);

	dirty = true;

}

void LightGroup::SetDirLightColor(int index, const XMFLOAT3& lightcolor)
{

	assert(0 <= index and index < DirLightNum);

	dirLights[index].SetLightColor(lightcolor);

	dirty = true;

}

void LightGroup::DefaultLightSetting()
{

	dirLights[0].SetIsActive(true);
	dirLights[0].SetLightColor({1.0f,1.0f,1.0f});
	dirLights[0].SetLightDir({0.0f,-1.0f,0.0f,0});

	dirLights[1].SetIsActive(true);
	dirLights[1].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights[1].SetLightDir({ +0.5f,+0.1f,+0.2f,0 });

	dirLights[2].SetIsActive(true);
	dirLights[2].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights[2].SetLightDir({ -0.5f,+0.1f,-0.2f,0 });

}