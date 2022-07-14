#include "DrawingObj.h"



using namespace Microsoft::WRL;

const float PI = 3.141592653589;

DrawingObj::DrawingObj(const int windowWidth,const int windowHeight)
{
	Win_width = windowWidth;
	Win_height = windowHeight;

	//���_�f�[�^(�l�_���̍��W)
					//  x     y    z   �@��  u    v
	//�O
	vertices[0] = { { -50.0f,-50.0f,50.0f },{},{0.0f,1.0f}};//����
	vertices[1] = { {  50.0f,-50.0f,50.0f },{},{0.0f,0.0f} };//����
	vertices[2] = { { -50.0f, 50.0f,50.0f },{},{1.0f,1.0f} };//�E��
	vertices[3] = { {  50.0f, 50.0f,50.0f },{},{1.0f,0.0f} };//�E��

	//��
	vertices[4] = { { -50.0f,-50.0f,-50.0f },{},{0.0f,1.0f} };//����
	vertices[5] = { { -50.0f, 50.0f,-50.0f },{},{0.0f,0.0f} };//����
	vertices[6] = { {  50.0f,-50.0f,-50.0f },{},{1.0f,1.0f} };//�E��
	vertices[7] = { {  50.0f, 50.0f,-50.0f },{},{1.0f,0.0f} };//�E��

	//��
	vertices[8]  = { { -50.0f,-50.0f,-50.0f },{},{0.0f,1.0f} };//����
	vertices[9]  = { { -50.0f,-50.0f, 50.0f },{},{0.0f,0.0f} };//����
	vertices[10] = { { -50.0f, 50.0f,-50.0f },{},{1.0f,1.0f} };//�E��
	vertices[11] = { { -50.0f, 50.0f, 50.0f },{},{1.0f,0.0f} };//�E��

	//�E
	vertices[12] = { { 50.0f,-50.0f,-50.0f },{},{0.0f,1.0f} };//����
	vertices[13] = { { 50.0f, 50.0f,-50.0f },{},{0.0f,0.0f} };//����
	vertices[14] = { { 50.0f,-50.0f, 50.0f },{},{1.0f,1.0f} };//�E��
	vertices[15] = { { 50.0f, 50.0f, 50.0f },{},{1.0f,0.0f} };//�E��

	//��
	vertices[16] = { { -50.0f,50.0f,-50.0f },{},{0.0f,1.0f} };//����
	vertices[17] = { { -50.0f,50.0f, 50.0f },{},{0.0f,0.0f} };//����
	vertices[18] = { {  50.0f,50.0f,-50.0f },{},{1.0f,1.0f} };//�E��
	vertices[19] = { {  50.0f,50.0f, 50.0f },{},{1.0f,0.0f} };//�E��

	//��
	vertices[20] = { { -50.0f,-50.0f,-50.0f },{},{0.0f,1.0f} };//����
	vertices[21] = { {  50.0f,-50.0f,-50.0f },{},{0.0f,0.0f} };//����
	vertices[22] = { { -50.0f,-50.0f, 50.0f },{},{1.0f,1.0f} };//�E��
	vertices[23] = { {  50.0f,-50.0f, 50.0f },{},{1.0f,0.0f} };//�E��

	//�C���f�b�N�X�f�[�^
	//�O
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	//��
	indices[6] = 4;
	indices[7] = 5;
	indices[8] = 6;
	indices[9] = 6;
	indices[10] = 5;
	indices[11] = 7;

	//��
	indices[12] = 8;
	indices[13] = 9;
	indices[14] = 10;
	indices[15] = 10;
	indices[16] = 9;
	indices[17] = 11;

	//�E
	indices[18] = 12;
	indices[19] = 13;
	indices[20] = 14;
	indices[21] = 14;
	indices[22] = 13;
	indices[23] = 15;

	//��
	indices[24] = 16;
	indices[25] = 17;
	indices[26] = 18;
	indices[27] = 18;
	indices[28] = 17;
	indices[29] = 19;

	//��
	indices[30] = 20;
	indices[31] = 21;
	indices[32] = 22;
	indices[33] = 22;
	indices[34] = 21;
	indices[35] = 23;
	
	
	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	//�r���[�ϊ��s��
	eye_ = { 0, 0, -400 };//���_���W
	target_ = { 0, 0, 0 };//�����_���W
	up_ = { 0, 1, 0 };//������x�N�g��
}

DrawingObj::DrawingObj(const int windowWidth, const int windowHeight,XMFLOAT3 vertexPos1, XMFLOAT3 vertexPos2, XMFLOAT3 vertexPos3, XMFLOAT3 vertexPos4, XMFLOAT2 vertexUv1, XMFLOAT2 vertexUv2, XMFLOAT2 vertexUv3, XMFLOAT2 vertexUv4)
{
	Win_width = windowWidth;
	Win_height = windowHeight;

	//���_�f�[�^(�l�_���̍��W)
					//  x     y    z      u    v
	vertices[0] = { vertexPos1,{},vertexUv1 };//����
	vertices[1] = { vertexPos2,{},vertexUv2 };//����
	vertices[2] = { vertexPos3,{},vertexUv3 };//�E��
	vertices[3] = { vertexPos4,{},vertexUv4 };//�E��

	//�C���f�b�N�X�f�[�^
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;



	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	//�r���[�ϊ��s��
	eye_ = { 0, 0, -100 };//���_���W
	target_ = { 0, 0, 0 };//�����_���W
	up_ = { 0, 1, 0 };//������x�N�g��
}


DrawingObj::~DrawingObj()
{
}

void DrawingObj::basicInit(ID3D12Device* dev)
{
	vertexBuffGeneration(dev);

	vertexShaderGeneration();

	pixelShaderGeneration();

	vertexLayout();

	graphicPipelineGeneration();

	descriptorRangeGeneration();

	rootParamGeneration();

	textureSamplerGeneration();

	rootsignatureGeneration(dev);

	constantBuffGeneration(dev);
	

	indicesBuffGeneration(dev);

	/*imageDataGeneration();

	textureBuffGeneraion(dev);*/

	texture.Init(dev);

	//SRVGeneraion(dev);

}

void DrawingObj::colorChangeInit(ID3D12Device* dev)
{
	vertexBuffGeneration(dev);

	vertexShaderGeneration2();

	pixelShaderGeneration2();

	vertexLayout();

	graphicPipelineGeneration();

	descriptorRangeGeneration();

	rootParamGeneration();

	textureSamplerGeneration();

	rootsignatureGeneration(dev);

	constantBuffGeneration(dev);

	indicesBuffGeneration(dev);

	imageDataGeneration();

	textureBuffGeneraion(dev);

	SRVGeneraion(dev);

}

void DrawingObj::vertexBuffGeneration(ID3D12Device* dev)
{
#pragma region ���_�o�b�t�@�̊m��

	//���_�o�b�t�@�̊m�ە�
	
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p

	
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;//���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

#pragma endregion

#pragma region ���_�o�b�t�@�̐���

	//���_�o�b�t�@�̐���
	
	result = dev->CreateCommittedResource(
		&heapprop,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);

#pragma endregion

#pragma region �@���x�N�g���v�Z

	for (int i = 0; i < _countof(indices) / 3; i++)
	{//�O�p�`1���ƂɌv�Z���Ă���

		//�O�p�`�̃C���f�b�N�X�����o����,�ꎞ�I�ȕϐ��ɓ����
		unsigned short indices0 = indices[i * 3 + 0];
		unsigned short indices1 = indices[i * 3 + 1];
		unsigned short indices2 = indices[i * 3 + 2];

		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[indices0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[indices1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[indices2].pos);

		//p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z(�x�N�g���̌��Z)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);

		//���K��(������1�ɂ���)
		normal = XMVector3Normalize(normal);

		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[indices0].normal, normal);
		XMStoreFloat3(&vertices[indices1].normal, normal);
		XMStoreFloat3(&vertices[indices2].normal, normal);

	}

#pragma endregion


#pragma region ���_�o�b�t�@�ւ̃f�[�^�]��

	//���_�o�b�t�@�ւ̃f�[�^�]��
	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);

	//�S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];//���W���R�s�[
	}


	//�Ȃ��������
	vertBuff->Unmap(0, nullptr);

#pragma endregion

#pragma region ���_�o�b�t�@�r���[�̐���

	//���_�o�b�t�@�r���[�̍쐬(GPU�ւ̓������)��

	//���_�o�b�t�@�r���[�̍쐬
	
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

#pragma endregion

}

void DrawingObj::vertexShaderGeneration()
{
#pragma region ���_�V�F�[�_�t�@�C���̓ǂݍ��݂ƃR���p�C��

	//���_�V�F�[�_�t�@�C���̓ǂݍ��ݕ�
	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicVS.hlsl",//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"main",//�G���g���[�|�C���g��
		"vs_5_0",//�V�F�[�_���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�N�p�ݒ�
		0,
		&vsBlob,
		&errorBlob
	);

#pragma endregion 

#pragma region ���_�V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ

	//���_�V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ
	if (FAILED(result))
	{
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void DrawingObj::vertexShaderGeneration2()
{
#pragma region ���_�V�F�[�_�t�@�C���̓ǂݍ��݂ƃR���p�C��

	//���_�V�F�[�_�t�@�C���̓ǂݍ��ݕ�
	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"vertexMoveVS.hlsl",//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"main",//�G���g���[�|�C���g��
		"vs_5_0",//�V�F�[�_���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�N�p�ݒ�
		0,
		&vsBlob,
		&errorBlob
	);

#pragma endregion 

#pragma region ���_�V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ

	//���_�V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ
	if (FAILED(result))
	{
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void DrawingObj::pixelShaderGeneration()
{
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicPS.hlsl",//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"main",//�G���g���[�|�C���g��
		"ps_5_0",//�V�F�[�_���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�N�p�ݒ�
		0,
		&psBlob,
		&errorBlob
	);

#pragma endregion

#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ

	//�s�N�Z���V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ
	if (FAILED(result))
	{
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void DrawingObj::pixelShaderGeneration2()
{
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"colorChangePS.hlsl",//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"main",//�G���g���[�|�C���g��
		"ps_5_0",//�V�F�[�_���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�N�p�ݒ�
		0,
		&psBlob,
		&errorBlob
	);

#pragma endregion

#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ

	//�s�N�Z���V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ
	if (FAILED(result))
	{
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void DrawingObj::vertexLayout()
{
#pragma region ���_���C�A�E�g�̐ݒ�

	//���_���C�A�E�g
	//�O���t�B�b�N�p�C�v���C���Œ��_����̃f�[�^�ɉ����������邩���߂�
	//����͍Œ����3D���W�����������Ă���炵��
	inputLayout[0] =
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
	inputLayout[1] =
	{//�@���x�N�g��
		"NORMAL",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
	};

	inputLayout[2] =
	{//UV���W
		"TEXCOORD",
		0,
		DXGI_FORMAT_R32G32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
	};

#pragma endregion
}

void DrawingObj::graphicPipelineGeneration()
{
#pragma region �O���t�B�b�N�X�p�C�v���C���ݒ�

	//�O���t�B�b�N�X�p�C�v���C����

	//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	gpipeline.VS.BytecodeLength = vsBlob->GetBufferSize();
	gpipeline.PS.pShaderBytecode = psBlob->GetBufferPointer();
	gpipeline.PS.BytecodeLength = psBlob->GetBufferSize();

	//�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O���Ȃ�
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//�|���S�����h��Ԃ�
	gpipeline.RasterizerState.DepthClipEnable = true;//�[�x�N���b�s���O��L����

	//�u�����h�X�e�[�g�̐ݒ�
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA�S�Ẵ`�����l����`��
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region �u�����h�X�e�[�g�̋��ʐݒ�(�A���t�@�l)

	blenddesc.BlendEnable = true;//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g��

#pragma endregion


#pragma region ���Z����

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

#pragma region ����������

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l

#pragma endregion



	//���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	gpipeline.NumRenderTargets = 1;//�`��Ώۂ͂P��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1;//�P�s�N�Z���ɂ��P��T���v�����O

#pragma endregion �ʏ�`��|���S������h��Ԃ�(�O�p�`)

#pragma region �O���t�B�b�N�X�p�C�v���C���Q�̐ݒ�

//�O���t�B�b�N�X�p�C�v���C����
	

	//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline2.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	gpipeline2.VS.BytecodeLength = vsBlob->GetBufferSize();
	gpipeline2.PS.pShaderBytecode = psBlob->GetBufferPointer();
	gpipeline2.PS.BytecodeLength = psBlob->GetBufferSize();

	//�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
	gpipeline2.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�
	gpipeline2.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;//�J�����O���Ȃ�
	gpipeline2.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;//���C���[�t���[���`��
	gpipeline2.RasterizerState.DepthClipEnable = true;//�[�x�N���b�s���O��L����

	//�u�����h�X�e�[�g�̐ݒ�
	//gpipeline2.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA�S�Ẵ`�����l����`��
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc2 = gpipeline2.BlendState.RenderTarget[0];
	blenddesc2.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region ���ʐݒ�(�A���t�@�l)

	blenddesc2.BlendEnable = true;//�u�����h��L���ɂ���
	blenddesc2.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blenddesc2.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blenddesc2.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g��

#pragma endregion

#pragma region ����������

	blenddesc2.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	blenddesc2.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̃A���t�@�l
	blenddesc2.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l

#pragma endregion

	//���_���C�A�E�g�̐ݒ�
	gpipeline2.InputLayout.pInputElementDescs = inputLayout;
	gpipeline2.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline2.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	gpipeline2.NumRenderTargets = 1;//�`��Ώۂ͂P��
	gpipeline2.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0�`255�w���RGBA
	gpipeline2.SampleDesc.Count = 1;//�P�s�N�Z���ɂ��P��T���v�����O

#pragma endregion �ʏ�`�惏�C���[�t���[���`��(�O�p�`)

}

void DrawingObj::descriptorRangeGeneration()
{
#pragma region �f�X�N���v�^�����W�̐ݒ�
	descriptorRange.NumDescriptors = 1;//��x�̕`��Ɏg���e�N�X�`�����P���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;//�e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

#pragma endregion
}

void DrawingObj::rootParamGeneration()
{
#pragma region ���[�g�p�����[�^

	//���[�g�p�����[�^�̐ݒ�
	
	//�F
	rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@�r���[
	rootParam[0].Descriptor.ShaderRegister = 0;//�萔�o�b�t�@�ԍ�
	rootParam[0].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_�[���猩����

	//�ʒu
	rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@�r���[
	rootParam[1].Descriptor.ShaderRegister = 1;//�萔�o�b�t�@�ԍ�
	rootParam[1].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_�[���猩����

	//�摜�f�[�^�p
	rootParam[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//���
	rootParam[2].DescriptorTable.pDescriptorRanges = &descriptorRange;//�f�X�N���v�^�����W
	rootParam[2].DescriptorTable.NumDescriptorRanges = 1;//�f�X�N���v�^�����W��
	rootParam[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_���猩����

	rootParam[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@�r���[
	rootParam[3].Descriptor.ShaderRegister = 2;//�萔�o�b�t�@�ԍ�
	rootParam[3].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParam[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_�[���猩����

#pragma endregion �萔�o�b�t�@�𑝂₵���瑝�₷�Ƃ��낪����
}

void DrawingObj::textureSamplerGeneration()
{
#pragma region �e�N�X�`���T���v���[�̐ݒ�

	//�e�N�X�`���T���v���[�̐ݒ�
	sampleDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���J��Ԃ�(�^�C�����O)
	sampleDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//�c�J��Ԃ�(�^�C�����O)
	sampleDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���s�J��Ԃ�(�^�C�����O)
	sampleDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//�{�[�_�[�̎��͍�
	sampleDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//�S�ă��j�A���
	sampleDesc.MaxLOD = D3D12_FLOAT32_MAX;//�~�b�v�}�b�v�ő�l
	sampleDesc.MinLOD = 0.0f;//�~�b�v�}�b�v�ŏ��l
	sampleDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampleDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//�s�N�Z���V�F�[�_����̂ݎg�p�\

#pragma endregion
}

void DrawingObj::rootsignatureGeneration(ID3D12Device* dev)
{
#pragma region ���[�g�V�O�l�`���ݒ�

	//���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParam;//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParam);//���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &sampleDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	//���[�g�V�O�l�`���̐���
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	

	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = rootsignature.Get();

	//�p�C�v���C��2�ɂ����[�g�V�O�l�`�����Z�b�g
	gpipeline2.pRootSignature = rootsignature.Get();

#pragma endregion �萔�o�b�t�@�𑝂₵���烋�[�g�p�����[�^�����������p�����[�^��������������

#pragma region �f�v�X�X�e���V���X�e�[�g

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	gpipeline.DepthStencilState.DepthEnable = true;//�[�x�e�X�g���s��
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//�������݋���
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//��������΍��i
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g

#pragma endregion


#pragma region �p�C�v���C���X�e�[�g�̐���

	//�p�C�v���C���X�e�[�g�̐���
	
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

	//�p�C�v���C���X�e�[�g�Q�̐���
	
	result = dev->CreateGraphicsPipelineState(&gpipeline2, IID_PPV_ARGS(&pipelinestate2));

#pragma endregion

}

void DrawingObj::constantBuffGeneration(ID3D12Device* dev)
{
#pragma region �萔�o�b�t�@

	//�萔�o�b�t�@�̐����p�̐ݒ�
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p


	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};

	cbResourceDesc = constBuffResourceGeneration(sizeof(ConstBufferDataMaterial));
	
	//�萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial)
	);
	assert(SUCCEEDED(result));

	cbResourceDesc = constBuffResourceGeneration(sizeof(ConstBufferDataMaterial2));
	
	//�萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial2)
	);
	assert(SUCCEEDED(result));

	//cbResourceDesc = constBuffResourceGeneration(sizeof(ConstBufferDataTransform));

	////�萔�o�b�t�@�̐���
	//result = dev->CreateCommittedResource(
	//	&cbHeapProp,//�q�[�v�ݒ�
	//	D3D12_HEAP_FLAG_NONE,
	//	&cbResourceDesc,//���\�[�X�ݒ�
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuffTransform0)
	//);
	//assert(SUCCEEDED(result));


	////�萔�o�b�t�@�̃}�b�s���O
	//
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);//�}�b�s���O
	assert(SUCCEEDED(result));

	//�}�b�s���O�������GPU��VRSM��CPU�ƘA������
	//Unmap������ƂȂ��肪��������邪�萔�o�b�t�@�͏��������邱�Ƃ������̂�
	//���Ȃ��Ă����v

	//�l���������ނƎ����I�ɓ]�������



	constMapMaterial->color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	
	result = constBuffMaterial2->Map(0, nullptr, (void**)&constMapMaterial2);//�}�b�s���O
	assert(SUCCEEDED(result));

	constMapMaterial2->posM = XMFLOAT4(0,0,0,1);

	//result = constBuffTransform0->Map(0, nullptr, (void**)&constMapTransform0);//�}�b�s���O
	//assert(SUCCEEDED(result));

	//�s��ɒP�ʍs�����
	/*matWorld.IdentityMatrix();

	matScale.IdentityMatrix();

	matRotate.IdentityMatrix();

	matTrans.IdentityMatrix();

	constMapTransform->mat.IdentityMatrix();*/

	/*constMapTransform->mat.r[0].m128_f32[0] = 2.0f / Win_width;
	constMapTransform->mat.r[1].m128_f32[1] = -2.0f / Win_height;
	constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
	constMapTransform->mat.r[3].m128_f32[1] = 1.0f;*/

	//���s���ˍs��̌v�Z
	//constMapTransform->mat = XMMatrixOrthographicOffCenterLH(0.0f,Win_width,Win_height, 0.0f, 0.0f, 1.0f);

	//�������e�s��̌v�Z
	matProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), (float)Win_width / Win_height, 0.1f, 1000.0f);

	/*matProjection = perspectiveProjectionGeneration((45.0f * (PI / 180)), Win_width / Win_height, 0.1f, 1000.0f);

	matView = matViewGeneration(eye_, target_, up_);*/

	 matView = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
	 
	 /*Scale_ = { 1.0f,1.0f,1.0f };
	 Rotate_ = { 0.0f,0.0f,0.0f };
	 Trans_ = { 0.0f,0.0f,0.0f };

	 matWorldUpdata();

	 constTransformMatUpdata();*/

	 for (int i = 0; i < _countof(object3Ds); i++)
	 {
		 //������
		 object3Ds[i].Init(dev);

		 if (i > 0)
		 {
			 //�ЂƂO�̃I�u�W�F�N�g��e�I�u�W�F�N�g�Ƃ���
			 object3Ds[i].SetParent(&object3Ds[i - 1]);

			 //�e�I�u�W�F�N�g��9���̑傫��
			 object3Ds[i].SetScale({ 0.9f,0.9f,0.9f });

			 //�e�I�u�W�F�N�g�ɑ΂���z�������30�x��]
			 object3Ds[i].SetRotate({ 0.0f,0.0f,XMConvertToRadians(30.0f) });

			 //�e�I�u�W�F�N�g�ɑ΂���z����-8.0���炷
			 object3Ds[i].SetPos({ 0.0f,0.0f,-8.0f });

			 object3Ds[i].Update(matView, matProjection);

		 }

	 }

	 for (size_t i = 0; i < _countof(object3Ds); i++)
	 {
		 object3Ds[i].Update(matView, matProjection);
	 }

#pragma endregion
}

void DrawingObj::constantBuffGeneration1(ID3D12Device* dev)
{
#pragma region �萔�o�b�t�@

	//�萔�o�b�t�@�̐����p�̐ݒ�
	//�q�[�v�ݒ�
	//D3D12_HEAP_PROPERTIES cbHeapProp{};
	//cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p


	////���\�[�X�ݒ�
	//D3D12_RESOURCE_DESC cbResourceDesc{};

	//cbResourceDesc = constBuffResourceGeneration(sizeof(ConstBufferDataTransform));

	////�萔�o�b�t�@�̐���
	//result = dev->CreateCommittedResource(
	//	&cbHeapProp,//�q�[�v�ݒ�
	//	D3D12_HEAP_FLAG_NONE,
	//	&cbResourceDesc,//���\�[�X�ݒ�
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuffTransform1)
	//);
	//assert(SUCCEEDED(result));


	////�萔�o�b�t�@�̃}�b�s���O

	//result = constBuffTransform1->Map(0, nullptr, (void**)&constMapTransform1);//�}�b�s���O
	//assert(SUCCEEDED(result));

	////�s��ɒP�ʍs�����
	///*matWorld.IdentityMatrix();

	//matScale.IdentityMatrix();

	//matRotate.IdentityMatrix();

	//matTrans.IdentityMatrix();

	//constMapTransform->mat.IdentityMatrix();*/

	///*constMapTransform->mat.r[0].m128_f32[0] = 2.0f / Win_width;
	//constMapTransform->mat.r[1].m128_f32[1] = -2.0f / Win_height;
	//constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
	//constMapTransform->mat.r[3].m128_f32[1] = 1.0f;*/

	////���s���ˍs��̌v�Z
	////constMapTransform->mat = XMMatrixOrthographicOffCenterLH(0.0f,Win_width,Win_height, 0.0f, 0.0f, 1.0f);

	//matWorldUpdata1();

	//constTransformMatUpdata1();

#pragma endregion
}

D3D12_RESOURCE_DESC DrawingObj::constBuffResourceGeneration(int size)
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

void DrawingObj::indicesBuffGeneration(ID3D12Device* dev)
{
#pragma region �C���f�b�N�X�o�b�t�@

	//���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;//�C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�C���f�b�N�X�o�b�t�@�̐���
	
	result = dev->CreateCommittedResource(
		&heapprop,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];//�C���f�b�N�X���R�s�[
	}

	//�}�b�s���O����
	indexBuff->Unmap(0, nullptr);

#pragma endregion ���_��ʂ鏇�Ԃ����߂�ꏊ

#pragma region �C���f�b�N�X�o�b�t�@�r���[

	//�C���f�b�N�X�o�b�t�@�r���[�̐���
	
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

#pragma endregion

}

void DrawingObj::imageDataGeneration()
{
#pragma region �摜�C���[�W�f�[�^�̍쐬

	//���삵���C���[�W�f�[�^
	////�������s�N�Z����
	//const size_t textureWidth = 256;
	////�c�����s�N�Z����
	//const size_t textureHeight = 256;
	////�z��̗v�f��
	//const size_t imageDataCount = textureWidth * textureHeight;
	////�摜�C���[�W�f�[�^�z��
	//XMFLOAT4* imageData = new XMFLOAT4[imageDataCount];//�K����ŉ������

	//for (size_t i = 0; i < imageDataCount; i++)
	//{
	//	imageData[i].x = 0.0f;//R
	//	imageData[i].y = 1.0f;//G
	//	imageData[i].z = 0.0f;//B
	//	imageData[i].w = 1.0f;//A

	//}

	//�摜�ǂݍ��݂��ĉ摜�C���[�W�f�[�^�𐶐�
	

	//WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		L"Resources/hokehoke.png",
		WIC_FLAGS_NONE,
		&metadata,
		scratchImg
	);

	ScratchImage mipChain{};
	//�~�b�v�}�b�v�̐���
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

	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

#pragma endregion
}

void DrawingObj::textureBuffGeneraion(ID3D12Device* dev)
{
#pragma region �e�N�X�`���o�b�t�@�ݒ�

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;//��
	textureResourceDesc.Height = (UINT)metadata.height;//����
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

#pragma endregion

#pragma region �e�N�X�`���o�b�t�@�̐���

	//�e�N�X�`���o�b�t�@�̐���
	
	result = dev->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);

	//�~�b�v�}�b�v�Œu��������
	//�S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,//�S�̈�փR�s�[
			img->pixels,//���f�[�^�A�h���X
			(UINT)img->rowPitch,//1���C���A�h���X
			(UINT)img->slicePitch//1���T�C�Y
		);
		assert(SUCCEEDED(result));
	}

	//result = texBuff->WriteToSubresource(
	//	0,
	//	nullptr,//�S�̈�փR�s�[
	//	imageData,//���f�[�^�A�h���X
	//	sizeof(XMFLOAT4) * textureWidth,//1���C���T�C�Y
	//	sizeof(XMFLOAT4) * imageDataCount//�S�T�C�Y
	//);

	//delete[] imageData;

#pragma endregion �����ŉ摜�C���[�W�f�[�^��delete���Ă���(�R�����g�A�E�g��)
}

void DrawingObj::SRVGeneraion(ID3D12Device* dev)
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
	
	//result = dev->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	//assert(SUCCEEDED(result));

	////�f�X�N���v�^�n���h��(�q�[�v���̑��삷��ꏊ�w��Ɏg��)
	////SRV�q�[�v�̐擪�n���h�����擾
	//D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

#pragma endregion

#pragma region �V�F�[�_���\�[�X�r���[

	//�V�F�[�_���\�[�X�r���[
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	srvDesc.Format = resDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_���\�[�X�r���[�쐬
	//dev->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);

#pragma endregion
}

void DrawingObj::vertexMap()
{

	vertBuff->Map(0, nullptr, (void**)&vertMap);
	//�S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];//���W���R�s�[
	}

	//�Ȃ��������
	vertBuff->Unmap(0, nullptr);
}

void DrawingObj::Draw(ID3D12GraphicsCommandList* cmdList,bool PipeLineRuleFlag, bool ChangeSquareFlag)
{

	//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�
	//������p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`�����Z�b�g����
	//���߂����[���ŕ`������肢����Ƃ���
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;

	if (PipeLineRuleFlag)
	{
		cmdList->SetPipelineState(pipelinestate.Get());
	}
	else
	{
		cmdList->SetPipelineState(pipelinestate2.Get());
	}

	cmdList->SetGraphicsRootSignature(rootsignature.Get());

	//���_�o�b�t�@�r���[�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &vbView);



	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffMaterial2->GetGPUVirtualAddress());


	//SRV�q�[�v�̐ݒ�R�}���h
	//cmdList->SetDescriptorHeaps(1, &texture.srvHeap);
	////SRV�q�[�v�̐擪�n���h�����擾(SRV���w���Ă���͂�)
	//srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
	////SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^�Q�Ԃɐݒ�
	//cmdList->SetGraphicsRootDescriptorTable(2, srvGpuHandle);

	texture.Draw(cmdList);

	cmdList->IASetIndexBuffer(&ibView);

	for (int i = 0; i < _countof(object3Ds); i++)
	{
		object3Ds[i].Draw(cmdList, vbView, ibView, _countof(indices),ChangeSquareFlag);
	}

	////�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h(��ԍŏ��̈�����"���[�g�p�����[�^"�̗v�f�ԍ��ł���)
	//cmdList->SetGraphicsRootConstantBufferView(3, constBuffTransform0->GetGPUVirtualAddress());

	////�`��R�}���h
	//if (ChangeSquareFlag)
	//{
	//	//�l�p�`�ɕ`��
	//	cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
	//}
	//else
	//{
	//	cmdList->DrawInstanced(3, 1, 0, 0);
	//}

	////�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h(��ԍŏ��̈�����"���[�g�p�����[�^"�̗v�f�ԍ��ł���)
	//cmdList->SetGraphicsRootConstantBufferView(3, constBuffTransform1->GetGPUVirtualAddress());

	////�`��R�}���h
	//if (ChangeSquareFlag)
	//{
	//	//�l�p�`�ɕ`��
	//	cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
	//}
	//else
	//{
	//	cmdList->DrawInstanced(3, 1, 0, 0);
	//}
}

XMFLOAT3 DrawingObj::Afin(XMFLOAT3 box, float moveX, float moveY, float rotate, float scaleX, float scaleY)
{
	float ansBuff[3] = {};
	float ansBuff2[3] = {};
	float ansBuff3[3] = {};
	float ansBuff4[3] = {};
	float ans[3] = {};

	float moveA[3][3] = {
		{1.0f,0.0f, moveX},
		{0.0f,1.0f, moveY},
		{0.0f,0.0f, 1.0f}
	};
	float rotateA[3][3] = {
		{cos(rotate * (PI / 180)),-sin(rotate * (PI / 180)), 0.0f},
		{sin(rotate * (PI / 180)),cos(rotate * (PI / 180)), 0.0f},
		{0.0f,0.0f, 1.0f}
	};

	float scaleA[3][3] = {
		{scaleX,0.0f, 0.0f},
		{0.0f,scaleY, 0.0f},
		{0.0f,0.0f, 1.0f}
	};

	ans[0] = box.x;
	ans[1] = box.y;
	ans[2] = 1.0f;

	ansBuff[0] = moveA[0][0] * vertices[0].pos.x + moveA[0][1] * vertices[0].pos.y + -ans[0] * 1.0f;
	ansBuff[1] = moveA[1][0] * vertices[0].pos.x + moveA[1][1] * vertices[0].pos.y + -ans[1] * 1.0f;
	ansBuff[2] = moveA[2][0] * vertices[0].pos.x + moveA[2][1] * vertices[0].pos.y + ans[2] * 1.0f;


	ansBuff2[0] = rotateA[0][0] * ansBuff[0] + rotateA[0][1] * ansBuff[1] + rotateA[0][2] * ansBuff[2];
	ansBuff2[1] = rotateA[1][0] * ansBuff[0] + rotateA[1][1] * ansBuff[1] + rotateA[1][2] * ansBuff[2];
	ansBuff2[2] = rotateA[2][0] * ansBuff[0] + rotateA[2][1] * ansBuff[1] + rotateA[2][2] * ansBuff[2];

	ansBuff3[0] = scaleA[0][0] * ansBuff2[0] + scaleA[0][1] * ansBuff2[1] + scaleA[0][2] * ansBuff2[2];
	ansBuff3[1] = scaleA[1][0] * ansBuff2[0] + scaleA[1][1] * ansBuff2[1] + scaleA[1][2] * ansBuff2[2];
	ansBuff3[2] = scaleA[2][0] * ansBuff2[0] + scaleA[2][1] * ansBuff2[1] + scaleA[2][2] * ansBuff2[2];

	ansBuff4[0] = moveA[0][0] * ansBuff3[0] + moveA[0][1] * ansBuff3[1] + ans[0] * ansBuff3[2];
	ansBuff4[1] = moveA[1][0] * ansBuff3[0] + moveA[1][1] * ansBuff3[1] + ans[1] * ansBuff3[2];
	ansBuff4[2] = moveA[2][0] * ansBuff3[0] + moveA[2][1] * ansBuff3[1] + ans[2] * ansBuff3[2];

	box.x = moveA[0][0] * ansBuff4[0] + moveA[0][1] * ansBuff4[1] + moveA[0][2] * ansBuff4[2];
	box.y = moveA[1][0] * ansBuff4[0] + moveA[1][1] * ansBuff4[1] + moveA[1][2] * ansBuff4[2];

	return box;

}

//Obj�ɑ΂��ăA�t�B���ϊ���������֐�
void DrawingObj::ObjAfin(float moveX, float moveY, float rotate, float scaleX, float scaleY)
{
#pragma region �A�t�B���ϊ�

	//���_�͍Ō�ɕϊ�����(�����W���ς���Ă��܂�����)
	vertices[1].pos = Afin(vertices[1].pos, moveX, moveY, rotate, scaleX, scaleY);
	vertices[2].pos = Afin(vertices[2].pos, moveX, moveY, rotate, scaleX, scaleY);
	vertices[3].pos = Afin(vertices[3].pos, moveX, moveY, rotate, scaleX, scaleY);
	vertices[0].pos = Afin(vertices[0].pos, moveX, moveY, rotate, scaleX, scaleY);

#pragma endregion

#pragma region �A�t�B���ϊ���̒��_�f�[�^���V�F�[�_�ɓ]��

	vertexMap();

#pragma endregion
}

//�萔�o�b�t�@��ύX����֐�
void DrawingObj::constBuffColorUpdata(float Red, float Green, float Blue)
{
	constMapMaterial->color = XMFLOAT4(Red, Green, Blue, 1.0f);
}

void DrawingObj::constBuffPosMUpdata(float X, float Y, float Z)
{
	constMapMaterial2->posM = XMFLOAT4(X, Y, Z, 0.0f);
}

void DrawingObj::matViewUpdata(XMFLOAT3 eye, XMFLOAT3 target, XMFLOAT3 up)
{
	eye_ = eye;
	target_ = target;
	up_ = up;

	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
	//matView = matViewGeneration(eye_, target_, up_);

	for (size_t i = 0; i < _countof(object3Ds); i++)
	{
		object3Ds[i].Update(matView, matProjection);
	}
	
}

void DrawingObj::constTransformMatUpdata()
{
	constMapTransform0->mat = matWorld * matView * matProjection;
}

void DrawingObj::constTransformMatUpdata1()
{
	constMapTransform1->mat = matWorld1 * matView * matProjection;
}

void DrawingObj::matWorldUpdata()
{

	////�X�P�[���s��X�V
	//matScale = matScaleGeneration(Scale_);

	////��]�s��X�V
	//matRotate = matRotateGeneration(Rotate_);

	////���s�ړ��s��X�V
	//matTrans = matMoveGeneration(Trans_);

	////���[���h�s��X�V
	//matWorld.IdentityMatrix();
	//matWorld *= matScale*matRotate*matTrans;

	for (size_t i = 0; i < _countof(object3Ds); i++)
	{
		object3Ds[i].Update(matView, matProjection);
	}
	
}

void DrawingObj::matWorldUpdata1()
{

	////�X�P�[���s��X�V
	//matScale = matScaleGeneration(Scale_);

	////��]�s��X�V
	//matRotate = matRotateGeneration(Rotate_);

	////���s�ړ��s��X�V
	//matTrans = matMoveGeneration(Trans_);

	////���[���h�s��X�V
	//matWorld.IdentityMatrix();
	//matWorld *= matScale*matRotate*matTrans;

	

	XMMATRIX matScale1;
	XMMATRIX matRotate1;
	XMMATRIX matTrans1;
	// �X�P�[���s��X�V
	matScale1 = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	//��]�s��X�V
	matRotate1 = XMMatrixIdentity();

	matRotate1 *= XMMatrixRotationY(XM_PI/4.0f);

	//���s�ړ��s��X�V
	matTrans1 = XMMatrixTranslation(-20.0f, 0, 0);

	//���[���h�s��X�V
	matWorld1 = XMMatrixIdentity();
	matWorld1 *= matScale1;
	matWorld1 *= matRotate1;
	matWorld1 *= matTrans1;

	constTransformMatUpdata1();

}


void DrawingObj::SetScale(XMFLOAT3 scale)
{
	Scale_ = scale;
}

void DrawingObj::SetRotate(XMFLOAT3 rotate)
{
	Rotate_ = rotate;
}

void DrawingObj::SetTrans(XMFLOAT3 TransForm)
{
	Trans_ = TransForm;
}

Matrix4x4 DrawingObj::matScaleGeneration(Float3 scale)
{
	//�X�P�[�����O�s���錾
	Matrix4x4 matScale;
	matScale.IdentityMatrix();

	//�X�P�[�����O�{�����s��ɐݒ�
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1;

	return matScale;
}

Matrix4x4 DrawingObj::matRotateXGeneration(float rotateX)
{
	//X����]�s���錾
	Matrix4x4 matRotateX;
	matRotateX.IdentityMatrix();

	//��]�p���s��ɐݒ�(���W�A��)
	matRotateX.m[0][0] = 1;
	matRotateX.m[1][1] = cos(rotateX);
	matRotateX.m[1][2] = sin(rotateX);
	matRotateX.m[2][1] = -sin(rotateX);
	matRotateX.m[2][2] = cos(rotateX);
	matRotateX.m[3][3] = 1;

	return matRotateX;
}

Matrix4x4 DrawingObj::matRotateYGeneration(float rotateY)
{
	//Y����]�s���錾
	Matrix4x4 matRotateY;
	matRotateY.IdentityMatrix();

	//��]�p���s��ɐݒ�(���W�A��)
	matRotateY.m[0][0] = cos(rotateY);
	matRotateY.m[0][2] = -sin(rotateY);
	matRotateY.m[1][1] = 1;
	matRotateY.m[2][0] = sin(rotateY);
	matRotateY.m[2][2] = cos(rotateY);
	matRotateY.m[3][3] = 1;

	return matRotateY;
}

Matrix4x4 DrawingObj::matRotateZGeneration(float rotateZ)
{
	//Z����]�s���錾
	Matrix4x4 matRotateZ;
	matRotateZ.IdentityMatrix();

	//��]�p���s��ɐݒ�(���W�A��)
	matRotateZ.m[0][0] = cos(rotateZ);
	matRotateZ.m[0][1] = sin(rotateZ);
	matRotateZ.m[1][0] = -sin(rotateZ);
	matRotateZ.m[1][1] = cos(rotateZ);
	matRotateZ.m[2][2] = 1;
	matRotateZ.m[3][3] = 1;

	return matRotateZ;
}

Matrix4x4 DrawingObj::matRotateGeneration(Float3 rotate)
{
	//X����]�s���錾
	Matrix4x4 matRotateX = matRotateXGeneration(rotate.x);

	//Y����]�s���錾
	Matrix4x4 matRotateY = matRotateYGeneration(rotate.y);

	//Z����]�s���錾
	Matrix4x4 matRotateZ = matRotateZGeneration(rotate.z);

	//��]�������s���錾
	Matrix4x4 matRotate;
	matRotate.IdentityMatrix();

	//�v�Z�����p�x���v�Z(���Ԃ͉�]�����郂�f���ɂ���ĕς���)

	matRotateX *= matRotateY;

	matRotateZ *= matRotateX;

	matRotate = matRotateZ;

	return matRotate;

	

}

Matrix4x4 DrawingObj::matMoveGeneration(Float3 translation)
{
	//�ړ����邽�߂̍s���p��
	Matrix4x4 matMove;
	matMove.IdentityMatrix();

	//�s��Ɉړ��ʂ���
	matMove.m[3][0] = translation.x;
	matMove.m[3][1] = translation.y;
	matMove.m[3][2] = translation.z;

	return matMove;
}

Matrix4x4 DrawingObj::matViewGeneration(Float3 eye, Float3 target, Float3 up)
{

	Float3 zVer = target - eye;

	zVer.normalize();

	Float3 xVer = up.cross(zVer);

	xVer.normalize();

	Float3 yVer = zVer.cross(xVer);


	Matrix4x4 cameraRotateMat;

	cameraRotateMat.IdentityMatrix();

	cameraRotateMat.m[0][0] = xVer.x;
	cameraRotateMat.m[0][1] = xVer.y;
	cameraRotateMat.m[0][2] = xVer.z;

	cameraRotateMat.m[1][0] = yVer.x;
	cameraRotateMat.m[1][1] = yVer.y;
	cameraRotateMat.m[1][2] = yVer.z;

	cameraRotateMat.m[2][0] = zVer.x;
	cameraRotateMat.m[2][1] = zVer.y;
	cameraRotateMat.m[2][2] = zVer.z;

	Float3 cameraPos;
	Float3 Eye = eye;
	Eye = -Eye;

	cameraPos.x = Eye.dot(xVer);
	cameraPos.y = Eye.dot(yVer);
	cameraPos.z = Eye.dot(zVer);


	Matrix4x4 cameraMat;

	cameraMat.IdentityMatrix();

	Matrix4x4 cameRotMat = {};
	cameRotMat.IdentityMatrix();
	cameRotMat.m[0][0] = cameraRotateMat.m[0][0];
	cameRotMat.m[1][0] = cameraRotateMat.m[0][1];
	cameRotMat.m[2][0] = cameraRotateMat.m[0][2];
	cameRotMat.m[3][0] = cameraRotateMat.m[0][3];

	cameRotMat.m[0][1] = cameraRotateMat.m[1][0];
	cameRotMat.m[1][1] = cameraRotateMat.m[1][1];
	cameRotMat.m[2][1] = cameraRotateMat.m[1][2];
	cameRotMat.m[3][1] = cameraRotateMat.m[1][3];

	cameRotMat.m[0][2] = cameraRotateMat.m[2][0];
	cameRotMat.m[1][2] = cameraRotateMat.m[2][1];
	cameRotMat.m[2][2] = cameraRotateMat.m[2][2];
	cameRotMat.m[3][2] = cameraRotateMat.m[2][3];

	cameraMat = cameraRotateMat.InverseMatrix();

	cameraMat.m[3][0] = cameraPos.x;
	cameraMat.m[3][1] = cameraPos.y;
	cameraMat.m[3][2] = cameraPos.z;

	return cameraMat;
}

Matrix4x4 DrawingObj::perspectiveProjectionGeneration(float FovAngleY, float aspect, float NearZ, float FarZ)
{
	Matrix4x4 ans = {};

	ans.m[0][0] = 1 / tanf(2);
	ans.m[1][1] = 1 / tanf(2) * aspect;
	ans.m[2][2] = (FarZ + NearZ) / (FarZ - NearZ);
	ans.m[2][3] = 1.0f;
	ans.m[3][2] = -((2 * (FarZ * NearZ)) / (FarZ - NearZ));

	return ans;

}
