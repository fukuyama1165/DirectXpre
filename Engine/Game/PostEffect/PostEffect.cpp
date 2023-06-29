#include "PostEffect.h"
#include "Input.h"

//�w���p�[�\���̎g���̂ɒǉ�����
#include <d3dx12.h>

Texture* PostEffect::STexture_ = nullptr;

const float PostEffect::clearColor[4] = { 0.25f,0.5f,0.1f,0.0f };

PostEffect::PostEffect()
{
	

}

void PostEffect::Initialize()
{
	
	STexture_ = Texture::GetInstance();
	STexture_->Init();

	textureBuffGeneraion();

	//�e�N�X�`����ԃN���A
	{
		HRESULT result;
		//��f��(1280x720=921600�s�N�Z��)
		const uint32_t pixelCount = WinApp::SWindowWidth_ * WinApp::SWindowHeight_;

		//�摜1�s���̃f�[�^�T�C�Y
		const uint32_t rowPitch = sizeof(uint32_t) * WinApp::SWindowWidth_;

		//�摜�S�̂̃f�[�^�T�C�Y
		const uint32_t depthPitch = rowPitch * WinApp::SWindowHeight_;

		//�摜�C���[�W
		std::vector<uint32_t> img;
		img.resize(pixelCount);

		for (uint32_t i = 0; i < pixelCount; i++)
		{
			img[i] = 0xff0000ff;
		}

		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		for (uint16_t i = 0; i < _countof(texBuff_); i++)
		{
			result = texBuff_[i]->WriteToSubresource(
				0,
				nullptr,
				img.data(),
				rowPitch,
				depthPitch
			);

			assert(SUCCEEDED(result));
		}

	}

	SRVDescHeapGeneraion();
	RTVDescHeapGeneraion();
	DSVDescHeapGeneraion();

	vertices_[0] = { {-1.0f,-1.0f,0.0f},{0.0f,1.0f} };
	vertices_[1] = { {-1.0f,+1.0f,0.0f},{0.0f,0.0f} };
	vertices_[2] = { {+1.0f,-1.0f,0.0f},{1.0f,1.0f} };

	vertices_[3] = { {-1.0f,+1.0f,0.0f},{0.0f,0.0f} };
	vertices_[4] = { {+1.0f,-1.0f,0.0f},{1.0f,1.0f} };
	vertices_[5] = { {+1.0f,+1.0f,0.0f},{1.0f,0.0f} };

	sizeVB_ = static_cast<uint32_t>(sizeof(vertices_[0]) * _countof(vertices_));

	vertexBuffGeneration();

	vertexShaderGeneration();

	pixelShaderGeneration();

	vertexLayout();

	graphicPipelineGeneration();

	descriptorRangeGeneration();

	textureSamplerGeneration();

	rootParamGeneration();

	rootsignatureGeneration();

	constantBuffGeneration();

	
}

void PostEffect::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_0))
	{
		cheng_ = !cheng_;
	}
}

void PostEffect::Draw(uint16_t PipeLineRuleFlag)
{
	//�v���~�e�B�u�`��(�O�p�`���X�g)
	DirectXInit::GetInstance()->GetcmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�
	//������p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`�����Z�b�g����
	//���߂����[���ŕ`������肢����Ƃ���

	switch (PipeLineRuleFlag)
	{
	case 0:
		DirectXInit::GetInstance()->GetcmdList()->SetPipelineState(pipelinestate_.Get());
		break;

	case 1:
		DirectXInit::GetInstance()->GetcmdList()->SetPipelineState(pipelinestate2_.Get());
		break;

	case 2:
		DirectXInit::GetInstance()->GetcmdList()->SetPipelineState(pipelinestate3_.Get());
		break;

	case 3:
		DirectXInit::GetInstance()->GetcmdList()->SetPipelineState(pipelinestate4_.Get());
		break;

	case 4:
		DirectXInit::GetInstance()->GetcmdList()->SetPipelineState(pipelinestate5_.Get());
		break;

	default:
		DirectXInit::GetInstance()->GetcmdList()->SetPipelineState(pipelinestate_.Get());
		break;
	}


	DirectXInit::GetInstance()->GetcmdList()->SetGraphicsRootSignature(rootsignature_.Get());


	//���_�o�b�t�@�r���[�̐ݒ�
	DirectXInit::GetInstance()->GetcmdList()->IASetVertexBuffers(0, 1, &vbView_);


	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	DirectXInit::GetInstance()->GetcmdList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());
	DirectXInit::GetInstance()->GetcmdList()->SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());

	
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;
	//uint32_t incremantSize_ = DirectXInit::GetInstance()->Getdev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//SRV�q�[�v�̐ݒ�R�}���h
	DirectXInit::GetInstance()->GetcmdList()->SetDescriptorHeaps(1, descHeapSRV_.GetAddressOf());
	//SRV�q�[�v�̐擪�n���h�����擾(SRV���w���Ă���͂�)
	srvGpuHandle = descHeapSRV_->GetGPUDescriptorHandleForHeapStart();


	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	{
		
		DirectXInit::GetInstance()->GetcmdList()->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV_->GetGPUDescriptorHandleForHeapStart(),0,
			DirectXInit::GetInstance()->Getdev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
		if (cheng_)
		{
			DirectXInit::GetInstance()->GetcmdList()->SetGraphicsRootDescriptorTable(3, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV_->GetGPUDescriptorHandleForHeapStart(), 1,
				DirectXInit::GetInstance()->Getdev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
		}
		else
		{
			DirectXInit::GetInstance()->GetcmdList()->SetGraphicsRootDescriptorTable(3, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV_->GetGPUDescriptorHandleForHeapStart(), 0,
				DirectXInit::GetInstance()->Getdev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
		}
	}
	

	DirectXInit::GetInstance()->GetcmdList()->DrawInstanced(_countof(vertices_), 1, 0, 0);

}

void  PostEffect::PreDrawScene()
{
	DirectXInit* directXinit = DirectXInit::GetInstance();

	for (uint16_t i = 0; i < _countof(texBuff_); i++)
	{

		barrierDesc_.Transition.pResource = texBuff_[i].Get();
		barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;//�\������
		barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;//�`��
		directXinit->GetcmdList()->ResourceBarrier(1, &barrierDesc_);
	}

	//���\�[�X�o���A��ύX(�V�F�[�_�[���\�[�X�`��\)
	/*directXinit->GetcmdList()->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(texBuff_.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET)
	);*/

	//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[2];

	for (uint16_t i = 0; i < _countof(rtvHs); i++)
	{
		rtvHs[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapRTV_->GetCPUDescriptorHandleForHeapStart(), i,
			directXinit->Getdev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
	}

	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV_->GetCPUDescriptorHandleForHeapStart();

	//�����_�[�^�[�Q�b�g���Z�b�g
	directXinit->GetcmdList()->OMSetRenderTargets(2, rtvHs, false, &dsvH);

	//�r���[�|�[�g�ݒ�
	//�������̃r���[�|�[�g��p��(���ɂ����̂Ō��炵��)
	D3D12_VIEWPORT viewports[2]{};
	for (uint16_t i = 0; i < _countof(rtvHs); i++)
	{
		viewports[i].Width = (float)WinApp::GetInstance()->getWindowSizeWidth();//����
		viewports[i].Height = (float)WinApp::GetInstance()->getWindowSizeHeight();//�c��
		viewports[i].TopLeftX = 0;//����X
		viewports[i].TopLeftY = 0;//����Y
		viewports[i].MinDepth = 0.1f;//�ŏ��[�x(0�ł悢)
		viewports[i].MaxDepth = 1.0f;//�ő�[�x(�P�ł悢)
	}

	//�R�}���h���X�g�ɒǉ�
	directXinit->GetcmdList()->RSSetViewports(2, viewports);


	//�V�U�[��`�ݒ�
	D3D12_RECT scissorrects[2]{};

	for (uint16_t i = 0; i < _countof(rtvHs); i++)
	{
		scissorrects[i].left = 0;//�؂蔲�����W��
		scissorrects[i].right = scissorrects[i].left + WinApp::GetInstance()->getWindowSizeWidth();//�؂蔲�����W�E
		scissorrects[i].top = 0;//�؂蔲�����W��
		scissorrects[i].bottom = scissorrects[i].top + WinApp::GetInstance()->getWindowSizeHeight();//�؂蔲�����W��
	}

	directXinit->GetcmdList()->RSSetScissorRects(2, scissorrects);

	for (uint16_t i = 0; i < _countof(rtvHs); i++)
	{
		//�S��ʃN���A
		directXinit->GetcmdList()->ClearRenderTargetView(rtvHs[i], clearColor, 0, nullptr);
	}

	//�[�x�o�b�t�@�̃N���A
	directXinit->GetcmdList()->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);



}

void  PostEffect::PostDrawScene()
{

	DirectXInit* directXinit = DirectXInit::GetInstance();

	for (uint16_t i = 0; i < _countof(texBuff_); i++)
	{
		// 5.���\�[�X�o���A��߂�(�������߂��Ԃ��ƕ`��ł��Ȃ��̂ŕ`��ł���悤�ɂ���)
		barrierDesc_.Transition.pResource = texBuff_[i].Get();
		barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;//�`��
		barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;//�\����
		directXinit->GetcmdList()->ResourceBarrier(1, &barrierDesc_);
	}

	



}

void PostEffect::textureBuffGeneraion()
{
	HRESULT result;
	//�e�N�X�`�����\�[�X�ݒ�

	//�w���p�[�\����
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WinApp::SWindowWidth_,
		(uint32_t)WinApp::SWindowHeight_,
		1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET//�|�X�g�G�t�F�N�g�̂��߂�flag
	);

	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	D3D12_CLEAR_VALUE clearValue = {};

	clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	memcpy(clearValue.Color, clearColor, sizeof(D3D12_CLEAR_VALUE::Color));

	for (uint16_t i = 0; i < _countof(texBuff_); i++)
	{
		//�e�N�X�`���o�b�t�@�̐���
		result = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,//�|�X�g�G�t�F�N�g�p
			&clearValue,
			IID_PPV_ARGS(&texBuff_[i])
		);

		assert(SUCCEEDED(result));
	}
}

void PostEffect::SRVDescHeapGeneraion()
{
	HRESULT result;
	//SRV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};

	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 2;

	//SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = DirectXInit::GetInstance()->Getdev()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV_));
	assert(SUCCEEDED(result));

	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	for (uint16_t i = 0; i < _countof(texBuff_); i++)
	{
		DirectXInit::GetInstance()->Getdev()->CreateShaderResourceView(texBuff_[i].Get(),
			&srvDesc,			
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descHeapSRV_->GetCPUDescriptorHandleForHeapStart(), i,
				DirectXInit::GetInstance()->Getdev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			)
		);
	}


}

void PostEffect::RTVDescHeapGeneraion()
{
	HRESULT result;
	//RTV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 2;

	//RTV�p�f�X�N���v�^�q�[�v�𐶐�
	result = DirectXInit::GetInstance()->Getdev()->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV_));
	assert(SUCCEEDED(result));

	//�����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};

	//�V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	for (uint16_t i = 0; i < _countof(texBuff_); i++)
	{
		//�f�X�N���v�^�q�[�v��RTV�쐬
		DirectXInit::GetInstance()->Getdev()->CreateRenderTargetView(texBuff_[i].Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descHeapRTV_->GetCPUDescriptorHandleForHeapStart(),i,
				DirectXInit::GetInstance()->Getdev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
			)
		);
	}

}

void PostEffect::DSVDescHeapGeneraion()
{
	HRESULT result;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC depthResourceDesc{};

	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = WinApp::GetInstance()->getWindowSizeWidth();//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.Height = WinApp::GetInstance()->getWindowSizeHeight();//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;//�f�v�X�e���V��

	//�[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	//�[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;//�[�x�l1.0f(�ő�l)�ŃN���A
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g

	//���\�[�X����

	result = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//�[�x�l�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff_)
	);

	//�[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//�[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//�f�v�X�X�e���V���r���[
	result = DirectXInit::GetInstance()->Getdev()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&descHeapDSV_));
	assert(SUCCEEDED(result));

	//�[�x�r���[����
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DirectXInit::GetInstance()->Getdev()->CreateDepthStencilView(
		depthBuff_.Get(),
		&dsvDesc,
		descHeapDSV_->GetCPUDescriptorHandleForHeapStart()
	);

}

void PostEffect::vertexBuffGeneration()
{
#pragma region ���_�o�b�t�@�̊m��

	//���_�o�b�t�@�̊m�ە�

	heapprop_.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p


	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeVB_;//���_�f�[�^�S�̂̃T�C�Y
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

#pragma endregion

#pragma region ���_�o�b�t�@�̐���

	//���_�o�b�t�@�̐���

	result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
		&heapprop_,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc_,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_)
	);

	assert(SUCCEEDED(result_));

#pragma endregion
#pragma region ���_�o�b�t�@�ւ̃f�[�^�]��

	//���_�o�b�t�@�ւ̃f�[�^�]��
	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap_);

	assert(SUCCEEDED(result_));

	//�S���_�ɑ΂���
	for (uint16_t i = 0; i < _countof(vertices_); i++)
	{
		vertMap_[i] = vertices_[i];//���W���R�s�[
	}


	//�Ȃ��������
	vertBuff_->Unmap(0, nullptr);

#pragma endregion

#pragma region ���_�o�b�t�@�r���[�̐���

	//���_�o�b�t�@�r���[�̍쐬(GPU�ւ̓������)��

	//���_�o�b�t�@�r���[�̍쐬

	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeVB_;
	vbView_.StrideInBytes = sizeof(vertices_[0]);

#pragma endregion

}

void PostEffect::vertexShaderGeneration()
{
	//���_�V�F�[�_�t�@�C���̓ǂݍ��݂ƃR���p�C��
#pragma region 

	//���_�V�F�[�_�t�@�C���̓ǂݍ��ݕ�
	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result_ = D3DCompileFromFile(
		L"Resources/shaders/PostEffectVS.hlsl",//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"main",//�G���g���[�|�C���g��
		"vs_5_0",//�V�F�[�_���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�N�p�ݒ�
		0,
		&vsBlob_,
		&errorBlob_
	);

#pragma endregion 

	//���_�V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ
#pragma region 

	//���_�V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ
	if (FAILED(result_))
	{
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob_->GetBufferSize());

		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void PostEffect::pixelShaderGeneration()
{
	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
#pragma region 

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result_ = D3DCompileFromFile(
		L"Resources/shaders/PostEffectPS.hlsl",//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"main",//�G���g���[�|�C���g��
		"ps_5_0",//�V�F�[�_���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�N�p�ݒ�
		0,
		&psBlob_,
		&errorBlob_
	);

#pragma endregion


	//�s�N�Z���V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ
#pragma region 

	//�s�N�Z���V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ
	if (FAILED(result_))
	{
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob_->GetBufferSize());

		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void PostEffect::vertexLayout()
{
#pragma region ���_���C�A�E�g�̐ݒ�

	//���_���C�A�E�g
	//�O���t�B�b�N�p�C�v���C���Œ��_����̃f�[�^�ɉ����������邩���߂�
	//����͍Œ����3D���W�����������Ă���炵��
	inputLayout_[0] =
	{
		"POSITION",//�Z�}���e�B�b�N��
		0,//�����Z�}���e�B�b�N������������Ƃ��̃C���f�b�N�X
		DXGI_FORMAT_R32G32B32_FLOAT,//�v�f���ƃr�b�g����Ԃ�(XYZ�̂R��float�^�Ȃ̂�R32G32B32_FLOAT)
		0,//���̓X���b�g�C���f�b�N�X
		D3D12_APPEND_ALIGNED_ELEMENT,//�f�[�^�̃I�t�Z�b�g�l(D3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�)
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,//���̓f�[�^����(�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
		0,//��x�ɕ`�悷��C���X�^���X��
	};
	//���W�ȊO�Ɂ@�F�A�e�N�X�`��UV�Ȃǂ�n���ꍇ�͂���ɑ�����
	inputLayout_[1] =
	{
		"TEXCOORD",//�Z�}���e�B�b�N��
		0,//�����Z�}���e�B�b�N������������Ƃ��̃C���f�b�N�X
		DXGI_FORMAT_R32G32_FLOAT,//�v�f���ƃr�b�g����Ԃ�(XY��2��float�^�Ȃ̂�R32G32_FLOAT)
		0,//���̓X���b�g�C���f�b�N�X
		D3D12_APPEND_ALIGNED_ELEMENT,//�f�[�^�̃I�t�Z�b�g�l(D3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�)
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,//���̓f�[�^����(�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
		0,//��x�ɕ`�悷��C���X�^���X��
	};


#pragma endregion
}

void PostEffect::graphicPipelineGeneration()
{
#pragma region �O���t�B�b�N�X�p�C�v���C���ݒ�

	//�O���t�B�b�N�X�p�C�v���C����

	//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
	gpipeline_.VS.BytecodeLength = vsBlob_->GetBufferSize();

	/*gpipeline.GS.pShaderBytecode = common->getGsBlob()->GetBufferPointer();
	gpipeline.GS.BytecodeLength = common->getGsBlob()->GetBufferSize();*/

	gpipeline_.PS.pShaderBytecode =psBlob_->GetBufferPointer();
	gpipeline_.PS.BytecodeLength = psBlob_->GetBufferSize();

	//�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
	gpipeline_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�
	gpipeline_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O���Ȃ�
	gpipeline_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//�|���S�����h��Ԃ�
	//gpipeline.RasterizerState.DepthClipEnable = true;//�[�x�N���b�s���O��L����


	//�u�����h�X�e�[�g�̐ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline_.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region �u�����h�X�e�[�g�̋��ʐݒ�(�A���t�@�l)

	blenddesc.BlendEnable = true;//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g��

#pragma endregion

	//���Z����
#pragma region 

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//�f�X�g�̒l��100%�g��

#pragma endregion �R�����g�A�E�g��

#pragma region ���Z����

	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;//�f�X�g����\�[�X�����Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//�f�X�g�̒l��100%�g��

#pragma endregion �R�����g�A�E�g��

#pragma region �F���]

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-�f�X�g�J���[�̒l
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;//�g��Ȃ�

#pragma endregion �R�����g�A�E�g��

	//����������
#pragma region 

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l

#pragma endregion



	//���_���C�A�E�g�̐ݒ�
	gpipeline_.InputLayout.pInputElementDescs = inputLayout_;
	gpipeline_.InputLayout.NumElements = _countof(inputLayout_);

	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	gpipeline_.NumRenderTargets = 1;//�`��Ώۂ͂P��
	gpipeline_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0�`255�w���RGBA
	gpipeline_.SampleDesc.Count = 1;//�P�s�N�Z���ɂ��P��T���v�����O

#pragma endregion �ʏ�`��|���S������h��Ԃ�(�O�p�`)

	//�ʏ�`�惏�C���[�t���[���`��(�O�p�`)
#pragma region �O���t�B�b�N�X�p�C�v���C���Q�̐ݒ�

//�O���t�B�b�N�X�p�C�v���C����


		//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline2_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
	gpipeline2_.VS.BytecodeLength = vsBlob_->GetBufferSize();

	/*gpipeline2.GS.pShaderBytecode = common->getGsBlob()->GetBufferPointer();
	gpipeline2.GS.BytecodeLength = common->getGsBlob()->GetBufferSize();*/

	gpipeline2_.PS.pShaderBytecode = psBlob_->GetBufferPointer();
	gpipeline2_.PS.BytecodeLength = psBlob_->GetBufferSize();

	//�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
	gpipeline2_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�
	gpipeline2_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O���Ȃ�
	gpipeline2_.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;//���C���[�t���[���`��
	//gpipeline2.RasterizerState.DepthClipEnable = true;//�[�x�N���b�s���O��L����

	//�u�����h�X�e�[�g�̐ݒ�
	//gpipeline2.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA�S�Ẵ`�����l����`��
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc2 = gpipeline2_.BlendState.RenderTarget[0];
	blenddesc2.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region ���ʐݒ�(�A���t�@�l)

	blenddesc2.BlendEnable = true;//�u�����h��L���ɂ���
	blenddesc2.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blenddesc2.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blenddesc2.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g��

#pragma endregion

	//����������
#pragma region 

	blenddesc2.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	blenddesc2.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̃A���t�@�l
	blenddesc2.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l

#pragma endregion

	//���_���C�A�E�g�̐ݒ�
	gpipeline2_.InputLayout.pInputElementDescs = inputLayout_;
	gpipeline2_.InputLayout.NumElements = _countof(inputLayout_);

	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline2_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	gpipeline2_.NumRenderTargets = 1;//�`��Ώۂ͂P��
	gpipeline2_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0�`255�w���RGBA
	gpipeline2_.SampleDesc.Count = 1;//�P�s�N�Z���ɂ��P��T���v�����O

#pragma endregion 

	//�ʏ�`��|���S������h��Ԃ�(�O�p�`)���Z����
#pragma region �O���t�B�b�N�X�p�C�v���C��3�ݒ�

//�O���t�B�b�N�X�p�C�v���C����

	//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline3_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
	gpipeline3_.VS.BytecodeLength = vsBlob_->GetBufferSize();

	/*gpipeline3.GS.pShaderBytecode = common->getGsBlob()->GetBufferPointer();
	gpipeline3.GS.BytecodeLength = common->getGsBlob()->GetBufferSize();*/

	gpipeline3_.PS.pShaderBytecode = psBlob_->GetBufferPointer();
	gpipeline3_.PS.BytecodeLength = psBlob_->GetBufferSize();

	//�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
	gpipeline3_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�
	gpipeline3_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O���Ȃ�
	gpipeline3_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//�|���S�����h��Ԃ�
	//gpipeline3.RasterizerState.DepthClipEnable = true;//�[�x�N���b�s���O��L����


	//�u�����h�X�e�[�g�̐ݒ�
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA�S�Ẵ`�����l����`��
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc3 = gpipeline3_.BlendState.RenderTarget[0];
	blenddesc3.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region �u�����h�X�e�[�g�̋��ʐݒ�(�A���t�@�l)

	blenddesc3.BlendEnable = true;//�u�����h��L���ɂ���
	blenddesc3.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blenddesc3.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blenddesc3.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g��

#pragma endregion

	//���Z����
#pragma region 

	blenddesc3.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	blenddesc3.SrcBlend = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blenddesc3.DestBlend = D3D12_BLEND_ONE;//�f�X�g�̒l��100%�g��

#pragma endregion 

#pragma region ���Z����

	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;//�f�X�g����\�[�X�����Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//�f�X�g�̒l��100%�g��

#pragma endregion �R�����g�A�E�g��

#pragma region �F���]

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-�f�X�g�J���[�̒l
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;//�g��Ȃ�

#pragma endregion �R�����g�A�E�g��

	//����������
#pragma region 

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̃A���t�@�l
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l

#pragma endregion �R�����g�A�E�g��



	//���_���C�A�E�g�̐ݒ�
	gpipeline3_.InputLayout.pInputElementDescs = inputLayout_;
	gpipeline3_.InputLayout.NumElements = _countof(inputLayout_);

	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline3_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	gpipeline3_.NumRenderTargets = 1;//�`��Ώۂ͂P��
	gpipeline3_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0�`255�w���RGBA
	gpipeline3_.SampleDesc.Count = 1;//�P�s�N�Z���ɂ��P��T���v�����O

#pragma endregion 

#pragma region �O���t�B�b�N�X�p�C�v���C��4�ݒ�

	//�O���t�B�b�N�X�p�C�v���C����

		//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline4_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
	gpipeline4_.VS.BytecodeLength = vsBlob_->GetBufferSize();

	/*gpipeline4.GS.pShaderBytecode = common->getGsBlob()->GetBufferPointer();
	gpipeline4.GS.BytecodeLength = common->getGsBlob()->GetBufferSize();*/

	gpipeline4_.PS.pShaderBytecode = psBlob_->GetBufferPointer();
	gpipeline4_.PS.BytecodeLength = psBlob_->GetBufferSize();

	//�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
	gpipeline4_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�
	gpipeline4_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O���Ȃ�
	gpipeline4_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//�|���S�����h��Ԃ�
	//gpipeline4.RasterizerState.DepthClipEnable = true;//�[�x�N���b�s���O��L����


	//�u�����h�X�e�[�g�̐ݒ�
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA�S�Ẵ`�����l����`��
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc4 = gpipeline4_.BlendState.RenderTarget[0];
	blenddesc4.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region �u�����h�X�e�[�g�̋��ʐݒ�(�A���t�@�l)

	blenddesc4.BlendEnable = true;//�u�����h��L���ɂ���
	blenddesc4.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blenddesc4.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blenddesc4.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g��

#pragma endregion

	//���Z����
#pragma region 

	//blenddesc3.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	//blenddesc3.SrcBlend = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	//blenddesc3.DestBlend = D3D12_BLEND_ONE;//�f�X�g�̒l��100%�g��

#pragma endregion �R�����g�A�E�g��

#pragma region ���Z����

	blenddesc4.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;//�f�X�g����\�[�X�����Z
	blenddesc4.SrcBlend = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blenddesc4.DestBlend = D3D12_BLEND_ONE;//�f�X�g�̒l��100%�g��

#pragma endregion

#pragma region �F���]

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-�f�X�g�J���[�̒l
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;//�g��Ȃ�

#pragma endregion �R�����g�A�E�g��

	//����������
#pragma region 

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̃A���t�@�l
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l

#pragma endregion �R�����g�A�E�g��



	//���_���C�A�E�g�̐ݒ�
	gpipeline4_.InputLayout.pInputElementDescs = inputLayout_;
	gpipeline4_.InputLayout.NumElements = _countof(inputLayout_);

	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline4_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	gpipeline4_.NumRenderTargets = 1;//�`��Ώۂ͂P��
	gpipeline4_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0�`255�w���RGBA
	gpipeline4_.SampleDesc.Count = 1;//�P�s�N�Z���ɂ��P��T���v�����O

#pragma endregion �ʏ�`��|���S������h��Ԃ�(�O�p�`)���Z����

#pragma region �O���t�B�b�N�X�p�C�v���C��5�ݒ�

	//�O���t�B�b�N�X�p�C�v���C����

		//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline5_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
	gpipeline5_.VS.BytecodeLength = vsBlob_->GetBufferSize();

	/*gpipeline5.GS.pShaderBytecode = common->getGsBlob()->GetBufferPointer();
	gpipeline5.GS.BytecodeLength = common->getGsBlob()->GetBufferSize();*/

	gpipeline5_.PS.pShaderBytecode = psBlob_->GetBufferPointer();
	gpipeline5_.PS.BytecodeLength = psBlob_->GetBufferSize();

	//�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
	gpipeline5_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�
	gpipeline5_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O���Ȃ�
	gpipeline5_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//�|���S�����h��Ԃ�
	//gpipeline5.RasterizerState.DepthClipEnable = true;//�[�x�N���b�s���O��L����


	//�u�����h�X�e�[�g�̐ݒ�
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA�S�Ẵ`�����l����`��
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc5 = gpipeline5_.BlendState.RenderTarget[0];
	blenddesc5.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region �u�����h�X�e�[�g�̋��ʐݒ�(�A���t�@�l)

	blenddesc5.BlendEnable = true;//�u�����h��L���ɂ���
	blenddesc5.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blenddesc5.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blenddesc5.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g��

#pragma endregion

	//���Z����
#pragma region 

	//blenddesc3.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	//blenddesc3.SrcBlend = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	//blenddesc3.DestBlend = D3D12_BLEND_ONE;//�f�X�g�̒l��100%�g��

#pragma endregion �R�����g�A�E�g��

#pragma region ���Z����

	//blenddesc4.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;//�f�X�g����\�[�X�����Z
	//blenddesc4.SrcBlend = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	//blenddesc4.DestBlend = D3D12_BLEND_ONE;//�f�X�g�̒l��100%�g��

#pragma endregion �R�����g�A�E�g��

#pragma region �F���]

	blenddesc5.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	blenddesc5.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-�f�X�g�J���[�̒l
	blenddesc5.DestBlend = D3D12_BLEND_ZERO;//�g��Ȃ�

#pragma endregion 

	// ����������
#pragma region

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̃A���t�@�l
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l

#pragma endregion �R�����g�A�E�g��



	//���_���C�A�E�g�̐ݒ�
	gpipeline5_.InputLayout.pInputElementDescs = inputLayout_;
	gpipeline5_.InputLayout.NumElements = _countof(inputLayout_);

	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline5_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	gpipeline5_.NumRenderTargets = 1;//�`��Ώۂ͂P��
	gpipeline5_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0�`255�w���RGBA
	gpipeline5_.SampleDesc.Count = 1;//�P�s�N�Z���ɂ��P��T���v�����O

#pragma endregion �ʏ�`��|���S������h��Ԃ�(�O�p�`)�F���]

}

void PostEffect::rootParamGeneration()
{
	//�萔�o�b�t�@�𑝂₵���瑝�₷�Ƃ��낪����
#pragma region ���[�g�p�����[�^

	//���[�g�p�����[�^�̐ݒ�

	//�F
	rootParam_[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@�r���[
	rootParam_[0].Descriptor.ShaderRegister = 0;//�萔�o�b�t�@�ԍ�
	rootParam_[0].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParam_[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_�[���猩����

	//�摜�f�[�^�p
	rootParam_[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//���
	rootParam_[1].DescriptorTable.pDescriptorRanges = &descriptorRange_;//�f�X�N���v�^�����W
	rootParam_[1].DescriptorTable.NumDescriptorRanges = 1;//�f�X�N���v�^�����W��
	rootParam_[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_���猩����

	//�F
	rootParam_[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@�r���[
	rootParam_[2].Descriptor.ShaderRegister = 1;//�萔�o�b�t�@�ԍ�
	rootParam_[2].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParam_[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_�[���猩����

	//�摜�f�[�^�p
	rootParam_[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//���
	rootParam_[3].DescriptorTable.pDescriptorRanges = &descriptorRange2_;//�f�X�N���v�^�����W
	rootParam_[3].DescriptorTable.NumDescriptorRanges = 1;//�f�X�N���v�^�����W��
	rootParam_[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_���猩����

#pragma endregion 
}

void PostEffect::rootsignatureGeneration()
{
	//�萔�o�b�t�@�𑝂₵���烋�[�g�p�����[�^�����������p�����[�^��������������
#pragma region ���[�g�V�O�l�`���ݒ�

	//���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParam_;
	rootSignatureDesc.NumParameters = _countof(rootParam_);//���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &sampleDesc_;
	rootSignatureDesc.NumStaticSamplers = 1;


	//���[�g�V�O�l�`���̐���
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob_);
	result_ = DirectXInit::GetInstance()->Getdev()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature_));


	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline_.pRootSignature = rootsignature_.Get();

	//�p�C�v���C��2�ɂ����[�g�V�O�l�`�����Z�b�g
	gpipeline2_.pRootSignature = rootsignature_.Get();

	gpipeline3_.pRootSignature = rootsignature_.Get();

	gpipeline4_.pRootSignature = rootsignature_.Get();

	gpipeline5_.pRootSignature = rootsignature_.Get();

#pragma endregion 

#pragma region �f�v�X�X�e���V���X�e�[�g

	////�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	//gpipeline.DepthStencilState.DepthEnable = true;//�[�x�e�X�g���s��
	//gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//�������݋���
	//gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//��������΍��i
	//gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g

	//gpipeline2.DepthStencilState.DepthEnable = true;//�[�x�e�X�g���s��
	//gpipeline2.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//�������݋���
	//gpipeline2.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//��������΍��i
	//gpipeline2.DSVFormat = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g



#pragma endregion


#pragma region �p�C�v���C���X�e�[�g�̐���

	//�p�C�v���C���X�e�[�g�̐���

	result_ = DirectXInit::GetInstance()->Getdev()->CreateGraphicsPipelineState(&gpipeline_, IID_PPV_ARGS(&pipelinestate_));

	//�p�C�v���C���X�e�[�g�Q�̐���

	result_ = DirectXInit::GetInstance()->Getdev()->CreateGraphicsPipelineState(&gpipeline2_, IID_PPV_ARGS(&pipelinestate2_));

	result_ = DirectXInit::GetInstance()->Getdev()->CreateGraphicsPipelineState(&gpipeline3_, IID_PPV_ARGS(&pipelinestate3_));

	result_ = DirectXInit::GetInstance()->Getdev()->CreateGraphicsPipelineState(&gpipeline4_, IID_PPV_ARGS(&pipelinestate4_));

	result_ = DirectXInit::GetInstance()->Getdev()->CreateGraphicsPipelineState(&gpipeline5_, IID_PPV_ARGS(&pipelinestate5_));

#pragma endregion

}

void PostEffect::constantBuffGeneration()
{
#pragma region �萔�o�b�t�@

	//�萔�o�b�t�@�̐����p�̐ݒ�

	cbHeapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p


	cbResourceDesc_ = constBuffResourceGeneration(sizeof(ConstBufferMaterial));

	result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
		&cbHeapProp_,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial_));
	assert(SUCCEEDED(result_));


	result_ = constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial_);//�}�b�s���O
	assert(SUCCEEDED(result_));

	cbHeapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p


	cbResourceDesc_ = constBuffResourceGeneration(sizeof(ConstBuffDataTransform));

	result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
		&cbHeapProp_,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform_));
	assert(SUCCEEDED(result_));


	result_ = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform_);//�}�b�s���O
	assert(SUCCEEDED(result_));



	constMapMaterial_->color = { 1.0f,1.0f,1.0f,1.0f };

	matProjection_.IdentityMatrix();

	matProjection_.m[0][0] = 2.0f / (float)WinApp::SWindowWidth_;
	matProjection_.m[1][1] = -2.0f / (float)WinApp::SWindowHeight_;
	matProjection_.m[3][0] = -1.0f;
	matProjection_.m[3][1] = 1.0f;

	constMapTransform_->mat.IdentityMatrix();
	//constMapTransform_->mat *= matProjection_;

#pragma endregion
}

D3D12_RESOURCE_DESC PostEffect::constBuffResourceGeneration(uint32_t size)
{
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (size + 0xff) & ~0xff;//256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	return cbResourceDesc;
}

void PostEffect::descriptorRangeGeneration()
{
#pragma region �f�X�N���v�^�����W�̐ݒ�
	descriptorRange_.NumDescriptors = 1;//��x�̕`��Ɏg���e�N�X�`�����P���Ȃ̂�1
	descriptorRange_.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange_.BaseShaderRegister = 0;//�e�N�X�`�����W�X�^0��
	descriptorRange_.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	descriptorRange2_.NumDescriptors = 1;//��x�̕`��Ɏg���e�N�X�`�����P���Ȃ̂�1
	descriptorRange2_.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange2_.BaseShaderRegister = 1;//�e�N�X�`�����W�X�^1��
	descriptorRange2_.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

#pragma endregion
}

void PostEffect::textureSamplerGeneration()
{
	//�e�N�X�`���T���v���[�̐ݒ�
#pragma region 

	//�e�N�X�`���T���v���[�̐ݒ�
	sampleDesc_.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;//���J��Ԃ�(�^�C�����O)
	sampleDesc_.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;//�c�J��Ԃ�(�^�C�����O)
	sampleDesc_.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;//���s�J��Ԃ�(�^�C�����O)
	sampleDesc_.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//�{�[�_�[�̎��͍�
	sampleDesc_.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//�S�ă��j�A���
	sampleDesc_.MaxLOD = D3D12_FLOAT32_MAX;//�~�b�v�}�b�v�ő�l
	sampleDesc_.MinLOD = 0.0f;//�~�b�v�}�b�v�ŏ��l
	sampleDesc_.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampleDesc_.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//�s�N�Z���V�F�[�_����̂ݎg�p�\

#pragma endregion
}