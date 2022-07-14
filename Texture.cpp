#include "Texture.h"

Texture::Texture()
{
	
}

Texture::~Texture()
{
}

void Texture::Init(ID3D12Device* dev)
{
	incremantSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	imageDataGeneration();

	//テクスチャバッファ
	textureBuffGeneraion(dev);

	//シェーダリソースビュー
	SRVGeneraion(dev);
}

void Texture::Draw(ID3D12GraphicsCommandList* cmdList)
{
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;

	//SRVヒープの設定コマンド
	cmdList->SetDescriptorHeaps(1, &srvHeap);
	//SRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
	//SRVヒープの先頭にあるSRVをルートパラメータ２番に設定
	cmdList->SetGraphicsRootDescriptorTable(2, srvGpuHandle);

	srvGpuHandle.ptr += incremantSize;
	cmdList->SetGraphicsRootDescriptorTable(2, srvGpuHandle);

}

void Texture::imageDataGeneration()
{
#pragma region 画像イメージデータの作成

	//画像読み込みして画像イメージデータを生成


	//WICテクスチャのロード
	result = LoadFromWICFile(
		L"Resources/hokehoke.png",
		WIC_FLAGS_NONE,
		&metadata,
		scratchImg
	);

	ScratchImage mipChain{};
	//ミップマップの生成
	result = GenerateMipMaps(
		scratchImg.GetImages(),
		scratchImg.GetImageCount(),
		scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain
	);

	if (SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	//WICテクスチャのロード
	result = LoadFromWICFile(
		L"Resources/basketballman2.png",
		WIC_FLAGS_NONE,
		&metadata2,
		scratchImg2
	);

	ScratchImage mipChain2{};
	//ミップマップの生成
	result = GenerateMipMaps(
		scratchImg2.GetImages(),
		scratchImg2.GetImageCount(),
		scratchImg2.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain2
	);

	if (SUCCEEDED(result))
	{
		scratchImg2 = std::move(mipChain2);
		metadata2 = scratchImg2.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata2.format = MakeSRGB(metadata2.format);

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
	
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;//幅
	textureResourceDesc.Height = (UINT)metadata.height;//高さ
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	
	textureResourceDesc2.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc2.Format = metadata2.format;
	textureResourceDesc2.Width = metadata2.width;//幅
	textureResourceDesc2.Height = (UINT)metadata2.height;//高さ
	textureResourceDesc2.DepthOrArraySize = (UINT16)metadata2.arraySize;
	textureResourceDesc2.MipLevels = (UINT16)metadata2.mipLevels;
	textureResourceDesc2.SampleDesc.Count = 1;

#pragma endregion

#pragma region テクスチャバッファの生成

	//テクスチャバッファの生成

	result = dev->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);

	//ミップマップで置き換える
	//全ミップマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		//ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,//全領域へコピー
			img->pixels,//元データアドレス
			(UINT)img->rowPitch,//1ラインアドレス
			(UINT)img->slicePitch//1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

	result = dev->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff2)
	);

	//ミップマップで置き換える
	//全ミップマップについて
	for (size_t i = 0; i < metadata2.mipLevels; i++)
	{
		//ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg2.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = texBuff2->WriteToSubresource(
			(UINT)i,
			nullptr,//全領域へコピー
			img->pixels,//元データアドレス
			(UINT)img->rowPitch,//1ラインアドレス
			(UINT)img->slicePitch//1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

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

	//シェーダリソースビュー
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	srvDesc.Format = textureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//ハンドルの指す位置にシェーダリソースビュー作成
	dev->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);

	srvHandle.ptr += incremantSize;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};//設定構造体
	srvDesc2.Format = textureResourceDesc2.Format;//RGBA float
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc2.Texture2D.MipLevels = textureResourceDesc2.MipLevels;

	//ハンドルの指す位置にシェーダリソースビュー作成
	dev->CreateShaderResourceView(texBuff2.Get(), &srvDesc2, srvHandle);

#pragma endregion
}