#include "PostEffect.h"
#include "Input.h"

//ヘルパー構造体使うのに追加した
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

	//テクスチャを赤クリア
	{
		HRESULT result;
		//画素数(1280x720=921600ピクセル)
		const uint32_t pixelCount = WinApp::SWindowWidth_ * WinApp::SWindowHeight_;

		//画像1行分のデータサイズ
		const uint32_t rowPitch = sizeof(uint32_t) * WinApp::SWindowWidth_;

		//画像全体のデータサイズ
		const uint32_t depthPitch = rowPitch * WinApp::SWindowHeight_;

		//画像イメージ
		std::vector<uint32_t> img;
		img.resize(pixelCount);

		for (uint32_t i = 0; i < pixelCount; i++)
		{
			img[i] = 0xff0000ff;
		}

		//テクスチャバッファにデータ転送
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

}

void PostEffect::Draw(uint16_t PipeLineRuleFlag)
{
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

	
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;
	//uint32_t incremantSize_ = DirectXInit::GetInstance()->Getdev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//SRVヒープの設定コマンド
	DirectXInit::GetInstance()->GetcmdList()->SetDescriptorHeaps(1, descHeapSRV_.GetAddressOf());
	//SRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	srvGpuHandle = descHeapSRV_->GetGPUDescriptorHandleForHeapStart();


	//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	{
		
		DirectXInit::GetInstance()->GetcmdList()->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV_->GetGPUDescriptorHandleForHeapStart(),0,
			DirectXInit::GetInstance()->Getdev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
		DirectXInit::GetInstance()->GetcmdList()->SetGraphicsRootDescriptorTable(3, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV_->GetGPUDescriptorHandleForHeapStart(), 1,
			DirectXInit::GetInstance()->Getdev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	}
	

	DirectXInit::GetInstance()->GetcmdList()->DrawInstanced(_countof(vertices_), 1, 0, 0);

}

void  PostEffect::PreDrawScene()
{
	DirectXInit* directXinit = DirectXInit::GetInstance();

	for (uint16_t i = 0; i < _countof(texBuff_); i++)
	{

		barrierDesc_.Transition.pResource = texBuff_[i].Get();
		barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;//表示から
		barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;//描画
		directXinit->GetcmdList()->ResourceBarrier(1, &barrierDesc_);
	}

	//リソースバリアを変更(シェーダーリソース描画可能)
	/*directXinit->GetcmdList()->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(texBuff_.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET)
	);*/

	//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[2];

	for (uint16_t i = 0; i < _countof(rtvHs); i++)
	{
		rtvHs[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapRTV_->GetCPUDescriptorHandleForHeapStart(), i,
			directXinit->Getdev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
	}

	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV_->GetCPUDescriptorHandleForHeapStart();

	//レンダーターゲットをセット
	directXinit->GetcmdList()->OMSetRenderTargets(2, rtvHs, false, &dsvH);

	//ビューポート設定
	//分割分のビューポートを用意(見にくいので減らした)
	D3D12_VIEWPORT viewports[2]{};
	for (uint16_t i = 0; i < _countof(rtvHs); i++)
	{
		viewports[i].Width = (float)WinApp::GetInstance()->getWindowSizeWidth();//横幅
		viewports[i].Height = (float)WinApp::GetInstance()->getWindowSizeHeight();//縦幅
		viewports[i].TopLeftX = 0;//左上X
		viewports[i].TopLeftY = 0;//左上Y
		viewports[i].MinDepth = 0.1f;//最少深度(0でよい)
		viewports[i].MaxDepth = 1.0f;//最大深度(１でよい)
	}

	//コマンドリストに追加
	directXinit->GetcmdList()->RSSetViewports(2, viewports);


	//シザー矩形設定
	D3D12_RECT scissorrects[2]{};

	for (uint16_t i = 0; i < _countof(rtvHs); i++)
	{
		scissorrects[i].left = 0;//切り抜き座標左
		scissorrects[i].right = scissorrects[i].left + WinApp::GetInstance()->getWindowSizeWidth();//切り抜き座標右
		scissorrects[i].top = 0;//切り抜き座標上
		scissorrects[i].bottom = scissorrects[i].top + WinApp::GetInstance()->getWindowSizeHeight();//切り抜き座標下
	}

	directXinit->GetcmdList()->RSSetScissorRects(2, scissorrects);

	for (uint16_t i = 0; i < _countof(rtvHs); i++)
	{
		//全画面クリア
		directXinit->GetcmdList()->ClearRenderTargetView(rtvHs[i], clearColor, 0, nullptr);
	}

	//深度バッファのクリア
	directXinit->GetcmdList()->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);



}

void  PostEffect::PostDrawScene()
{

	DirectXInit* directXinit = DirectXInit::GetInstance();

	for (uint16_t i = 0; i < _countof(texBuff_); i++)
	{
		// 5.リソースバリアを戻す(書き込める状態だと描画できないので描画できるようにする)
		barrierDesc_.Transition.pResource = texBuff_[i].Get();
		barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;//描画
		barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;//表示に
		directXinit->GetcmdList()->ResourceBarrier(1, &barrierDesc_);
	}

	



}

void PostEffect::textureBuffGeneraion()
{
	HRESULT result;
	//テクスチャリソース設定

	//ヘルパー構造体
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WinApp::SWindowWidth_,
		(uint32_t)WinApp::SWindowHeight_,
		1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET//ポストエフェクトのためのflag
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
		//テクスチャバッファの生成
		result = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
			&textureHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,//ポストエフェクト用
			&clearValue,
			IID_PPV_ARGS(&texBuff_[i])
		);

		assert(SUCCEEDED(result));
	}
}

void PostEffect::SRVDescHeapGeneraion()
{
	HRESULT result;
	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};

	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 2;

	//SRV用デスクリプタヒープを生成
	result = DirectXInit::GetInstance()->Getdev()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV_));
	assert(SUCCEEDED(result));

	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
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
	//RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 2;

	//RTV用デスクリプタヒープを生成
	result = DirectXInit::GetInstance()->Getdev()->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV_));
	assert(SUCCEEDED(result));

	//レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};

	//シェーダーの計算結果をSRGBに変換して書き込む
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	for (uint16_t i = 0; i < _countof(texBuff_); i++)
	{
		//デスクリプタヒープにRTV作成
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
	//リソース設定
	D3D12_RESOURCE_DESC depthResourceDesc{};

	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = WinApp::GetInstance()->getWindowSizeWidth();//レンダーターゲットに合わせる
	depthResourceDesc.Height = WinApp::GetInstance()->getWindowSizeHeight();//レンダーターゲットに合わせる
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;//デプステンシル

	//深度値用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;//深度値1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット

	//リソース生成

	result = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//深度値書き込みに使用
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff_)
	);

	//深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//デプスステンシルビュー
	result = DirectXInit::GetInstance()->Getdev()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&descHeapDSV_));
	assert(SUCCEEDED(result));

	//深度ビュー生成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DirectXInit::GetInstance()->Getdev()->CreateDepthStencilView(
		depthBuff_.Get(),
		&dsvDesc,
		descHeapDSV_->GetCPUDescriptorHandleForHeapStart()
	);

}

void PostEffect::vertexBuffGeneration()
{
#pragma region 頂点バッファの確保

	//頂点バッファの確保辺

	heapprop_.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用


	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeVB_;//頂点データ全体のサイズ
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

#pragma endregion

#pragma region 頂点バッファの生成

	//頂点バッファの生成

	result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
		&heapprop_,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc_,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_)
	);

	assert(SUCCEEDED(result_));

#pragma endregion
#pragma region 頂点バッファへのデータ転送

	//頂点バッファへのデータ転送
	//GPU上のバッファに対応した仮想メモリを取得
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap_);

	assert(SUCCEEDED(result_));

	//全頂点に対して
	for (uint16_t i = 0; i < _countof(vertices_); i++)
	{
		vertMap_[i] = vertices_[i];//座標をコピー
	}


	//つながりを解除
	vertBuff_->Unmap(0, nullptr);

#pragma endregion

#pragma region 頂点バッファビューの生成

	//頂点バッファビューの作成(GPUへの道しるべ)辺

	//頂点バッファビューの作成

	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeVB_;
	vbView_.StrideInBytes = sizeof(vertices_[0]);

#pragma endregion

}

void PostEffect::vertexShaderGeneration()
{
	//頂点シェーダファイルの読み込みとコンパイル
#pragma region 

	//頂点シェーダファイルの読み込み辺
	//頂点シェーダの読み込みとコンパイル
	result_ = D3DCompileFromFile(
		L"Resources/shaders/PostEffectVS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main",//エントリーポイント名
		"vs_5_0",//シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&vsBlob_,
		&errorBlob_
	);

#pragma endregion 

	//頂点シェーダの読み込み時のエラーを表示する場所
#pragma region 

	//頂点シェーダの読み込み時のエラーを表示する場所
	if (FAILED(result_))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob_->GetBufferSize());

		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容をウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void PostEffect::pixelShaderGeneration()
{
	//ピクセルシェーダの読み込みとコンパイル
#pragma region 

	//ピクセルシェーダの読み込みとコンパイル
	result_ = D3DCompileFromFile(
		L"Resources/shaders/PostEffectPS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main",//エントリーポイント名
		"ps_5_0",//シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&psBlob_,
		&errorBlob_
	);

#pragma endregion


	//ピクセルシェーダの読み込み時のエラーを表示する場所
#pragma region 

	//ピクセルシェーダの読み込み時のエラーを表示する場所
	if (FAILED(result_))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob_->GetBufferSize());

		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容をウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void PostEffect::vertexLayout()
{
#pragma region 頂点レイアウトの設定

	//頂点レイアウト
	//グラフィックパイプラインで頂点一つ分のデータに何を持たせるか決める
	//これは最低限の3D座標だけ持たせているらしい
	inputLayout_[0] =
	{
		"POSITION",//セマンティック名
		0,//同じセマンティック名が複数あるときのインデックス
		DXGI_FORMAT_R32G32B32_FLOAT,//要素数とビット数を返す(XYZの３つでfloat型なのでR32G32B32_FLOAT)
		0,//入力スロットインデックス
		D3D12_APPEND_ALIGNED_ELEMENT,//データのオフセット値(D3D12_APPEND_ALIGNED_ELEMENTだと自動設定)
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,//入力データ識別(標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
		0,//一度に描画するインスタンス数
	};
	//座標以外に　色、テクスチャUVなどを渡す場合はさらに続ける
	inputLayout_[1] =
	{
		"TEXCOORD",//セマンティック名
		0,//同じセマンティック名が複数あるときのインデックス
		DXGI_FORMAT_R32G32_FLOAT,//要素数とビット数を返す(XYの2つでfloat型なのでR32G32_FLOAT)
		0,//入力スロットインデックス
		D3D12_APPEND_ALIGNED_ELEMENT,//データのオフセット値(D3D12_APPEND_ALIGNED_ELEMENTだと自動設定)
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,//入力データ識別(標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
		0,//一度に描画するインスタンス数
	};


#pragma endregion
}

void PostEffect::graphicPipelineGeneration()
{
#pragma region グラフィックスパイプライン設定

	//グラフィックスパイプライン辺

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
	gpipeline_.VS.BytecodeLength = vsBlob_->GetBufferSize();

	/*gpipeline.GS.pShaderBytecode = common->getGsBlob()->GetBufferPointer();
	gpipeline.GS.BytecodeLength = common->getGsBlob()->GetBufferSize();*/

	gpipeline_.PS.pShaderBytecode =psBlob_->GetBufferPointer();
	gpipeline_.PS.BytecodeLength = psBlob_->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	gpipeline_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	gpipeline_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	//gpipeline.RasterizerState.DepthClipEnable = true;//深度クリッピングを有効に


	//ブレンドステートの設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline_.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region ブレンドステートの共通設定(アルファ値)

	blenddesc.BlendEnable = true;//ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//デストの値を0%使う

#pragma endregion

	//加算合成
#pragma region 

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//デストの値を100%使う

#pragma endregion コメントアウト中

#pragma region 減算合成

	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;//デストからソースを減算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//デストの値を100%使う

#pragma endregion コメントアウト中

#pragma region 色反転

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-デストカラーの値
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;//使わない

#pragma endregion コメントアウト中

	//半透明合成
#pragma region 

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-ソースのアルファ値

#pragma endregion



	//頂点レイアウトの設定
	gpipeline_.InputLayout.pInputElementDescs = inputLayout_;
	gpipeline_.InputLayout.NumElements = _countof(inputLayout_);

	//図形の形状を三角形に設定
	gpipeline_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline_.NumRenderTargets = 1;//描画対象は１つ
	gpipeline_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0〜255指定のRGBA
	gpipeline_.SampleDesc.Count = 1;//１ピクセルにつき１回サンプリング

#pragma endregion 通常描画ポリゴン内を塗りつぶし(三角形)

	//通常描画ワイヤーフレーム描画(三角形)
#pragma region グラフィックスパイプライン２の設定

//グラフィックスパイプライン辺


		//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline2_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
	gpipeline2_.VS.BytecodeLength = vsBlob_->GetBufferSize();

	/*gpipeline2.GS.pShaderBytecode = common->getGsBlob()->GetBufferPointer();
	gpipeline2.GS.BytecodeLength = common->getGsBlob()->GetBufferSize();*/

	gpipeline2_.PS.pShaderBytecode = psBlob_->GetBufferPointer();
	gpipeline2_.PS.BytecodeLength = psBlob_->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline2_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	gpipeline2_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	gpipeline2_.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;//ワイヤーフレーム描画
	//gpipeline2.RasterizerState.DepthClipEnable = true;//深度クリッピングを有効に

	//ブレンドステートの設定
	//gpipeline2.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc2 = gpipeline2_.BlendState.RenderTarget[0];
	blenddesc2.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region 共通設定(アルファ値)

	blenddesc2.BlendEnable = true;//ブレンドを有効にする
	blenddesc2.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
	blenddesc2.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使う
	blenddesc2.DestBlendAlpha = D3D12_BLEND_ZERO;//デストの値を0%使う

#pragma endregion

	//半透明合成
#pragma region 

	blenddesc2.BlendOp = D3D12_BLEND_OP_ADD;//加算
	blenddesc2.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースのアルファ値
	blenddesc2.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-ソースのアルファ値

#pragma endregion

	//頂点レイアウトの設定
	gpipeline2_.InputLayout.pInputElementDescs = inputLayout_;
	gpipeline2_.InputLayout.NumElements = _countof(inputLayout_);

	//図形の形状を三角形に設定
	gpipeline2_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline2_.NumRenderTargets = 1;//描画対象は１つ
	gpipeline2_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0〜255指定のRGBA
	gpipeline2_.SampleDesc.Count = 1;//１ピクセルにつき１回サンプリング

#pragma endregion 

	//通常描画ポリゴン内を塗りつぶし(三角形)加算合成
#pragma region グラフィックスパイプライン3設定

//グラフィックスパイプライン辺

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline3_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
	gpipeline3_.VS.BytecodeLength = vsBlob_->GetBufferSize();

	/*gpipeline3.GS.pShaderBytecode = common->getGsBlob()->GetBufferPointer();
	gpipeline3.GS.BytecodeLength = common->getGsBlob()->GetBufferSize();*/

	gpipeline3_.PS.pShaderBytecode = psBlob_->GetBufferPointer();
	gpipeline3_.PS.BytecodeLength = psBlob_->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline3_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	gpipeline3_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	gpipeline3_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	//gpipeline3.RasterizerState.DepthClipEnable = true;//深度クリッピングを有効に


	//ブレンドステートの設定
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc3 = gpipeline3_.BlendState.RenderTarget[0];
	blenddesc3.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region ブレンドステートの共通設定(アルファ値)

	blenddesc3.BlendEnable = true;//ブレンドを有効にする
	blenddesc3.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
	blenddesc3.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使う
	blenddesc3.DestBlendAlpha = D3D12_BLEND_ZERO;//デストの値を0%使う

#pragma endregion

	//加算合成
#pragma region 

	blenddesc3.BlendOp = D3D12_BLEND_OP_ADD;//加算
	blenddesc3.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使う
	blenddesc3.DestBlend = D3D12_BLEND_ONE;//デストの値を100%使う

#pragma endregion 

#pragma region 減算合成

	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;//デストからソースを減算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//デストの値を100%使う

#pragma endregion コメントアウト中

#pragma region 色反転

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-デストカラーの値
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;//使わない

#pragma endregion コメントアウト中

	//半透明合成
#pragma region 

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースのアルファ値
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-ソースのアルファ値

#pragma endregion コメントアウト中



	//頂点レイアウトの設定
	gpipeline3_.InputLayout.pInputElementDescs = inputLayout_;
	gpipeline3_.InputLayout.NumElements = _countof(inputLayout_);

	//図形の形状を三角形に設定
	gpipeline3_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline3_.NumRenderTargets = 1;//描画対象は１つ
	gpipeline3_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0〜255指定のRGBA
	gpipeline3_.SampleDesc.Count = 1;//１ピクセルにつき１回サンプリング

#pragma endregion 

#pragma region グラフィックスパイプライン4設定

	//グラフィックスパイプライン辺

		//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline4_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
	gpipeline4_.VS.BytecodeLength = vsBlob_->GetBufferSize();

	/*gpipeline4.GS.pShaderBytecode = common->getGsBlob()->GetBufferPointer();
	gpipeline4.GS.BytecodeLength = common->getGsBlob()->GetBufferSize();*/

	gpipeline4_.PS.pShaderBytecode = psBlob_->GetBufferPointer();
	gpipeline4_.PS.BytecodeLength = psBlob_->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline4_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	gpipeline4_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	gpipeline4_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	//gpipeline4.RasterizerState.DepthClipEnable = true;//深度クリッピングを有効に


	//ブレンドステートの設定
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc4 = gpipeline4_.BlendState.RenderTarget[0];
	blenddesc4.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region ブレンドステートの共通設定(アルファ値)

	blenddesc4.BlendEnable = true;//ブレンドを有効にする
	blenddesc4.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
	blenddesc4.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使う
	blenddesc4.DestBlendAlpha = D3D12_BLEND_ZERO;//デストの値を0%使う

#pragma endregion

	//加算合成
#pragma region 

	//blenddesc3.BlendOp = D3D12_BLEND_OP_ADD;//加算
	//blenddesc3.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使う
	//blenddesc3.DestBlend = D3D12_BLEND_ONE;//デストの値を100%使う

#pragma endregion コメントアウト中

#pragma region 減算合成

	blenddesc4.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;//デストからソースを減算
	blenddesc4.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使う
	blenddesc4.DestBlend = D3D12_BLEND_ONE;//デストの値を100%使う

#pragma endregion

#pragma region 色反転

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-デストカラーの値
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;//使わない

#pragma endregion コメントアウト中

	//半透明合成
#pragma region 

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースのアルファ値
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-ソースのアルファ値

#pragma endregion コメントアウト中



	//頂点レイアウトの設定
	gpipeline4_.InputLayout.pInputElementDescs = inputLayout_;
	gpipeline4_.InputLayout.NumElements = _countof(inputLayout_);

	//図形の形状を三角形に設定
	gpipeline4_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline4_.NumRenderTargets = 1;//描画対象は１つ
	gpipeline4_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0〜255指定のRGBA
	gpipeline4_.SampleDesc.Count = 1;//１ピクセルにつき１回サンプリング

#pragma endregion 通常描画ポリゴン内を塗りつぶし(三角形)減算合成

#pragma region グラフィックスパイプライン5設定

	//グラフィックスパイプライン辺

		//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline5_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
	gpipeline5_.VS.BytecodeLength = vsBlob_->GetBufferSize();

	/*gpipeline5.GS.pShaderBytecode = common->getGsBlob()->GetBufferPointer();
	gpipeline5.GS.BytecodeLength = common->getGsBlob()->GetBufferSize();*/

	gpipeline5_.PS.pShaderBytecode = psBlob_->GetBufferPointer();
	gpipeline5_.PS.BytecodeLength = psBlob_->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline5_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	gpipeline5_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	gpipeline5_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	//gpipeline5.RasterizerState.DepthClipEnable = true;//深度クリッピングを有効に


	//ブレンドステートの設定
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc5 = gpipeline5_.BlendState.RenderTarget[0];
	blenddesc5.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region ブレンドステートの共通設定(アルファ値)

	blenddesc5.BlendEnable = true;//ブレンドを有効にする
	blenddesc5.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
	blenddesc5.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使う
	blenddesc5.DestBlendAlpha = D3D12_BLEND_ZERO;//デストの値を0%使う

#pragma endregion

	//加算合成
#pragma region 

	//blenddesc3.BlendOp = D3D12_BLEND_OP_ADD;//加算
	//blenddesc3.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使う
	//blenddesc3.DestBlend = D3D12_BLEND_ONE;//デストの値を100%使う

#pragma endregion コメントアウト中

#pragma region 減算合成

	//blenddesc4.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;//デストからソースを減算
	//blenddesc4.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使う
	//blenddesc4.DestBlend = D3D12_BLEND_ONE;//デストの値を100%使う

#pragma endregion コメントアウト中

#pragma region 色反転

	blenddesc5.BlendOp = D3D12_BLEND_OP_ADD;//加算
	blenddesc5.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-デストカラーの値
	blenddesc5.DestBlend = D3D12_BLEND_ZERO;//使わない

#pragma endregion 

	// 半透明合成
#pragma region

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースのアルファ値
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-ソースのアルファ値

#pragma endregion コメントアウト中



	//頂点レイアウトの設定
	gpipeline5_.InputLayout.pInputElementDescs = inputLayout_;
	gpipeline5_.InputLayout.NumElements = _countof(inputLayout_);

	//図形の形状を三角形に設定
	gpipeline5_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline5_.NumRenderTargets = 1;//描画対象は１つ
	gpipeline5_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0〜255指定のRGBA
	gpipeline5_.SampleDesc.Count = 1;//１ピクセルにつき１回サンプリング

#pragma endregion 通常描画ポリゴン内を塗りつぶし(三角形)色反転

}

void PostEffect::rootParamGeneration()
{
	//定数バッファを増やしたら増やすところがある
#pragma region ルートパラメータ

	//ルートパラメータの設定

	//色
	rootParam_[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//定数バッファビュー
	rootParam_[0].Descriptor.ShaderRegister = 0;//定数バッファ番号
	rootParam_[0].Descriptor.RegisterSpace = 0;//デフォルト値
	rootParam_[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える

	//画像データ用
	rootParam_[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//種類
	rootParam_[1].DescriptorTable.pDescriptorRanges = &descriptorRange_;//デスクリプタレンジ
	rootParam_[1].DescriptorTable.NumDescriptorRanges = 1;//デスクリプタレンジ数
	rootParam_[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダから見える

	//色
	rootParam_[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//定数バッファビュー
	rootParam_[2].Descriptor.ShaderRegister = 1;//定数バッファ番号
	rootParam_[2].Descriptor.RegisterSpace = 0;//デフォルト値
	rootParam_[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える

	//画像データ用
	rootParam_[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//種類
	rootParam_[3].DescriptorTable.pDescriptorRanges = &descriptorRange2_;//デスクリプタレンジ
	rootParam_[3].DescriptorTable.NumDescriptorRanges = 1;//デスクリプタレンジ数
	rootParam_[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダから見える

#pragma endregion 
}

void PostEffect::rootsignatureGeneration()
{
	//定数バッファを増やしたらルートパラメータを書き換えパラメータ数を書き換える
#pragma region ルートシグネチャ設定

	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParam_;
	rootSignatureDesc.NumParameters = _countof(rootParam_);//ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &sampleDesc_;
	rootSignatureDesc.NumStaticSamplers = 1;


	//ルートシグネチャの生成
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob_);
	result_ = DirectXInit::GetInstance()->Getdev()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature_));


	//パイプラインにルートシグネチャをセット
	gpipeline_.pRootSignature = rootsignature_.Get();

	//パイプライン2にもルートシグネチャをセット
	gpipeline2_.pRootSignature = rootsignature_.Get();

	gpipeline3_.pRootSignature = rootsignature_.Get();

	gpipeline4_.pRootSignature = rootsignature_.Get();

	gpipeline5_.pRootSignature = rootsignature_.Get();

#pragma endregion 

#pragma region デプスステンシルステート

	////デプスステンシルステートの設定
	//gpipeline.DepthStencilState.DepthEnable = true;//深度テストを行う
	//gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
	//gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//小さければ合格
	//gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット

	//gpipeline2.DepthStencilState.DepthEnable = true;//深度テストを行う
	//gpipeline2.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
	//gpipeline2.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//小さければ合格
	//gpipeline2.DSVFormat = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット



#pragma endregion


#pragma region パイプラインステートの生成

	//パイプラインステートの生成

	result_ = DirectXInit::GetInstance()->Getdev()->CreateGraphicsPipelineState(&gpipeline_, IID_PPV_ARGS(&pipelinestate_));

	//パイプラインステート２の生成

	result_ = DirectXInit::GetInstance()->Getdev()->CreateGraphicsPipelineState(&gpipeline2_, IID_PPV_ARGS(&pipelinestate2_));

	result_ = DirectXInit::GetInstance()->Getdev()->CreateGraphicsPipelineState(&gpipeline3_, IID_PPV_ARGS(&pipelinestate3_));

	result_ = DirectXInit::GetInstance()->Getdev()->CreateGraphicsPipelineState(&gpipeline4_, IID_PPV_ARGS(&pipelinestate4_));

	result_ = DirectXInit::GetInstance()->Getdev()->CreateGraphicsPipelineState(&gpipeline5_, IID_PPV_ARGS(&pipelinestate5_));

#pragma endregion

}

void PostEffect::constantBuffGeneration()
{
#pragma region 定数バッファ

	//定数バッファの生成用の設定

	cbHeapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用


	cbResourceDesc_ = constBuffResourceGeneration(sizeof(ConstBufferMaterial));

	result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
		&cbHeapProp_,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial_));
	assert(SUCCEEDED(result_));


	result_ = constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial_);//マッピング
	assert(SUCCEEDED(result_));

	cbHeapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用


	cbResourceDesc_ = constBuffResourceGeneration(sizeof(ConstBuffDataTransform));

	result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
		&cbHeapProp_,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform_));
	assert(SUCCEEDED(result_));


	result_ = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform_);//マッピング
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
	cbResourceDesc.Width = (size + 0xff) & ~0xff;//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	return cbResourceDesc;
}

void PostEffect::descriptorRangeGeneration()
{
#pragma region デスクリプタレンジの設定
	descriptorRange_.NumDescriptors = 1;//一度の描画に使うテクスチャが１枚なので1
	descriptorRange_.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange_.BaseShaderRegister = 0;//テクスチャレジスタ0番
	descriptorRange_.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	descriptorRange2_.NumDescriptors = 1;//一度の描画に使うテクスチャが１枚なので1
	descriptorRange2_.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange2_.BaseShaderRegister = 1;//テクスチャレジスタ1番
	descriptorRange2_.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

#pragma endregion
}

void PostEffect::textureSamplerGeneration()
{
	//テクスチャサンプラーの設定
#pragma region 

	//テクスチャサンプラーの設定
	sampleDesc_.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;//横繰り返し(タイリング)
	sampleDesc_.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;//縦繰り返し(タイリング)
	sampleDesc_.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;//奥行繰り返し(タイリング)
	sampleDesc_.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//ボーダーの時は黒
	sampleDesc_.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//全てリニア補間
	sampleDesc_.MaxLOD = D3D12_FLOAT32_MAX;//ミップマップ最大値
	sampleDesc_.MinLOD = 0.0f;//ミップマップ最小値
	sampleDesc_.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampleDesc_.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//ピクセルシェーダからのみ使用可能

#pragma endregion
}