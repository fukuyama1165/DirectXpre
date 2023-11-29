#include "LightGroup.h"
#include "DirectXInit.h"

using namespace DirectX;

//Microsoft::WRL::ComPtr<ID3D12Device> LightGroup::dev = nullptr;

void LightGroup::Staticlnitialize()
{

	//LightGroup::dev = DirectXInit::GetInstance()->Getdev();

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
#pragma region 定数バッファ

	//定数バッファの生成用の設定

	cbHeapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用

	cbResourceDesc_ = constBuffResourceGeneration(sizeof(ConstBufferData));

	result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
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

		for (uint16_t i = 0; i < SPointLightNum_; i++)
		{
			//有効ならデータを送る
			if (pointLights_[i].IsActive())
			{
				constMapData->pointLights[i].active = 1;
				constMapData->pointLights[i].lightPos = pointLights_[i].GetLightPos();
				constMapData->pointLights[i].lightColor = pointLights_[i].GetLightColor();
				constMapData->pointLights[i].lightAtten = pointLights_[i].GetLightAtten();
			}
			else
			{
				constMapData->pointLights[i].active = 0;
			}
		}

		for (uint16_t i = 0; i < SSpotLightNum_; i++)
		{
			//有効ならデータを送る
			if (SpotLights_[i].IsActive())
			{
				constMapData->spotLights[i].active = 1000;
				constMapData->spotLights[i].lightV = SpotLights_[i].GetLightDir();
				constMapData->spotLights[i].lightPos = SpotLights_[i].GetLightPos();
				constMapData->spotLights[i].lightColor = SpotLights_[i].GetLightColor();
				constMapData->spotLights[i].lightAtten = SpotLights_[i].GetLightAtten();
				constMapData->spotLights[i].lightFactorAhgleCos = { SpotLights_[i].GetLightFactorAhgleCos().x, SpotLights_[i].GetLightFactorAhgleCos().y,0 };
			}
			else
			{
				constMapData->spotLights[i].active = 0;
			}
		}

		constBuff_->Unmap(0, nullptr);
	}

}

void LightGroup::SetAmbientColor(const Vector3& color)
{

	ambientColor_ = color;
	dirty_ = true;

}

void LightGroup::SetDirLightActive(uint32_t index, bool active)
{

	assert(0 <= index && index < SDirLightNum_);

	dirLights_[index].SetIsActive(active);

}

void LightGroup::SetDirLightDir(uint32_t index, const Vector4& lightdir)
{

	assert(0 <= index && index < SDirLightNum_);

	dirLights_[index].SetLightDir(lightdir);

	dirty_ = true;

}

void LightGroup::SetDirLightColor(uint32_t index, const Vector3& lightcolor)
{

	assert(0 <= index && index < SDirLightNum_);

	dirLights_[index].SetLightColor(lightcolor);

	dirty_ = true;

}

void LightGroup::SetPointLightActive(uint32_t index, bool active)
{

	assert(0 <= index && index < SPointLightNum_);

	pointLights_[index].SetIsActive(active);

}

void LightGroup::SetPointLightPos(uint32_t index, const Vector3& lightPos)
{

	assert(0 <= index && index < SPointLightNum_);

	pointLights_[index].SetLightPos(lightPos);

	dirty_ = true;

}

void LightGroup::SetPointLightColor(uint32_t index, const Vector3& lightColor)
{

	assert(0 <= index && index < SPointLightNum_);

	pointLights_[index].SetLightColor(lightColor);

	dirty_ = true;

}

void LightGroup::SetPointLightAtten(uint32_t index, const Vector3& lightAtten)
{

	assert(0 <= index && index < SPointLightNum_);

	pointLights_[index].SetLightAtten(lightAtten);

	dirty_ = true;

}


void LightGroup::SetSpotLightActive(uint32_t index, bool active)
{

	assert(0 <= index && index < SSpotLightNum_);

	SpotLights_[index].SetIsActive(active);

}

void LightGroup::SetSpotLightDir(uint32_t index, const Vector3& lightdir)
{

	assert(0 <= index && index < SSpotLightNum_);

	SpotLights_[index].SetLightDir(lightdir);

	dirty_ = true;

}

void LightGroup::SetSpotLightPos(uint32_t index, const Vector3& lightPos)
{

	assert(0 <= index && index < SSpotLightNum_);

	SpotLights_[index].SetLightPos(lightPos);

	dirty_ = true;

}

void LightGroup::SetSpotLightColor(uint32_t index, const Vector3& lightColor)
{

	assert(0 <= index && index < SSpotLightNum_);

	SpotLights_[index].SetLightColor(lightColor);

	dirty_ = true;

}

void LightGroup::SetSpotLightAtten(uint32_t index, const Vector3& lightAtten)
{

	assert(0 <= index && index < SSpotLightNum_);

	SpotLights_[index].SetLightAtten(lightAtten);

	dirty_ = true;

}

void LightGroup::SetSpotLightFactorAngle(uint32_t index, const Vector2& lightFactorAngle)
{

	assert(0 <= index && index < SSpotLightNum_);

	SpotLights_[index].SetLightFactorAhgle(lightFactorAngle);

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