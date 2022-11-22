#include "Texture.h"

Texture* Texture::instance = nullptr;

Texture::~Texture()
{
}

Texture* Texture::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Texture();
	}

	return instance;
}

void Texture::instanceDelete()
{
		delete(instance);
}

void Texture::Init(ID3D12Device* dev)
{
	incremantSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	this->dev = dev;
	
}

int Texture::loadTexture(const char filename[])
{
	imageDataGeneration(filename);

	//テクスチャバッファ
	textureBuffGeneraion(dev.Get());

	//シェーダリソースビュー
	SRVGeneraion(dev.Get());

	return metadata.size();
}

void Texture::Draw(ID3D12GraphicsCommandList* cmdList, int tex)
{
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;

	//SRVヒープの設定コマンド
	cmdList->SetDescriptorHeaps(1, srvHeap.GetAddressOf());
	//SRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

	//SRVヒープの先頭にあるSRVをルートパラメータ２番に設定
	{
		srvGpuHandle.ptr += incremantSize*(tex-1);
		cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
	}

}

void Texture::imageDataGeneration(const char filename[])
{
#pragma region 画像イメージデータの作成

	//画像読み込みして画像イメージデータを生成

	TexMetadata metadataBuff{};
	ScratchImage scratchImgBuff{};

	std::string filepath = filename;

	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	//WICテクスチャのロード
	result = LoadFromWICFile(
		wfilepath,
		WIC_FLAGS_NONE,
		&metadataBuff,
		scratchImgBuff
	);

	ScratchImage mipChain{};
	//ミップマップの生成
	result = GenerateMipMaps(
		scratchImgBuff.GetImages(),
		scratchImgBuff.GetImageCount(),
		scratchImgBuff.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain
	);

	if (SUCCEEDED(result))
	{
		scratchImg.push_back(std::move(mipChain));
		metadataBuff=scratchImgBuff.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadataBuff.format = MakeSRGB(metadataBuff.format);

	metadata.push_back(metadataBuff);

	////WICテクスチャのロード
	//result = LoadFromWICFile(
	//	L"Resources/basketballman2.png",
	//	WIC_FLAGS_NONE,
	//	&metadata2,
	//	scratchImg2
	//);

	//ScratchImage mipChain2{};
	////ミップマップの生成
	//result = GenerateMipMaps(
	//	scratchImg2.GetImages(),
	//	scratchImg2.GetImageCount(),
	//	scratchImg2.GetMetadata(),
	//	TEX_FILTER_DEFAULT,
	//	0,
	//	mipChain2
	//);

	//if (SUCCEEDED(result))
	//{
	//	scratchImg2 = std::move(mipChain2);
	//	metadata2 = scratchImg2.GetMetadata();
	//}

	////読み込んだディフューズテクスチャをSRGBとして扱う
	//metadata2.format = MakeSRGB(metadata2.format);

#pragma endregion
}

void Texture::textureBuffGeneraion(ID3D12Device* dev)
{
#pragma region テクスチャバッファ設定

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//リソース設定
	for (int i = 0; i < metadata.size(); i++)
	{

		D3D12_RESOURCE_DESC textureResourceDescBuff{};
		textureResourceDescBuff.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureResourceDescBuff.Format = metadata[i].format;
		textureResourceDescBuff.Width = metadata[i].width;//幅
		textureResourceDescBuff.Height = (UINT)metadata[i].height;//高さ
		textureResourceDescBuff.DepthOrArraySize = (UINT16)metadata[i].arraySize;
		textureResourceDescBuff.MipLevels = (UINT16)metadata[i].mipLevels;
		textureResourceDescBuff.SampleDesc.Count = 1;

		if (textureResourceDesc.size() - 1 != i)
		{
			textureResourceDesc.push_back(textureResourceDescBuff);
		}
	}

	
	//textureResourceDesc2.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//textureResourceDesc2.Format = metadata2.format;
	//textureResourceDesc2.Width = metadata2.width;//幅
	//textureResourceDesc2.Height = (UINT)metadata2.height;//高さ
	//textureResourceDesc2.DepthOrArraySize = (UINT16)metadata2.arraySize;
	//textureResourceDesc2.MipLevels = (UINT16)metadata2.mipLevels;
	//textureResourceDesc2.SampleDesc.Count = 1;

#pragma endregion

#pragma region テクスチャバッファの生成

	//テクスチャバッファの生成

	

	for (int i = 0; i < metadata.size(); i++)
	{

		Microsoft::WRL::ComPtr<ID3D12Resource > newTexBuff;

		result = dev->CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureResourceDesc[i],
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&newTexBuff)
		);

		//ミップマップで置き換える
		//全ミップマップについて
		for (size_t j = 0; j < metadata[i].mipLevels; j++)
		{
			//ミップマップレベルを指定してイメージを取得
			const Image* img = scratchImg[i].GetImage(j, 0, 0);
			//テクスチャバッファにデータ転送
			result = newTexBuff->WriteToSubresource(
				(UINT)j,
				nullptr,//全領域へコピー
				img->pixels,//元データアドレス
				(UINT)img->rowPitch,//1ラインアドレス
				(UINT)img->slicePitch//1枚サイズ
			);
			assert(SUCCEEDED(result));
		}

		if (texBuff.size() - 1 != i)
		{
			texBuff.push_back(newTexBuff);
		}
	}

	//result = dev->CreateCommittedResource(
	//	&textureHeapProp,
	//	D3D12_HEAP_FLAG_NONE,
	//	&textureResourceDesc2,
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&texBuff2)
	//);

	////ミップマップで置き換える
	////全ミップマップについて
	//for (size_t i = 0; i < metadata2.mipLevels; i++)
	//{
	//	//ミップマップレベルを指定してイメージを取得
	//	const Image* img = scratchImg2.GetImage(i, 0, 0);
	//	//テクスチャバッファにデータ転送
	//	result = texBuff2->WriteToSubresource(
	//		(UINT)i,
	//		nullptr,//全領域へコピー
	//		img->pixels,//元データアドレス
	//		(UINT)img->rowPitch,//1ラインアドレス
	//		(UINT)img->slicePitch//1枚サイズ
	//	);
	//	assert(SUCCEEDED(result));
	//}

#pragma endregion 
}

void Texture::SRVGeneraion(ID3D12Device* dev)
{
#pragma region シェーダーリソースビューの為のデスクリプタヒープ生成

	//SRVの最大個数
	const size_t kMaxSRVCount = 2056;

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//設定をもとにSRV用デスクリプタヒープを生成

	result = dev->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	//デスクリプタハンドル(ヒープ内の操作する場所指定に使う)
	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

#pragma endregion

#pragma region シェーダリソースビュー

	for (int i = 0; i < textureResourceDesc.size(); i++)
	{

		srvHandle.ptr += incremantSize*i;
		//シェーダリソースビュー
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
		srvDesc.Format = textureResourceDesc[i].Format;//RGBA float
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
		srvDesc.Texture2D.MipLevels = textureResourceDesc[i].MipLevels;

		//ハンドルの指す位置にシェーダリソースビュー作成
		dev->CreateShaderResourceView(texBuff[i].Get(), &srvDesc, srvHandle);

		
	}

	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};//設定構造体
	//srvDesc2.Format = textureResourceDesc2.Format;//RGBA float
	//srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	//srvDesc2.Texture2D.MipLevels = textureResourceDesc2.MipLevels;

	////ハンドルの指す位置にシェーダリソースビュー作成
	//dev->CreateShaderResourceView(texBuff2.Get(), &srvDesc2, srvHandle);

#pragma endregion
}