#include "Sprite.h"

void Sprite::initialize(ID3D12Device* dev)
{

	vertices[0] = { -0.5f,-0.5f,0.0f };
	vertices[1] = { -0.5f,+0.5f,0.0f };
	vertices[2] = { +0.5f,-0.5f,0.0f };

	sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));

	vertexBuffGeneration(dev);

}

void Sprite::vertexBuffGeneration(ID3D12Device* dev)
{
#pragma region 頂点バッファの確保

	//頂点バッファの確保辺

	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用


	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;//頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

#pragma endregion

#pragma region 頂点バッファの生成

	//頂点バッファの生成

	result = dev->CreateCommittedResource(
		&heapprop,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);

	assert(SUCCEEDED(result));

#pragma endregion
#pragma region 頂点バッファへのデータ転送

	//頂点バッファへのデータ転送
	//GPU上のバッファに対応した仮想メモリを取得
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);

	assert(SUCCEEDED(result));

	//全頂点に対して
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];//座標をコピー
	}


	//つながりを解除
	vertBuff->Unmap(0, nullptr);

#pragma endregion

#pragma region 頂点バッファビューの生成

	//頂点バッファビューの作成(GPUへの道しるべ)辺

	//頂点バッファビューの作成

	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(XMFLOAT3);

#pragma endregion

}

void Sprite::Draw(ID3D12GraphicsCommandList* cmdList, SpriteCommon& spriteCommon, bool PipeLineRuleFlag)
{

	//パイプラインステートとルートシグネチャの設定
	//作ったパイプラインステートとルートシグネチャをセットする
	//決めたルールで描画をお願いするところ

	if (PipeLineRuleFlag)
	{
		cmdList->SetPipelineState(spriteCommon.getPipelinestate().Get());
	}
	else
	{
		cmdList->SetPipelineState(spriteCommon.getPipelinestate2().Get());
	}

	cmdList->SetGraphicsRootSignature(spriteCommon.getRootsignature().Get());


	//頂点バッファビューの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);


	//定数バッファビュー(CBV)の設定コマンド
	/*cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffMaterial2->GetGPUVirtualAddress());*/

	//cmdList->IASetVertexBuffers(0, 1, &vbView);

	//定数バッファビュー(CBV)の設定コマンド

	cmdList->DrawInstanced(_countof(vertices), 1, 0, 0);

}