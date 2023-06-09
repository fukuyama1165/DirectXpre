#include "Texture.h"
#include <dxgidebug.h>
#include "DirectXInit.h"

Texture* Texture::instance = nullptr;

Texture::~Texture()
{
}

Texture* Texture::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Texture();
		instance->SRVHeapGeneraion();
	}

	return instance;
}

void Texture::SRVHeapGeneraion()
{

	const size_t kMaxSRVCount = 2056;

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//設定をもとにSRV用デスクリプタヒープを生成

	result_ = DirectXInit::GetInstance()->Getdev()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result_));

	//SRVヒープの先頭ハンドルを取得
	 srvHandle_ = srvHeap_->GetCPUDescriptorHandleForHeapStart();

}

void Texture::instanceDelete()
{
	delete(instance);
}

void Texture::Init()
{
	incremantSize_ = DirectXInit::GetInstance()->Getdev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//this->dev = dev;

	

}

uint32_t Texture::loadTexture(const char filename[])
{
	isReturn_ = false;
	sufferTexNum_ = 0;

	TexMetadata metadataBuff{};
	ScratchImage scratchImgBuff{};
	D3D12_RESOURCE_DESC textureResourceDescBuff{};
	

	imageDataGeneration(filename,metadataBuff,scratchImgBuff);
	
	

	if (isReturn_)
	{
		return sufferTexNum_;
	}
	//metadata.push_back(metadataBuff);
	//textureResourceDesc.push_back(textureResourceDescBuff);

	//テクスチャバッファ
	textureBuffGeneraion(metadataBuff,scratchImgBuff,textureResourceDescBuff);

	//シェーダリソースビュー
	
	
	
	//texBuff.push_back(newTexBuff);

	

	return (uint32_t)textureFileName_.size();
}

uint32_t Texture::loadTexture(std::string filename)
{
	isReturn_ = false;
	sufferTexNum_ = 0;

	TexMetadata metadataBuff{};
	ScratchImage scratchImgBuff{};
	D3D12_RESOURCE_DESC textureResourceDescBuff{};
	

	imageDataGeneration(filename, metadataBuff, scratchImgBuff);
	

	if (isReturn_)
	{
		return sufferTexNum_;
	}
	//テクスチャバッファ
	textureBuffGeneraion(metadataBuff, scratchImgBuff, textureResourceDescBuff);

	return (uint32_t)textureFileName_.size();
}

void Texture::Draw(uint32_t tex)
{
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;

	//SRVヒープの設定コマンド
	DirectXInit::GetInstance()->GetcmdList()->SetDescriptorHeaps(1, srvHeap_.GetAddressOf());
	//SRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	srvGpuHandle = srvHeap_->GetGPUDescriptorHandleForHeapStart();

	//metadetaのsizeを返しているはずなので0にならないはずだけど一応
	if (tex == 0)
	{
		tex = 1;
	}

	//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	{
		srvGpuHandle.ptr += incremantSize_ * (tex);
		DirectXInit::GetInstance()->GetcmdList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
	}

}

void Texture::imageDataGeneration(const char filename[])
{
#pragma region 画像イメージデータの作成

	//画像読み込みして画像イメージデータを生成

	TexMetadata metadataBuff{};
	ScratchImage scratchImgBuff{};

	std::string filepath = filename;

	wchar_t wfilepath[128] = {};
	MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	//WICテクスチャのロード
	result_ = LoadFromWICFile(
		wfilepath,
		WIC_FLAGS_NONE,
		&metadataBuff,
		scratchImgBuff
	);

	ScratchImage mipChain{};
	//ミップマップの生成
	result_ = GenerateMipMaps(
		scratchImgBuff.GetImages(),
		scratchImgBuff.GetImageCount(),
		scratchImgBuff.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain
	);

	if (SUCCEEDED(result_))
	{
		scratchImg_.push_back(std::move(mipChain));
		metadataBuff = scratchImgBuff.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadataBuff.format = MakeSRGB(metadataBuff.format);

	metadata_.push_back(metadataBuff);

#pragma endregion
}

void Texture::imageDataGeneration(std::string filename)

{
#pragma region 画像イメージデータの作成

	//画像読み込みして画像イメージデータを生成

	TexMetadata metadataBuff{};
	ScratchImage scratchImgBuff{};

	wchar_t wfilepath[128] = {};
	MultiByteToWideChar(CP_ACP, 0, filename.c_str(), -1, wfilepath, _countof(wfilepath));

	//WICテクスチャのロード
	result_ = LoadFromWICFile(
		wfilepath,
		WIC_FLAGS_NONE,
		&metadataBuff,
		scratchImgBuff
	);

	ScratchImage mipChain{};
	//ミップマップの生成
	result_ = GenerateMipMaps(
		scratchImgBuff.GetImages(),
		scratchImgBuff.GetImageCount(),
		scratchImgBuff.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain
	);

	if (SUCCEEDED(result_))
	{
		scratchImg_.push_back(std::move(mipChain));
		metadataBuff = scratchImgBuff.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadataBuff.format = MakeSRGB(metadataBuff.format);

	metadata_.push_back(metadataBuff);

#pragma endregion
}

void Texture::textureBuffGeneraion()
{
#pragma region テクスチャバッファ設定

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//リソース設定
	for (uint16_t i = 0; i < metadata_.size(); i++)
	{

		D3D12_RESOURCE_DESC textureResourceDescBuff{};
		textureResourceDescBuff.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureResourceDescBuff.Format = metadata_[i].format;
		textureResourceDescBuff.Width = metadata_[i].width;//幅
		textureResourceDescBuff.Height = (uint32_t)metadata_[i].height;//高さ
		textureResourceDescBuff.DepthOrArraySize = (uint16_t)metadata_[i].arraySize;
		textureResourceDescBuff.MipLevels = (uint16_t)metadata_[i].mipLevels;
		textureResourceDescBuff.SampleDesc.Count = 1;

		if (textureResourceDesc_.size() - 1 != i)
		{
			textureResourceDesc_.push_back(textureResourceDescBuff);
		}
	}

#pragma endregion

#pragma region テクスチャバッファの生成

	//テクスチャバッファの生成



	for (uint16_t i = 0; i < metadata_.size(); i++)
	{

		Microsoft::WRL::ComPtr<ID3D12Resource > newTexBuff;

		result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc_[i],
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&newTexBuff)
		);

		//ミップマップで置き換える
		//全ミップマップについて
		for (size_t j = 0; j < metadata_[i].mipLevels; j++)
		{
			//ミップマップレベルを指定してイメージを取得
			const Image* img = scratchImg_[i].GetImage(j, 0, 0);
			//テクスチャバッファにデータ転送
			result_ = newTexBuff->WriteToSubresource(
				(uint32_t)j,
				nullptr,//全領域へコピー
				img->pixels,//元データアドレス
				(uint32_t)img->rowPitch,//1ラインアドレス
				(uint32_t)img->slicePitch//1枚サイズ
			);
			assert(SUCCEEDED(result_));
		}

		if (texBuff_.size() - 1 != i)
		{
			texBuff_.push_back(newTexBuff);
		}
	}

#pragma endregion 
}

void Texture::imageDataGeneration(const char filename[], TexMetadata& metadataBuff, ScratchImage& scratchImgBuff)
{
#pragma region 画像イメージデータの作成

	//画像読み込みして画像イメージデータを生成

	std::string filepath = filename;

	for (uint16_t i = 0; i < textureFileName_.size(); i++)
	{
		if (textureFileName_[i] == filepath)
		{
			isReturn_ = true;
			sufferTexNum_ = i+1;
			return;
		}
	}

	wchar_t wfilepath[128];
	MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	//WICテクスチャのロード
	result_ = LoadFromWICFile(
		wfilepath,
		WIC_FLAGS_NONE,
		&metadataBuff,
		scratchImgBuff
	);

	ScratchImage mipChain{};
	//ミップマップの生成
	result_ = GenerateMipMaps(
		scratchImgBuff.GetImages(),
		scratchImgBuff.GetImageCount(),
		scratchImgBuff.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain
	);

	if (SUCCEEDED(result_))
	{
		scratchImgBuff = std::move(mipChain);
		//scratchImg_.push_back(std::move(mipChain));
		metadataBuff = scratchImgBuff.GetMetadata();
		textureFileName_.push_back(filename);
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadataBuff.format = MakeSRGB(metadataBuff.format);

#pragma endregion
}

void Texture::imageDataGeneration(std::string filename, TexMetadata& metadataBuff, ScratchImage& scratchImgBuff)
{
	{
#pragma region 画像イメージデータの作成

		//画像読み込みして画像イメージデータを生成

		for (uint16_t i = 0; i < textureFileName_.size(); i++)
		{
			if (textureFileName_[i] == filename)
			{
				isReturn_ = true;
				sufferTexNum_ = i+1;
				return;
			}
		}

		wchar_t wfilepath[128];
		MultiByteToWideChar(CP_ACP, 0, filename.c_str(), -1, wfilepath, _countof(wfilepath));

		//WICテクスチャのロード
		result_ = LoadFromWICFile(
			wfilepath,
			WIC_FLAGS_NONE,
			&metadataBuff,
			scratchImgBuff
		);

		ScratchImage mipChain{};
		//ミップマップの生成
		result_ = GenerateMipMaps(
			scratchImgBuff.GetImages(),
			scratchImgBuff.GetImageCount(),
			scratchImgBuff.GetMetadata(),
			TEX_FILTER_DEFAULT,
			0,
			mipChain
		);

		if (SUCCEEDED(result_))
		{
			scratchImgBuff = std::move(mipChain);
			//scratchImg_.push_back(scratchImgBuff);
			metadataBuff = scratchImgBuff.GetMetadata();
			textureFileName_.push_back(filename);
		}

		//読み込んだディフューズテクスチャをSRGBとして扱う
		metadataBuff.format = MakeSRGB(metadataBuff.format);

#pragma endregion
	}
}

void Texture::textureBuffGeneraion(TexMetadata& metadataBuff, ScratchImage& scratchImgBuff, D3D12_RESOURCE_DESC& textureResourceDescBuff)
{
#pragma region テクスチャバッファ設定

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//リソース設定

	textureResourceDescBuff.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDescBuff.Format = metadataBuff.format;
	textureResourceDescBuff.Width = metadataBuff.width;//幅
	textureResourceDescBuff.Height = (uint32_t)metadataBuff.height;//高さ
	textureResourceDescBuff.DepthOrArraySize = (uint16_t)metadataBuff.arraySize;
	textureResourceDescBuff.MipLevels = (uint16_t)metadataBuff.mipLevels;
	textureResourceDescBuff.SampleDesc.Count = 1;



#pragma endregion

#pragma region テクスチャバッファの生成

	//テクスチャバッファの生成

	//Microsoft::WRL::ComPtr<ID3D12Resource > newTexBuff;

		result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDescBuff,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&texBuff2_)
		);

		//ミップマップで置き換える
		//全ミップマップについて
		for (size_t j = 0; j < metadataBuff.mipLevels; j++)
		{
			//ミップマップレベルを指定してイメージを取得
			const Image* img = scratchImgBuff.GetImage(j, 0, 0);
			//テクスチャバッファにデータ転送
			result_ = texBuff2_->WriteToSubresource(
				(uint32_t)j,
				nullptr,//全領域へコピー
				img->pixels,//元データアドレス
				(uint32_t)img->rowPitch,//1ラインアドレス
				(uint32_t)img->slicePitch//1枚サイズ
			);
			assert(SUCCEEDED(result_));
		}

		texBuff_.push_back(texBuff2_);

		SRVGeneraion(textureResourceDescBuff, texBuff2_);
		
#pragma endregion 
}


void Texture::SRVGeneraion(const D3D12_RESOURCE_DESC& TextureResourceDesc, Microsoft::WRL::ComPtr<ID3D12Resource> newTexBuff)
{
#pragma region シェーダーリソースビューの為のデスクリプタヒープ生成

	//デスクリプタハンドル(ヒープ内の操作する場所指定に使う)
	//SRVの最大個数
	

#pragma endregion

#pragma region シェーダリソースビュー

	
	srvHandle_.ptr += incremantSize_;
	//シェーダリソースビュー
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	srvDesc.Format = TextureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = TextureResourceDesc.MipLevels;

	//ハンドルの指す位置にシェーダリソースビュー作成
	DirectXInit::GetInstance()->Getdev()->CreateShaderResourceView(newTexBuff.Get(), &srvDesc, srvHandle_);

	

#pragma endregion
}



