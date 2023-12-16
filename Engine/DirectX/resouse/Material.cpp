#include "Material.h"

Material::Material()
{
}

Material::~Material()
{
}

void Material::Update()
{
	constMapB1_->ambient_ = material_.ambient_;
	constMapB1_->diffuse_ = material_.diffuse_;
	constMapB1_->specular_ = material_.specular_;
	constMapB1_->alpha_ = material_.alpha_;
	constMapB1_->tile = material_.tile_;
}

void Material::MaterialConstBuffInit()
{
	cbHeapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataB1) + 0xff) & ~0xff;//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
		&cbHeapProp_,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB1_));
	assert(SUCCEEDED(result_));

	result_ = constBuffB1_->Map(0, nullptr, (void**)&constMapB1_);
	assert(SUCCEEDED(result_));
	constMapB1_->ambient_ = material_.ambient_;
	constMapB1_->diffuse_ = material_.diffuse_;
	constMapB1_->specular_ = material_.specular_;
	constMapB1_->alpha_ = material_.alpha_;
	constMapB1_->tile = material_.tile_;
}