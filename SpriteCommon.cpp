#include "SpriteCommon.h"

void SpriteCommon::initialize(ID3D12Device* dev)
{

	vertexShaderGeneration();

	pixelShaderGeneration();

	vertexLayout();

	graphicPipelineGeneration();

	rootsignatureGeneration(dev);
}



void SpriteCommon::vertexShaderGeneration()
{
#pragma region ���_�V�F�[�_�t�@�C���̓ǂݍ��݂ƃR���p�C��

	//���_�V�F�[�_�t�@�C���̓ǂݍ��ݕ�
	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/SpriteVS.hlsl",//�V�F�[�_�t�@�C����
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

void SpriteCommon::pixelShaderGeneration()
{
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/SpritePS.hlsl",//�V�F�[�_�t�@�C����
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

void SpriteCommon::vertexLayout()
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

#pragma endregion
}

void SpriteCommon::graphicPipelineGeneration()
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
	gpipeline2.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O���Ȃ�
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

#pragma region �O���t�B�b�N�X�p�C�v���C��3�ݒ�

//�O���t�B�b�N�X�p�C�v���C����

//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline3.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	gpipeline3.VS.BytecodeLength = vsBlob->GetBufferSize();
	gpipeline3.PS.pShaderBytecode = psBlob->GetBufferPointer();
	gpipeline3.PS.BytecodeLength = psBlob->GetBufferSize();

	//�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
	gpipeline3.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�
	gpipeline3.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O���Ȃ�
	gpipeline3.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//�|���S�����h��Ԃ�
	gpipeline3.RasterizerState.DepthClipEnable = true;//�[�x�N���b�s���O��L����


	//�u�����h�X�e�[�g�̐ݒ�
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA�S�Ẵ`�����l����`��
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc3 = gpipeline3.BlendState.RenderTarget[0];
	blenddesc3.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region �u�����h�X�e�[�g�̋��ʐݒ�(�A���t�@�l)

	blenddesc3.BlendEnable = true;//�u�����h��L���ɂ���
	blenddesc3.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blenddesc3.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blenddesc3.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g��

#pragma endregion


#pragma region ���Z����

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

#pragma region ����������

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̃A���t�@�l
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l

#pragma endregion �R�����g�A�E�g��



	//���_���C�A�E�g�̐ݒ�
	gpipeline3.InputLayout.pInputElementDescs = inputLayout;
	gpipeline3.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline3.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	gpipeline3.NumRenderTargets = 1;//�`��Ώۂ͂P��
	gpipeline3.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0�`255�w���RGBA
	gpipeline3.SampleDesc.Count = 1;//�P�s�N�Z���ɂ��P��T���v�����O

#pragma endregion �ʏ�`��|���S������h��Ԃ�(�O�p�`)���Z����

#pragma region �O���t�B�b�N�X�p�C�v���C��4�ݒ�

//�O���t�B�b�N�X�p�C�v���C����

//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline4.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	gpipeline4.VS.BytecodeLength = vsBlob->GetBufferSize();
	gpipeline4.PS.pShaderBytecode = psBlob->GetBufferPointer();
	gpipeline4.PS.BytecodeLength = psBlob->GetBufferSize();

	//�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
	gpipeline4.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�
	gpipeline4.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O���Ȃ�
	gpipeline4.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//�|���S�����h��Ԃ�
	gpipeline4.RasterizerState.DepthClipEnable = true;//�[�x�N���b�s���O��L����


	//�u�����h�X�e�[�g�̐ݒ�
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA�S�Ẵ`�����l����`��
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc4 = gpipeline4.BlendState.RenderTarget[0];
	blenddesc4.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region �u�����h�X�e�[�g�̋��ʐݒ�(�A���t�@�l)

	blenddesc4.BlendEnable = true;//�u�����h��L���ɂ���
	blenddesc4.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blenddesc4.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blenddesc4.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g��

#pragma endregion


#pragma region ���Z����

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

#pragma region ����������

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̃A���t�@�l
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l

#pragma endregion �R�����g�A�E�g��



	//���_���C�A�E�g�̐ݒ�
	gpipeline4.InputLayout.pInputElementDescs = inputLayout;
	gpipeline4.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline4.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	gpipeline4.NumRenderTargets = 1;//�`��Ώۂ͂P��
	gpipeline4.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0�`255�w���RGBA
	gpipeline4.SampleDesc.Count = 1;//�P�s�N�Z���ɂ��P��T���v�����O

#pragma endregion �ʏ�`��|���S������h��Ԃ�(�O�p�`)���Z����

#pragma region �O���t�B�b�N�X�p�C�v���C��5�ݒ�

//�O���t�B�b�N�X�p�C�v���C����

//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline5.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	gpipeline5.VS.BytecodeLength = vsBlob->GetBufferSize();
	gpipeline5.PS.pShaderBytecode = psBlob->GetBufferPointer();
	gpipeline5.PS.BytecodeLength = psBlob->GetBufferSize();

	//�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
	gpipeline5.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�
	gpipeline5.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O���Ȃ�
	gpipeline5.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//�|���S�����h��Ԃ�
	gpipeline5.RasterizerState.DepthClipEnable = true;//�[�x�N���b�s���O��L����


	//�u�����h�X�e�[�g�̐ݒ�
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA�S�Ẵ`�����l����`��
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc5 = gpipeline5.BlendState.RenderTarget[0];
	blenddesc5.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region �u�����h�X�e�[�g�̋��ʐݒ�(�A���t�@�l)

	blenddesc5.BlendEnable = true;//�u�����h��L���ɂ���
	blenddesc5.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blenddesc5.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blenddesc5.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g��

#pragma endregion


#pragma region ���Z����

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

#pragma region ����������

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̃A���t�@�l
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l

#pragma endregion �R�����g�A�E�g��



	//���_���C�A�E�g�̐ݒ�
	gpipeline5.InputLayout.pInputElementDescs = inputLayout;
	gpipeline5.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline5.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	gpipeline5.NumRenderTargets = 1;//�`��Ώۂ͂P��
	gpipeline5.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0�`255�w���RGBA
	gpipeline5.SampleDesc.Count = 1;//�P�s�N�Z���ɂ��P��T���v�����O

#pragma endregion �ʏ�`��|���S������h��Ԃ�(�O�p�`)�F���]

}

void SpriteCommon::rootsignatureGeneration(ID3D12Device* dev)
{
#pragma region ���[�g�V�O�l�`���ݒ�

	//���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	

	//���[�g�V�O�l�`���̐���
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));


	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = rootsignature.Get();

	//�p�C�v���C��2�ɂ����[�g�V�O�l�`�����Z�b�g
	gpipeline2.pRootSignature = rootsignature.Get();

	gpipeline3.pRootSignature = rootsignature.Get();

	gpipeline4.pRootSignature = rootsignature.Get();

	gpipeline5.pRootSignature = rootsignature.Get();

#pragma endregion �萔�o�b�t�@�𑝂₵���烋�[�g�p�����[�^�����������p�����[�^��������������

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

	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

	//�p�C�v���C���X�e�[�g�Q�̐���

	result = dev->CreateGraphicsPipelineState(&gpipeline2, IID_PPV_ARGS(&pipelinestate2));

	result = dev->CreateGraphicsPipelineState(&gpipeline3, IID_PPV_ARGS(&pipelinestate3));

	result = dev->CreateGraphicsPipelineState(&gpipeline4, IID_PPV_ARGS(&pipelinestate4));

	result = dev->CreateGraphicsPipelineState(&gpipeline5, IID_PPV_ARGS(&pipelinestate5));

#pragma endregion

}

