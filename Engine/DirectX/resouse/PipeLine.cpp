#include "PipeLine.h"
#include "DirectXInit.h"


PipeLine::PipeLine(PipeLineSeting pipeLineSeting)
{
#pragma region �O���t�B�b�N�X�p�C�v���C���ݒ�

	//�O���t�B�b�N�X�p�C�v���C����

	//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline_.VS.pShaderBytecode = pipeLineSeting.vShader.blob_->GetBufferPointer();
	gpipeline_.VS.BytecodeLength = pipeLineSeting.vShader.blob_->GetBufferSize();
	gpipeline_.PS.pShaderBytecode = pipeLineSeting.pShader.blob_->GetBufferPointer();
	gpipeline_.PS.BytecodeLength = pipeLineSeting.pShader.blob_->GetBufferSize();

	//�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
	gpipeline_.SampleMask = pipeLineSeting.sampleMask;//�W���ݒ�
	gpipeline_.RasterizerState = pipeLineSeting.rasterizerState;



	//�u�����h�X�e�[�g�̐ݒ�
	for (uint16_t i = 0; i < pipeLineSeting.blendDescs.size(); i++)
	{
		gpipeline_.BlendState.RenderTarget[i] = pipeLineSeting.blendDescs[i];
	}
	
	//���_���C�A�E�g�̐ݒ�
	gpipeline_.InputLayout.pInputElementDescs = pipeLineSeting.inputLayouts.data();
	gpipeline_.InputLayout.NumElements = (uint32_t)pipeLineSeting.inputLayouts.size();

	//�}�`�̌`���ݒ�
	gpipeline_.PrimitiveTopologyType = pipeLineSeting.primitiveTopologyType;

	//���̑��̐ݒ�
	//�`��Ώۂ̐���ݒ�
	gpipeline_.NumRenderTargets = (uint32_t)pipeLineSeting.RTVFormats.size();

	//�`��ݒ����ʂ̐������p��?
	for (uint16_t i = 0; i < pipeLineSeting.RTVFormats.size(); i++)
	{
		gpipeline_.RTVFormats[i] = pipeLineSeting.RTVFormats[i];
	}

	//�P�s�N�Z���ɂ�����T���v�����O���邩
	gpipeline_.SampleDesc.Count = pipeLineSeting.sampleDesc.Count;

	//�f�u�X�X�e���V���X�e�[�g
	gpipeline_.DepthStencilState = pipeLineSeting.depthDesc;
	gpipeline_.DSVFormat = pipeLineSeting.DSVFormat;

	gpipeline_.pRootSignature = pipeLineSeting.rootsignature;

	HRESULT result;
	result = DirectXInit::GetInstance()->Getdev()->CreateGraphicsPipelineState(&gpipeline_, IID_PPV_ARGS(&pipelinestate_));

	Roadsuccess = SUCCEEDED(result);
	
	seting = pipeLineSeting;


#pragma endregion 
}

PipeLine PipeLine::CreatePipeLine(std::string id, PipeLineSeting pipeLineSeting)
{
	PipeLine newPipeLine = SearchPipeLines(id);

	if (!newPipeLine.Roadsuccess)
	{
		newPipeLine = PipeLine(pipeLineSeting);
	}

	RegisterPipeLine(id, newPipeLine);

	return newPipeLine;
}

PipeLine PipeLine::SearchPipeLines(std::string id)
{
	//���ǂݍ��񂾂��Ƃ�����t�@�C���͂��̂܂ܕԂ�
	auto itr = std::find_if(PipeLines::GetInstance()->pipeLines_.begin(), PipeLines::GetInstance()->pipeLines_.end(), [&](const std::pair<std::string, PipeLine>& p) {
		return p.first == id;//����
		});
	//���������炻���Ԃ�
	if (itr != PipeLines::GetInstance()->pipeLines_.end()) {
		return PipeLines::GetInstance()->pipeLines_[id];
	}

	return PipeLine();
}

void PipeLine::RegisterPipeLine(std::string id, PipeLine pipeLine)
{
	PipeLines::GetInstance()->pipeLines_[id] = pipeLine;
}

PipeLineSeting PipeLine::defCreatePipeLineSeting(const Shader& vs, const Shader& ps, const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout, ID3D12RootSignature* rootsignature)
{
	PipeLineSeting seting1;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline_{};

	seting1.vShader = vs;
	seting1.pShader = ps;
	seting1.sampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	seting1.rasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	seting1.rasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	seting1.rasterizerState.DepthClipEnable = true;

	D3D12_RENDER_TARGET_BLEND_DESC blenddesc = gpipeline_.BlendState.RenderTarget[0];
	gpipeline_.BlendState.RenderTarget[1] = blenddesc;
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendEnable = true;//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g��
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l

	seting1.blendDescs.push_back(blenddesc);
	seting1.blendDescs.push_back(blenddesc);

	seting1.inputLayouts = inputLayout;
	seting1.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	seting1.numRenderTargets = 2;
	seting1.RTVFormats.push_back(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
	seting1.RTVFormats.push_back(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);

	seting1.sampleDesc.Count = 1;

	seting1.depthDesc.DepthEnable = true;//�[�x�e�X�g���s��
	seting1.depthDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//�������݋���
	seting1.depthDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//��������΍��i
	seting1.DSVFormat = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g

	seting1.rootsignature = rootsignature;

	return seting1;

}