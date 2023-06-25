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
	
	//プリミティブ形状(三角形リスト)
	DirectXInit::GetInstance()->GetcmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//パイプラインステートとルートシグネチャの設定
	//作ったパイプラインステートとルートシグネチャをセットする
	//決めたルールで描画をお願いするところ

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


	//頂点バッファビューの設定
	DirectXInit::GetInstance()->GetcmdList()->IASetVertexBuffers(0, 1, &vbView_);


	//定数バッファビュー(CBV)の設定コマンド
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