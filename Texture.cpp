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

	//�e�N�X�`���o�b�t�@
	textureBuffGeneraion(dev.Get());

	//�V�F�[�_���\�[�X�r���[
	SRVGeneraion(dev.Get());

	return metadata.size();
}

void Texture::Draw(ID3D12GraphicsCommandList* cmdList, int tex)
{
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;

	//SRV�q�[�v�̐ݒ�R�}���h
	cmdList->SetDescriptorHeaps(1, srvHeap.GetAddressOf());
	//SRV�q�[�v�̐擪�n���h�����擾(SRV���w���Ă���͂�)
	srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^�Q�Ԃɐݒ�
	{
		srvGpuHandle.ptr += incremantSize*(tex-1);
		cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
	}

}

void Texture::imageDataGeneration(const char filename[])
{
#pragma region �摜�C���[�W�f�[�^�̍쐬

	//�摜�ǂݍ��݂��ĉ摜�C���[�W�f�[�^�𐶐�

	TexMetadata metadataBuff{};
	ScratchImage scratchImgBuff{};

	std::string filepath = filename;

	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	//WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		wfilepath,
		WIC_FLAGS_NONE,
		&metadataBuff,
		scratchImgBuff
	);

	ScratchImage mipChain{};
	//�~�b�v�}�b�v�̐���
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

	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadataBuff.format = MakeSRGB(metadataBuff.format);

	metadata.push_back(metadataBuff);

	////WIC�e�N�X�`���̃��[�h
	//result = LoadFromWICFile(
	//	L"Resources/basketballman2.png",
	//	WIC_FLAGS_NONE,
	//	&metadata2,
	//	scratchImg2
	//);

	//ScratchImage mipChain2{};
	////�~�b�v�}�b�v�̐���
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

	////�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	//metadata2.format = MakeSRGB(metadata2.format);

#pragma endregion
}

void Texture::textureBuffGeneraion(ID3D12Device* dev)
{
#pragma region �e�N�X�`���o�b�t�@�ݒ�

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//���\�[�X�ݒ�
	for (int i = 0; i < metadata.size(); i++)
	{

		D3D12_RESOURCE_DESC textureResourceDescBuff{};
		textureResourceDescBuff.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureResourceDescBuff.Format = metadata[i].format;
		textureResourceDescBuff.Width = metadata[i].width;//��
		textureResourceDescBuff.Height = (UINT)metadata[i].height;//����
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
	//textureResourceDesc2.Width = metadata2.width;//��
	//textureResourceDesc2.Height = (UINT)metadata2.height;//����
	//textureResourceDesc2.DepthOrArraySize = (UINT16)metadata2.arraySize;
	//textureResourceDesc2.MipLevels = (UINT16)metadata2.mipLevels;
	//textureResourceDesc2.SampleDesc.Count = 1;

#pragma endregion

#pragma region �e�N�X�`���o�b�t�@�̐���

	//�e�N�X�`���o�b�t�@�̐���

	

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

		//�~�b�v�}�b�v�Œu��������
		//�S�~�b�v�}�b�v�ɂ���
		for (size_t j = 0; j < metadata[i].mipLevels; j++)
		{
			//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
			const Image* img = scratchImg[i].GetImage(j, 0, 0);
			//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
			result = newTexBuff->WriteToSubresource(
				(UINT)j,
				nullptr,//�S�̈�փR�s�[
				img->pixels,//���f�[�^�A�h���X
				(UINT)img->rowPitch,//1���C���A�h���X
				(UINT)img->slicePitch//1���T�C�Y
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

	////�~�b�v�}�b�v�Œu��������
	////�S�~�b�v�}�b�v�ɂ���
	//for (size_t i = 0; i < metadata2.mipLevels; i++)
	//{
	//	//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
	//	const Image* img = scratchImg2.GetImage(i, 0, 0);
	//	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	//	result = texBuff2->WriteToSubresource(
	//		(UINT)i,
	//		nullptr,//�S�̈�փR�s�[
	//		img->pixels,//���f�[�^�A�h���X
	//		(UINT)img->rowPitch,//1���C���A�h���X
	//		(UINT)img->slicePitch//1���T�C�Y
	//	);
	//	assert(SUCCEEDED(result));
	//}

#pragma endregion 
}

void Texture::SRVGeneraion(ID3D12Device* dev)
{
#pragma region �V�F�[�_�[���\�[�X�r���[�ׂ̈̃f�X�N���v�^�q�[�v����

	//SRV�̍ő��
	const size_t kMaxSRVCount = 2056;

	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//�ݒ�����Ƃ�SRV�p�f�X�N���v�^�q�[�v�𐶐�

	result = dev->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	//�f�X�N���v�^�n���h��(�q�[�v���̑��삷��ꏊ�w��Ɏg��)
	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

#pragma endregion

#pragma region �V�F�[�_���\�[�X�r���[

	for (int i = 0; i < textureResourceDesc.size(); i++)
	{

		srvHandle.ptr += incremantSize*i;
		//�V�F�[�_���\�[�X�r���[
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
		srvDesc.Format = textureResourceDesc[i].Format;//RGBA float
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
		srvDesc.Texture2D.MipLevels = textureResourceDesc[i].MipLevels;

		//�n���h���̎w���ʒu�ɃV�F�[�_���\�[�X�r���[�쐬
		dev->CreateShaderResourceView(texBuff[i].Get(), &srvDesc, srvHandle);

		
	}

	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};//�ݒ�\����
	//srvDesc2.Format = textureResourceDesc2.Format;//RGBA float
	//srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	//srvDesc2.Texture2D.MipLevels = textureResourceDesc2.MipLevels;

	////�n���h���̎w���ʒu�ɃV�F�[�_���\�[�X�r���[�쐬
	//dev->CreateShaderResourceView(texBuff2.Get(), &srvDesc2, srvHandle);

#pragma endregion
}