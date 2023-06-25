#include "PostEffect.h"
#include "DirectXInit.h"

PostEffect::PostEffect()
{
	

}

void PostEffect::Initialize(uint32_t ChangeTexure)
{
	initialize(SpriteCommon::GetInstance(), ChangeTexure);
}

void PostEffect::Draw(uint32_t ChangeTexure, uint16_t PipeLineRuleFlag)
{

	

	if (texIndex == UINT32_MAX && ChangeTexure == 0)
	{
		assert(0);
	}
	
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

	if (texIndex != UINT32_MAX && ChangeTexure == 0)
	{
		STexture_->Draw(texIndex);
	}
	else
	{
		STexture_->Draw(ChangeTexure);
	}

	DirectXInit::GetInstance()->GetcmdList()->DrawInstanced(_countof(vertices_), 1, 0, 0);

}