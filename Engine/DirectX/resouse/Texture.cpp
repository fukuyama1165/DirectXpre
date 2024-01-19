#include "Texture.h"
#include <dxgidebug.h>
#include "DirectXInit.h"



bool Texture::SInitFlag_ = false;

Texture::~Texture()
{
}

Texture* Texture::GetInstance()
{
	static Texture instance;
	if (SInitFlag_ == false)
	{
		instance.SRVHeapGeneraion();
		instance.Init();
		SInitFlag_ = true;
	}

	return& instance;
}

void Texture::SRVHeapGeneraion()
{

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	srvHeapDesc.NumDescriptors = (uint32_t)kMaxSRVCount;

	//設定をもとにSRV用デスクリプタヒープを生成

	result_ = DirectXInit::GetInstance()->Getdev()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result_));

	//SRVヒープの先頭ハンドルを取得
	 srvHandle_ = srvHeap_->GetCPUDescriptorHandleForHeapStart();

}

void Texture::Init()
{

	loadTexture("Resources/basketballman2.png","basketballMan");

}

std::string Texture::loadTexture(const std::string& filename, std::string handle)
{
	
	//一回読み込んだことがあるファイルはそのまま返す
	auto itr = std::find_if(textureDetas_.begin(), textureDetas_.end(), [&](const std::pair<std::string, std::shared_ptr<TextureDeta>>& p) {
		return p.second->fileName == filename;//条件
		});
	//見つかったらそれを返す
	if (itr != textureDetas_.end()) {
		return itr->first;
	}

	DirectX::TexMetadata metadataBuff{};
	DirectX::ScratchImage scratchImgBuff{};
	D3D12_RESOURCE_DESC textureResourceDescBuff{};

	imageDataGeneration(filename, metadataBuff, scratchImgBuff);

	//テクスチャバッファ
	textureBuffGeneraion(metadataBuff, scratchImgBuff, textureResourceDescBuff);

	SRVGeneraion(textureResourceDescBuff, texBuff2_,filename,handle);

	

	

	return handle;
}

std::string Texture::loadTexture(float new_xres, float new_yres, DXGI_FORMAT new_format, bool is_dynamic, const std::string& filename, std::string handle)
{
	//一回読み込んだことがあるファイルはそのまま返す
	auto itr = std::find_if(textureDetas_.begin(), textureDetas_.end(), [&](const std::pair<std::string, std::shared_ptr<TextureDeta>>& p) {
		return p.second->fileName == filename;//条件
		});
	//見つかったらそれを返す
	if (itr != textureDetas_.end()) {
		return itr->first;
	}

	is_dynamic;

	D3D12_RESOURCE_DESC textureResourceDescBuff{};

#pragma region テクスチャバッファ設定

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//リソース設定

	textureResourceDescBuff.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDescBuff.Format = new_format;
	textureResourceDescBuff.Width = (uint32_t)new_xres;//幅
	textureResourceDescBuff.Height = (uint32_t)new_yres;//高さ
	textureResourceDescBuff.DepthOrArraySize = (uint16_t)1;
	textureResourceDescBuff.MipLevels = (uint16_t)1;
	textureResourceDescBuff.SampleDesc.Count = 1;
	textureResourceDescBuff.SampleDesc.Quality = 0;



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


#pragma endregion 

#pragma region シェーダーリソースビューの為のデスクリプタヒープ生成

//デスクリプタハンドル(ヒープ内の操作する場所指定に使う)

#pragma endregion

#pragma region シェーダリソースビュー
	uint32_t indexNum = UINT32_MAX;

	//ヒープの位置決定
	//もうすでに使われていないか確認
	auto itr2 = textureDetas_.find(handle);
	//最後までいかなかった場合(見つかった場合)
	if (itr2 != textureDetas_.end())
	{
		//その値まんま使おうね
		indexNum = itr2->second->heapPositionIndex;
	}
	//見つからなかった場合
	else
	{
		//デスクリプタヒープのサイズ分回す
		for (uint32_t i = 0; i < kMaxSRVCount; i++)
		{
			//チェック変数(問題がなかったらそのまま)
			bool checkIndex = true;

			for (std::pair<const std::string, std::shared_ptr<TextureDeta>>& p : textureDetas_)
			{
				//もう使われているなら
				if (p.second->heapPositionIndex == i)
				{
					checkIndex = false;
					//続ける意味がないため
					break;
				}

			}

			if (checkIndex)
			{
				//問題ないならその値に設定
				indexNum = i;
				break;
			}

		}

	}

	//設定できなかった場合は止める
	if (indexNum == UINT32_MAX)
	{
		assert(0);
	}

	//設定を保存して登録
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = srvHeap_->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = srvHeap_->GetGPUDescriptorHandleForHeapStart();
	uint32_t incremantSize = DirectXInit::GetInstance()->Getdev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	cpuHandle.ptr += indexNum * incremantSize;
	gpuHandle.ptr += indexNum * incremantSize;
	std::shared_ptr<TextureDeta> newTexture = std::make_shared<TextureDeta>();
	newTexture->fileName = filename;
	newTexture->heapPositionIndex = indexNum;
	newTexture->srvCpuHandle = cpuHandle;
	newTexture->srvGpuHandle = gpuHandle;
	newTexture->texBuff = texBuff2_;


	//シェーダリソースビュー
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	srvDesc.Format = textureResourceDescBuff.Format;//RGBA float
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = textureResourceDescBuff.MipLevels;

	//ハンドルの指す位置にシェーダリソースビュー作成
	DirectXInit::GetInstance()->Getdev()->CreateShaderResourceView(texBuff2_.Get(), &srvDesc, cpuHandle);

	//ハンドルの設定がなかった場合
	if (handle.empty())
	{
		handle = "anSetingHandle_" + std::to_string(indexNum);
	}

	textureDetas_[handle] = newTexture;

#pragma endregion

	return handle;
}

void Texture::Draw(std::string handle)
{
	//見つからなかった場合終わり
	if (textureDetas_.find(handle) == textureDetas_.end()) {
		return;
	}
	DirectXInit::GetInstance()->GetcmdList()->SetDescriptorHeaps(1, srvHeap_.GetAddressOf());
	std::shared_ptr<TextureDeta> data = textureDetas_[handle];

	{
		
		DirectXInit::GetInstance()->GetcmdList()->SetGraphicsRootDescriptorTable(1, data->srvGpuHandle);
	}

}

void Texture::imageDataGeneration(const std::string& filename, DirectX::TexMetadata& metadataBuff, DirectX::ScratchImage& scratchImgBuff)
{
	{
#pragma region 画像イメージデータの作成

		//画像読み込みして画像イメージデータを生成

		wchar_t wfilepath[128];
		MultiByteToWideChar(CP_ACP, 0, filename.c_str(), -1, wfilepath, _countof(wfilepath));

		//拡張子を取得
		std::string ext = SeparateFilePath(filename);

		if (ext == "dds")
		{

			//DDSテクスチャのロード
			result_ = LoadFromDDSFile(
				wfilepath,
				DirectX::DDS_FLAGS_NONE,
				&metadataBuff,
				scratchImgBuff
			);

		}
		else
		{
			//WICテクスチャのロード
			result_ = LoadFromWICFile(
				wfilepath,
				DirectX::WIC_FLAGS_NONE,
				&metadataBuff,
				scratchImgBuff
			);

			DirectX::ScratchImage mipChain{};
			//ミップマップの生成
			result_ = GenerateMipMaps(
				scratchImgBuff.GetImages(),
				scratchImgBuff.GetImageCount(),
				scratchImgBuff.GetMetadata(),
				DirectX::TEX_FILTER_DEFAULT,
				0,
				mipChain
			);

			if (SUCCEEDED(result_))
			{
				scratchImgBuff = std::move(mipChain);
				//scratchImg_.push_back(scratchImgBuff);
				metadataBuff = scratchImgBuff.GetMetadata();
				//textureFileName_.push_back(filename);
			}
		}
		

		

		//読み込んだディフューズテクスチャをSRGBとして扱う
		metadataBuff.format = DirectX::MakeSRGB(metadataBuff.format);

#pragma endregion
	}
}

void Texture::textureBuffGeneraion(DirectX::TexMetadata& metadataBuff, DirectX::ScratchImage& scratchImgBuff, D3D12_RESOURCE_DESC& textureResourceDescBuff)
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
			const DirectX::Image* img = scratchImgBuff.GetImage(j, 0, 0);
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

		//texBuff_.push_back(texBuff2_);

		
		
#pragma endregion 
}


void Texture::SRVGeneraion(const D3D12_RESOURCE_DESC& TextureResourceDesc, Microsoft::WRL::ComPtr<ID3D12Resource> newTexBuff, const std::string& filename, std::string& handle)
{
#pragma region シェーダーリソースビューの為のデスクリプタヒープ生成

	//デスクリプタハンドル(ヒープ内の操作する場所指定に使う)
	
#pragma endregion

#pragma region シェーダリソースビュー
	uint32_t indexNum = UINT32_MAX;
	
	//ヒープの位置決定
	//もうすでに使われていないか確認
	auto itr = textureDetas_.find(handle);
	//最後までいかなかった場合(見つかった場合)
	if (itr != textureDetas_.end())
	{
		//その値まんま使おうね
		indexNum = itr->second->heapPositionIndex;
	}
	//見つからなかった場合
	else
	{
		//デスクリプタヒープのサイズ分回す
		for (uint32_t i = 0; i < kMaxSRVCount; i++)
		{
			//チェック変数(問題がなかったらそのまま)
			bool checkIndex = true;

			for (std::pair<const std::string, std::shared_ptr<TextureDeta>>& p : textureDetas_)
			{
				//もう使われているなら
				if (p.second->heapPositionIndex == i)
				{
					checkIndex = false;
					//続ける意味がないため
					break;
				}

			}

			if (checkIndex)
			{
				//問題ないならその値に設定
				indexNum = i;
				break;
			}

		}

	}

	//設定できなかった場合は止める
	if (indexNum == UINT32_MAX)
	{
		assert(0);
	}

	//設定を保存して登録
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = srvHeap_->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = srvHeap_->GetGPUDescriptorHandleForHeapStart();
	uint32_t incremantSize = DirectXInit::GetInstance()->Getdev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	cpuHandle.ptr += indexNum * incremantSize;
	gpuHandle.ptr += indexNum * incremantSize;
	std::shared_ptr<TextureDeta> newTexture = std::make_shared<TextureDeta>();
	newTexture->fileName = filename;
	newTexture->heapPositionIndex = indexNum;
	newTexture->srvCpuHandle = cpuHandle;
	newTexture->srvGpuHandle = gpuHandle;
	newTexture->texBuff = newTexBuff;


	//シェーダリソースビュー
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	srvDesc.Format = TextureResourceDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = TextureResourceDesc.MipLevels;

	//ハンドルの指す位置にシェーダリソースビュー作成
	DirectXInit::GetInstance()->Getdev()->CreateShaderResourceView(newTexBuff.Get(), &srvDesc, cpuHandle);

	//ハンドルの設定がなかった場合
	if (handle.empty())
	{
		handle = "anSetingHandle_" + std::to_string(indexNum);
	}

	textureDetas_[handle] = newTexture;

#pragma endregion
}

std::string Texture::SeparateFilePath(const std::string& filePath)
{

	size_t pos1;
	std::string ans;

	//区切り文字'.'が出てくる一番最後の部分を検索
	pos1 = filePath.rfind('.');

	//検索がヒットしたら
	if (pos1 != std::wstring::npos)
	{
		//区切り文字の後ろをファイル拡張子として保存
		ans = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);

	}
	else
	{

		//ないのはやばいので止める
		assert(0);

	}


	return ans;
}


