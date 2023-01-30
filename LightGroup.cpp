#include "LightGroup.h"

using namespace DirectX;

ID3D12Device* LightGroup::dev = nullptr;

void LightGroup::Staticlnitialize(ID3D12Device* device)
{

	assert(!LightGroup::dev);

	assert(device);

	LightGroup::dev = device;

}

void LightGroup::Init()
{

	constantBuffGeneration(dev);

	TransferConstBuffer();

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

}

void LightGroup::SetDirLightActive(int index, bool active)
{

}

void LightGroup::SetDirLightDir(int index, const XMVECTOR& lightdir)
{

}