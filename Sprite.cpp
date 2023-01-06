#include "Sprite.h"

Texture* Sprite::texture = nullptr;

const float PI = 3.141592653589f;


Sprite::Sprite()
{

	

}

void Sprite::initialize(ID3D12Device* dev, SpriteCommon* spriteCommon)
{
	texture = Texture::GetInstance(dev);
	common = spriteCommon;

	vertices[0] = { {-0.4,-0.7f,0.0f},{0.0f,1.0f} };
	vertices[1] = { {-0.4,+0.7f,0.0f},{0.0f,0.0f} };
	vertices[2] = { {+0.4,-0.7f,0.0f},{1.0f,1.0f} };

	vertices[3] = { {-0.4,+0.7f,0.0f},{0.0f,0.0f} };
	vertices[4] = { {+0.4,-0.7f,0.0f},{1.0f,1.0f} };
	vertices[5] = { {+0.4,+0.7f,0.0f},{1.0f,0.0f} };

	sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	vertexBuffGeneration(dev);

	vertexLayout();

	graphicPipelineGeneration();

	descriptorRangeGeneration();

	textureSamplerGeneration();

	rootParamGeneration();

	rootsignatureGeneration(dev);

	constantBuffGeneration(dev);

	texture->Init(dev);

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
	for(int i = 0; i < _countof(vertices); i++)
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
	vbView.StrideInBytes = sizeof(vertices[0]);

#pragma endregion

}

void Sprite::Draw(ID3D12GraphicsCommandList* cmdList, int ChangeTexure, int PipeLineRuleFlag)
{
	
	vertices[0] = { {-0.4,-0.7f,0.0f},{0.0f,1.0f} };
	vertices[1] = { {-0.4,+0.7f,0.0f},{0.0f,0.0f} };
	vertices[2] = { {+0.4,-0.7f,0.0f},{1.0f,1.0f} };

	vertices[3] = { {-0.4,+0.7f,0.0f},{0.0f,0.0f} };
	vertices[4] = { {+0.4,-0.7f,0.0f},{1.0f,1.0f} };
	vertices[5] = { {+0.4,+0.7f,0.0f},{1.0f,0.0f} };
	

	oldPosX += posX / 1000;
	oldPosY += posY / 1000;

	ObjAfin(oldPosX, oldPosY, rotate, scaleX, scaleY);

	//プリミティブ形状(三角形リスト)
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//パイプラインステートとルートシグネチャの設定
	//作ったパイプラインステートとルートシグネチャをセットする
	//決めたルールで描画をお願いするところ

	switch (PipeLineRuleFlag)
	{
	case 0:
		cmdList->SetPipelineState(pipelinestate.Get());
		break;

	case 1:
		cmdList->SetPipelineState(pipelinestate2.Get());
		break;

	case 2:
		cmdList->SetPipelineState(pipelinestate3.Get());
		break;

	case 3:
		cmdList->SetPipelineState(pipelinestate4.Get());
		break;

	case 4:
		cmdList->SetPipelineState(pipelinestate5.Get());
		break;

	default:
		cmdList->SetPipelineState(pipelinestate.Get());
		break;
	}
		

	

	cmdList->SetGraphicsRootSignature(rootsignature.Get());


	//頂点バッファビューの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);


	//定数バッファビュー(CBV)の設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

	texture->Draw(cmdList, ChangeTexure);

	cmdList->DrawInstanced(_countof(vertices), 1, 0, 0);


	posX = 0;
	posY = 0;
}

void Sprite::vertexLayout()
{
#pragma region 頂点レイアウトの設定

	//頂点レイアウト
	//グラフィックパイプラインで頂点一つ分のデータに何を持たせるか決める
	//これは最低限の3D座標だけ持たせているらしい
	inputLayout[0] =
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
	inputLayout[1] =
	{
		"TEXCOORD",//セマンティック名
		0,//同じセマンティック名が複数あるときのインデックス
		DXGI_FORMAT_R32G32_FLOAT,//要素数とビット数を返す(XYZの３つでfloat型なのでR32G32B32_FLOAT)
		0,//入力スロットインデックス
		D3D12_APPEND_ALIGNED_ELEMENT,//データのオフセット値(D3D12_APPEND_ALIGNED_ELEMENTだと自動設定)
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,//入力データ識別(標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
		0,//一度に描画するインスタンス数
	};


#pragma endregion
}

void Sprite::graphicPipelineGeneration()
{
#pragma region グラフィックスパイプライン設定

	//グラフィックスパイプライン辺

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline.VS.pShaderBytecode = common->getVsBlob()->GetBufferPointer();
	gpipeline.VS.BytecodeLength = common->getVsBlob()->GetBufferSize();

	/*gpipeline.GS.pShaderBytecode = common->getGsBlob()->GetBufferPointer();
	gpipeline.GS.BytecodeLength = common->getGsBlob()->GetBufferSize();*/

	gpipeline.PS.pShaderBytecode = common->getPsBlob()->GetBufferPointer();
	gpipeline.PS.BytecodeLength = common->getPsBlob()->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	//gpipeline.RasterizerState.DepthClipEnable = true;//深度クリッピングを有効に


	//ブレンドステートの設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];
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
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状を三角形に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline.NumRenderTargets = 1;//描画対象は１つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1;//１ピクセルにつき１回サンプリング

#pragma endregion 通常描画ポリゴン内を塗りつぶし(三角形)

	//通常描画ワイヤーフレーム描画(三角形)
#pragma region グラフィックスパイプライン２の設定

//グラフィックスパイプライン辺


		//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline2.VS.pShaderBytecode = common->getVsBlob()->GetBufferPointer();
	gpipeline2.VS.BytecodeLength = common->getVsBlob()->GetBufferSize();

	/*gpipeline2.GS.pShaderBytecode = common->getGsBlob()->GetBufferPointer();
	gpipeline2.GS.BytecodeLength = common->getGsBlob()->GetBufferSize();*/

	gpipeline2.PS.pShaderBytecode = common->getPsBlob()->GetBufferPointer();
	gpipeline2.PS.BytecodeLength = common->getPsBlob()->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline2.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	gpipeline2.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	gpipeline2.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;//ワイヤーフレーム描画
	//gpipeline2.RasterizerState.DepthClipEnable = true;//深度クリッピングを有効に

	//ブレンドステートの設定
	//gpipeline2.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc2 = gpipeline2.BlendState.RenderTarget[0];
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
	gpipeline2.InputLayout.pInputElementDescs = inputLayout;
	gpipeline2.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状を三角形に設定
	gpipeline2.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline2.NumRenderTargets = 1;//描画対象は１つ
	gpipeline2.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0～255指定のRGBA
	gpipeline2.SampleDesc.Count = 1;//１ピクセルにつき１回サンプリング

#pragma endregion 

	//通常描画ポリゴン内を塗りつぶし(三角形)加算合成
#pragma region グラフィックスパイプライン3設定

//グラフィックスパイプライン辺

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline3.VS.pShaderBytecode = common->getVsBlob()->GetBufferPointer();
	gpipeline3.VS.BytecodeLength = common->getVsBlob()->GetBufferSize();

	/*gpipeline3.GS.pShaderBytecode = common->getGsBlob()->GetBufferPointer();
	gpipeline3.GS.BytecodeLength = common->getGsBlob()->GetBufferSize();*/

	gpipeline3.PS.pShaderBytecode = common->getPsBlob()->GetBufferPointer();
	gpipeline3.PS.BytecodeLength = common->getPsBlob()->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline3.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	gpipeline3.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	gpipeline3.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	//gpipeline3.RasterizerState.DepthClipEnable = true;//深度クリッピングを有効に


	//ブレンドステートの設定
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc3 = gpipeline3.BlendState.RenderTarget[0];
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
	gpipeline3.InputLayout.pInputElementDescs = inputLayout;
	gpipeline3.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状を三角形に設定
	gpipeline3.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline3.NumRenderTargets = 1;//描画対象は１つ
	gpipeline3.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0～255指定のRGBA
	gpipeline3.SampleDesc.Count = 1;//１ピクセルにつき１回サンプリング

#pragma endregion 

#pragma region グラフィックスパイプライン4設定

//グラフィックスパイプライン辺

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline4.VS.pShaderBytecode = common->getVsBlob()->GetBufferPointer();
	gpipeline4.VS.BytecodeLength = common->getVsBlob()->GetBufferSize();

	/*gpipeline4.GS.pShaderBytecode = common->getGsBlob()->GetBufferPointer();
	gpipeline4.GS.BytecodeLength = common->getGsBlob()->GetBufferSize();*/

	gpipeline4.PS.pShaderBytecode = common->getPsBlob()->GetBufferPointer();
	gpipeline4.PS.BytecodeLength = common->getPsBlob()->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline4.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	gpipeline4.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	gpipeline4.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	//gpipeline4.RasterizerState.DepthClipEnable = true;//深度クリッピングを有効に


	//ブレンドステートの設定
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc4 = gpipeline4.BlendState.RenderTarget[0];
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
	gpipeline4.InputLayout.pInputElementDescs = inputLayout;
	gpipeline4.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状を三角形に設定
	gpipeline4.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline4.NumRenderTargets = 1;//描画対象は１つ
	gpipeline4.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0～255指定のRGBA
	gpipeline4.SampleDesc.Count = 1;//１ピクセルにつき１回サンプリング

#pragma endregion 通常描画ポリゴン内を塗りつぶし(三角形)減算合成

#pragma region グラフィックスパイプライン5設定

//グラフィックスパイプライン辺

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline5.VS.pShaderBytecode = common->getVsBlob()->GetBufferPointer();
	gpipeline5.VS.BytecodeLength = common->getVsBlob()->GetBufferSize();

	/*gpipeline5.GS.pShaderBytecode = common->getGsBlob()->GetBufferPointer();
	gpipeline5.GS.BytecodeLength = common->getGsBlob()->GetBufferSize();*/

	gpipeline5.PS.pShaderBytecode = common->getPsBlob()->GetBufferPointer();
	gpipeline5.PS.BytecodeLength = common->getPsBlob()->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline5.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	gpipeline5.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	gpipeline5.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	//gpipeline5.RasterizerState.DepthClipEnable = true;//深度クリッピングを有効に


	//ブレンドステートの設定
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc5 = gpipeline5.BlendState.RenderTarget[0];
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
	gpipeline5.InputLayout.pInputElementDescs = inputLayout;
	gpipeline5.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状を三角形に設定
	gpipeline5.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline5.NumRenderTargets = 1;//描画対象は１つ
	gpipeline5.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0～255指定のRGBA
	gpipeline5.SampleDesc.Count = 1;//１ピクセルにつき１回サンプリング

#pragma endregion 通常描画ポリゴン内を塗りつぶし(三角形)色反転

}

void Sprite::rootParamGeneration()
{
	//定数バッファを増やしたら増やすところがある
#pragma region ルートパラメータ

	//ルートパラメータの設定

	//色
	rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//定数バッファビュー
	rootParam[0].Descriptor.ShaderRegister = 0;//定数バッファ番号
	rootParam[0].Descriptor.RegisterSpace = 0;//デフォルト値
	rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える

	//画像データ用
	rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//種類
	rootParam[1].DescriptorTable.pDescriptorRanges = &descriptorRange;//デスクリプタレンジ
	rootParam[1].DescriptorTable.NumDescriptorRanges = 1;//デスクリプタレンジ数
	rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダから見える

#pragma endregion 
}

void Sprite::rootsignatureGeneration(ID3D12Device* dev)
{
	//定数バッファを増やしたらルートパラメータを書き換えパラメータ数を書き換える
#pragma region ルートシグネチャ設定

	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParam;
	rootSignatureDesc.NumParameters = _countof(rootParam);//ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &sampleDesc;
	rootSignatureDesc.NumStaticSamplers = 1;


	//ルートシグネチャの生成
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));


	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature.Get();

	//パイプライン2にもルートシグネチャをセット
	gpipeline2.pRootSignature = rootsignature.Get();

	gpipeline3.pRootSignature = rootsignature.Get();

	gpipeline4.pRootSignature = rootsignature.Get();

	gpipeline5.pRootSignature = rootsignature.Get();

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

	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

	//パイプラインステート２の生成

	result = dev->CreateGraphicsPipelineState(&gpipeline2, IID_PPV_ARGS(&pipelinestate2));

	result = dev->CreateGraphicsPipelineState(&gpipeline3, IID_PPV_ARGS(&pipelinestate3));

	result = dev->CreateGraphicsPipelineState(&gpipeline4, IID_PPV_ARGS(&pipelinestate4));

	result = dev->CreateGraphicsPipelineState(&gpipeline5, IID_PPV_ARGS(&pipelinestate5));

#pragma endregion

}

void Sprite::constantBuffGeneration(ID3D12Device* dev)
{
#pragma region 定数バッファ

	//定数バッファの生成用の設定

	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用


	cbResourceDesc = constBuffResourceGeneration(sizeof(ConstBufferMaterial));

	result = dev->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));


	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);//マッピング
	assert(SUCCEEDED(result));

	

	constMapMaterial->color = { 1.0f,1.0f,1.0f,1.0f };

#pragma endregion
}

D3D12_RESOURCE_DESC Sprite::constBuffResourceGeneration(int size)
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

void Sprite::setColor(Float4 color)
{

	constMapMaterial->color = {color.x,color.y,color.z,color.w };

}

void Sprite::descriptorRangeGeneration()
{
#pragma region デスクリプタレンジの設定
	descriptorRange.NumDescriptors = 1;//一度の描画に使うテクスチャが１枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;//テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

#pragma endregion
}

void Sprite::textureSamplerGeneration()
{
	//テクスチャサンプラーの設定
#pragma region 

	//テクスチャサンプラーの設定
	sampleDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//横繰り返し(タイリング)
	sampleDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//縦繰り返し(タイリング)
	sampleDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//奥行繰り返し(タイリング)
	sampleDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//ボーダーの時は黒
	sampleDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//全てリニア補間
	sampleDesc.MaxLOD = D3D12_FLOAT32_MAX;//ミップマップ最大値
	sampleDesc.MinLOD = 0.0f;//ミップマップ最小値
	sampleDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampleDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//ピクセルシェーダからのみ使用可能

#pragma endregion
}

XMFLOAT3 Sprite::Afin(XMFLOAT3 box, float moveX, float moveY, float rotate, float scaleX, float scaleY)
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
		{cosf(rotate * (PI / 180)),-sinf(rotate * (PI / 180)), 0.0f},
		{sinf(rotate * (PI / 180)),cosf(rotate * (PI / 180)), 0.0f},
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


	ansBuff2[0] = scaleA[0][0] * ans[0] + scaleA[0][1] * ans[1] + scaleA[0][2] * ans[2];
	ansBuff2[1] = scaleA[1][0] * ans[0] + scaleA[1][1] * ans[1] + scaleA[1][2] * ans[2];
	ansBuff2[2] = scaleA[2][0] * ans[0] + scaleA[2][1] * ans[1] + scaleA[2][2] * ans[2];

	ansBuff3[0] = rotateA[0][0] * ansBuff2[0] + rotateA[0][1] * ansBuff2[1] + rotateA[0][2] * ansBuff2[2];
	ansBuff3[1] = rotateA[1][0] * ansBuff2[0] + rotateA[1][1] * ansBuff2[1] + rotateA[1][2] * ansBuff2[2];
	ansBuff3[2] = rotateA[2][0] * ansBuff2[0] + rotateA[2][1] * ansBuff2[1] + rotateA[2][2] * ansBuff2[2];

	ansBuff4[0] = moveA[0][0] * ansBuff3[0] + moveA[0][1] * ansBuff3[1] + moveA[0][2] * ansBuff3[2];
	ansBuff4[1] = moveA[1][0] * ansBuff3[0] + moveA[1][1] * ansBuff3[1] + moveA[1][2] * ansBuff3[2];
	ansBuff4[2] = moveA[2][0] * ansBuff3[0] + moveA[2][1] * ansBuff3[1] + moveA[2][2] * ansBuff3[2];

	box.x = ansBuff4[0];
	box.y = ansBuff4[1];

	return box;

}

void Sprite::ObjAfin(float moveX, float moveY, float rotate, float scaleX, float scaleY)
{
#pragma region アフィン変換

	//原点は最後に変換する(元座標が変わってしまうため)
	vertices[0].pos = Afin(vertices[0].pos, moveX, moveY, rotate, scaleX, scaleY);
	vertices[1].pos = Afin(vertices[1].pos, moveX, moveY, rotate, scaleX, scaleY);
	vertices[2].pos = Afin(vertices[2].pos, moveX, moveY, rotate, scaleX, scaleY);
	vertices[3].pos = Afin(vertices[3].pos, moveX, moveY, rotate, scaleX, scaleY);
	vertices[4].pos = Afin(vertices[4].pos, moveX, moveY, rotate, scaleX, scaleY);
	vertices[5].pos = Afin(vertices[5].pos, moveX, moveY, rotate, scaleX, scaleY);
	

#pragma endregion

#pragma region アフィン変換後の頂点データをシェーダに転送

	vertexMap();

#pragma endregion
}

void Sprite::vertexMap()
{

	vertBuff->Map(0, nullptr, (void**)&vertMap);
	//全頂点に対して
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];//座標をコピー
	}

	//つながりを解除
	vertBuff->Unmap(0, nullptr);
}