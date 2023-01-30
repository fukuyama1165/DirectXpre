#include "DirectionalLight.h"

using namespace DirectX;

//静的メンバ変数の実体
ID3D12Device* DirectionalLight::dev = nullptr;

void DirectionalLight::StaticInitialize(ID3D12Device* device)
{

	//再初期化チェック
	assert(!DirectionalLight::dev);
	//nullptrチェック
	assert(device);
	//静的メンバ変数のセット
	DirectionalLight::dev = device;

}

DirectionalLight* DirectionalLight::Create()
{

	DirectionalLight* instance = new DirectionalLight();

	//初期化
	instance->Init();

	//生成したインスタンスを返す
	return instance;

}

void DirectionalLight::Init()
{

	constantBuffGeneration(dev);

	TransferConstBuffer();

}

void DirectionalLight::Update()
{


	//値の更新があった時だけ定数バッファに転送する
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

D3D12_RESOURCE_DESC DirectionalLight::constBuffResourceGeneration(int size)
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

void DirectionalLight::TransferConstBuffer()
{

	//マッピングするときのポインタ
	ConstBufferData* constMapData = nullptr;

	result = constBuff->Map(0, nullptr, (void**)&constMapData);
	if (SUCCEEDED(result))
	{
		constMapData->lightv = -lightDir;
		constMapData->lightColor = lightColor;
		constBuff->Unmap(0, nullptr);
	}

}