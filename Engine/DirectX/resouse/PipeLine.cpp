#include "PipeLine.h"



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
	gpipeline_.PrimitiveTopologyType = pipeLineSeting.PrimitiveTopologyType;

	//���̑��̐ݒ�
	//�`��Ώۂ̐���ݒ�
	gpipeline_.NumRenderTargets = (uint32_t)pipeLineSeting.RTVFormats.size();

	//�`��ݒ����ʂ̐������p��?
	for (uint16_t i = 0; i < pipeLineSeting.RTVFormats.size(); i++)
	{
		gpipeline_.RTVFormats[i] = pipeLineSeting.RTVFormats[i];
	}

	//�P�s�N�Z���ɂ�����T���v�����O���邩
	gpipeline_.SampleDesc.Count = pipeLineSeting.SampleDesc.Count;

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