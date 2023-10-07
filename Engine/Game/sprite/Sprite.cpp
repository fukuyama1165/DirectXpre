#include "Sprite.h"
#include "PI.h"
#include "DirectXInit.h"

Texture* Sprite::STexture_ = nullptr;

//const float PI = 3.141592653589f;


Sprite::Sprite()
{

	SpriteCommon::GetInstance()->initialize();

}

void Sprite::initialize(std::string textureIndex)
{
	STexture_ = Texture::GetInstance();
	STexture_->Init();
	common_ = SpriteCommon::GetInstance();

	
	AdjustTexSize(textureIndex);
	

	texIndex = textureIndex;
	Vertex vert = {};

	Vector2 textureDescSize = {
		static_cast<float>(STexture_->GetTexBuff(texIndex)->GetDesc().Width),
		static_cast<float>(STexture_->GetTexBuff(texIndex)->GetDesc().Height)
	};

	float texLeft = srcTexturePos_.x / textureDescSize.x;
	float texRight = (srcTexturePos_.x + textureSize_.x) / textureDescSize.x;
	float texTop = srcTexturePos_.y / textureDescSize.y;
	float texBottom = (srcTexturePos_.y + textureSize_.y) / textureDescSize.y;

	vert = { {-anchorPoint_.x * textureSize_.x, -anchorPoint_.y * textureSize_.y,0.0f},{ texLeft, texTop } };
	vertices_.emplace_back(vert);
	vert = { {-anchorPoint_.x * textureSize_.x,(1 - anchorPoint_.y) * textureSize_.y,0.0f},{ texLeft,texBottom } };
	vertices_.emplace_back(vert);
	vert = { {(1 - anchorPoint_.y) * textureSize_.x, -anchorPoint_.y * textureSize_.y,0.0f},{ texRight,texTop } };
	vertices_.emplace_back(vert);					  		
	vert = { {(1 - anchorPoint_.y) * textureSize_.x, (1 - anchorPoint_.y) * textureSize_.y,0.0f},{ texRight,texBottom } };
	vertices_.emplace_back(vert);

	indices_.emplace_back(uint16_t(0));
	indices_.emplace_back(uint16_t(1));
	indices_.emplace_back(uint16_t(2));
	indices_.emplace_back(uint16_t(2));
	indices_.emplace_back(uint16_t(1));
	indices_.emplace_back(uint16_t(3));

	sizeVB_ = static_cast<uint32_t>(sizeof(vertices_[0]) * vertices_.size());
	//インデックスデータ全体のサイズ
	sizeIB_ = static_cast<uint32_t>(sizeof(uint16_t) * indices_.size());

	vertexBuffGeneration();

	vertexLayout();

	graphicPipelineGeneration();

	descriptorRangeGeneration();

	textureSamplerGeneration();

	rootParamGeneration();

	rootsignatureGeneration();

	constantBuffGeneration();

	indicesBuffGeneration();
	

}

void Sprite::vertexBuffGeneration()
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
	for(uint16_t i = 0; i < vertices_.size(); i++)
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

void Sprite::Update()
{

	//情報取得
	if (changeFlag)
	{
		Vector2 textureDescSize = {
		static_cast<float>(STexture_->GetTexBuff(texIndex)->GetDesc().Width),
		static_cast<float>(STexture_->GetTexBuff(texIndex)->GetDesc().Height)
		};

		float texLeft = srcTexturePos_.x / textureDescSize.x;
		float texRight = (srcTexturePos_.x + textureSize_.x) / textureDescSize.x;
		float texTop = srcTexturePos_.y / textureDescSize.y;
		float texBottom = (srcTexturePos_.y + textureSize_.y) / textureDescSize.y;

		vertices_[0].pos = { -anchorPoint_.x * textureSize_.x, -anchorPoint_.y * textureSize_.y,0.0f };
		vertices_[1].pos = { -anchorPoint_.x * textureSize_.x,(1 - anchorPoint_.y) * textureSize_.y,0.0f };
		vertices_[2].pos = { (1 - anchorPoint_.y) * textureSize_.x, -anchorPoint_.y * textureSize_.y,0.0f };
		vertices_[3].pos = { (1 - anchorPoint_.y) * textureSize_.x, (1 - anchorPoint_.y) * textureSize_.y,0.0f };

		//頂点のuvに反映する
		vertices_[0].uv = { texLeft,texTop };
		vertices_[1].uv = { texLeft,texBottom };
		vertices_[2].uv = { texRight,texTop };
		vertices_[3].uv = { texRight,texBottom };

		vertexMap();
		changeFlag = false;
	}

	matWorldGeneration();

	constMapTransform_->mat = matWorld_ * matProjection_;

}

void Sprite::Draw(std::string ChangeTexure, uint16_t PipeLineRuleFlag)
{
	
	if (texIndex == "" && ChangeTexure == "")
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

	//インデックスバッファの設定
	DirectXInit::GetInstance()->GetcmdList()->IASetIndexBuffer(&ibView_);

	//定数バッファビュー(CBV)の設定コマンド
	DirectXInit::GetInstance()->GetcmdList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());
	DirectXInit::GetInstance()->GetcmdList()->SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());

	if (texIndex != "" && ChangeTexure == "")
	{
		STexture_->Draw(texIndex);
	}
	else
	{
		STexture_->Draw(ChangeTexure);
	}

	DirectXInit::GetInstance()->GetcmdList().Get()->DrawIndexedInstanced((uint32_t)indices_.size(), 1, 0, 0, 0);

}

void Sprite::SetTextureRect(int32_t srcX, int32_t srcY, int32_t width, int32_t height)
{

	srcTexturePos_ = { static_cast<float>(srcX),static_cast<float>(srcY) };
	textureSize_ = { static_cast<float>(width),static_cast<float>(height) };
	changeFlag = true;

}

void Sprite::ChangeTexture(std::string textureIndex)
{
	AdjustTexSize(textureIndex);

	texIndex = textureIndex;

	changeFlag = true;
}

void Sprite::vertexLayout()
{
#pragma region 頂点レイアウトの設定

	//頂点レイアウト
	//グラフィックパイプラインで頂点一つ分のデータに何を持たせるか決める
	//これは最低限の3D座標だけ持たせているらしい
	D3D12_INPUT_ELEMENT_DESC newInputLayout;
	newInputLayout =
	{
		"POSITION",//セマンティック名
		0,//同じセマンティック名が複数あるときのインデックス
		DXGI_FORMAT_R32G32B32_FLOAT,//要素数とビット数を返す(XYZの３つでfloat型なのでR32G32B32_FLOAT)
		0,//入力スロットインデックス
		D3D12_APPEND_ALIGNED_ELEMENT,//データのオフセット値(D3D12_APPEND_ALIGNED_ELEMENTだと自動設定)
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,//入力データ識別(標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
		0,//一度に描画するインスタンス数
	};
	inputLayout_.push_back(newInputLayout);
	//座標以外に　色、テクスチャUVなどを渡す場合はさらに続ける
	newInputLayout =
	{
		"TEXCOORD",//セマンティック名
		0,//同じセマンティック名が複数あるときのインデックス
		DXGI_FORMAT_R32G32_FLOAT,//要素数とビット数を返す(XYの2つでfloat型なのでR32G32_FLOAT)
		0,//入力スロットインデックス
		D3D12_APPEND_ALIGNED_ELEMENT,//データのオフセット値(D3D12_APPEND_ALIGNED_ELEMENTだと自動設定)
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,//入力データ識別(標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
		0,//一度に描画するインスタンス数
	};
	inputLayout_.push_back(newInputLayout);


#pragma endregion
}

void Sprite::graphicPipelineGeneration()
{
#pragma region グラフィックスパイプライン設定

	//グラフィックスパイプライン辺

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline_.VS.pShaderBytecode = common_->getVsBlob()->GetBufferPointer();
	gpipeline_.VS.BytecodeLength = common_->getVsBlob()->GetBufferSize();

	/*gpipeline.GS.pShaderBytecode = common->getGsBlob()->GetBufferPointer();
	gpipeline.GS.BytecodeLength = common->getGsBlob()->GetBufferSize();*/

	gpipeline_.PS.pShaderBytecode = common_->getPsBlob()->GetBufferPointer();
	gpipeline_.PS.BytecodeLength = common_->getPsBlob()->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	gpipeline_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	gpipeline_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	//gpipeline.RasterizerState.DepthClipEnable = true;//深度クリッピングを有効に


	//ブレンドステートの設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline_.BlendState.RenderTarget[0];
	gpipeline_.BlendState.RenderTarget[1] = blenddesc;
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
	gpipeline_.InputLayout.pInputElementDescs = inputLayout_.data();
	gpipeline_.InputLayout.NumElements = (uint32_t)inputLayout_.size();

	//図形の形状を三角形に設定
	gpipeline_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline_.NumRenderTargets = 2;//描画対象は2つ
	gpipeline_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0～255指定のRGBA
	gpipeline_.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0～255指定のRGBA
	gpipeline_.SampleDesc.Count = 1;//１ピクセルにつき１回サンプリング

#pragma endregion 通常描画ポリゴン内を塗りつぶし(三角形)

	//通常描画ワイヤーフレーム描画(三角形)
#pragma region グラフィックスパイプライン２の設定

//グラフィックスパイプライン辺


		//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline2_.VS.pShaderBytecode = common_->getVsBlob()->GetBufferPointer();
	gpipeline2_.VS.BytecodeLength = common_->getVsBlob()->GetBufferSize();

	/*gpipeline2.GS.pShaderBytecode = common->getGsBlob()->GetBufferPointer();
	gpipeline2.GS.BytecodeLength = common->getGsBlob()->GetBufferSize();*/

	gpipeline2_.PS.pShaderBytecode = common_->getPsBlob()->GetBufferPointer();
	gpipeline2_.PS.BytecodeLength = common_->getPsBlob()->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline2_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	gpipeline2_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	gpipeline2_.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;//ワイヤーフレーム描画
	//gpipeline2.RasterizerState.DepthClipEnable = true;//深度クリッピングを有効に

	//ブレンドステートの設定
	//gpipeline2.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc2 = gpipeline2_.BlendState.RenderTarget[0];
	gpipeline2_.BlendState.RenderTarget[1] = blenddesc;
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
	gpipeline2_.InputLayout.pInputElementDescs = inputLayout_.data();
	gpipeline2_.InputLayout.NumElements = (uint32_t)inputLayout_.size();

	//図形の形状を三角形に設定
	gpipeline2_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline2_.NumRenderTargets = 2;//描画対象は１つ
	gpipeline2_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0～255指定のRGBA
	gpipeline2_.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0～255指定のRGBA
	gpipeline2_.SampleDesc.Count = 1;//１ピクセルにつき１回サンプリング

#pragma endregion 

	//通常描画ポリゴン内を塗りつぶし(三角形)加算合成
#pragma region グラフィックスパイプライン3設定

//グラフィックスパイプライン辺

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline3_.VS.pShaderBytecode = common_->getVsBlob()->GetBufferPointer();
	gpipeline3_.VS.BytecodeLength = common_->getVsBlob()->GetBufferSize();

	/*gpipeline3.GS.pShaderBytecode = common->getGsBlob()->GetBufferPointer();
	gpipeline3.GS.BytecodeLength = common->getGsBlob()->GetBufferSize();*/

	gpipeline3_.PS.pShaderBytecode = common_->getPsBlob()->GetBufferPointer();
	gpipeline3_.PS.BytecodeLength = common_->getPsBlob()->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline3_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	gpipeline3_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	gpipeline3_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	//gpipeline3.RasterizerState.DepthClipEnable = true;//深度クリッピングを有効に


	//ブレンドステートの設定
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc3 = gpipeline3_.BlendState.RenderTarget[0];
	gpipeline3_.BlendState.RenderTarget[1] = blenddesc;
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
	gpipeline3_.InputLayout.pInputElementDescs = inputLayout_.data();
	gpipeline3_.InputLayout.NumElements = (uint32_t)inputLayout_.size();

	//図形の形状を三角形に設定
	gpipeline3_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline3_.NumRenderTargets = 2;//描画対象は2つ
	gpipeline3_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0～255指定のRGBA
	gpipeline3_.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0～255指定のRGBA
	gpipeline3_.SampleDesc.Count = 1;//１ピクセルにつき１回サンプリング

#pragma endregion 

#pragma region グラフィックスパイプライン4設定

//グラフィックスパイプライン辺

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline4_.VS.pShaderBytecode = common_->getVsBlob()->GetBufferPointer();
	gpipeline4_.VS.BytecodeLength = common_->getVsBlob()->GetBufferSize();

	/*gpipeline4.GS.pShaderBytecode = common->getGsBlob()->GetBufferPointer();
	gpipeline4.GS.BytecodeLength = common->getGsBlob()->GetBufferSize();*/

	gpipeline4_.PS.pShaderBytecode = common_->getPsBlob()->GetBufferPointer();
	gpipeline4_.PS.BytecodeLength = common_->getPsBlob()->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline4_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	gpipeline4_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	gpipeline4_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	//gpipeline4.RasterizerState.DepthClipEnable = true;//深度クリッピングを有効に


	//ブレンドステートの設定
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc4 = gpipeline4_.BlendState.RenderTarget[0];
	gpipeline4_.BlendState.RenderTarget[1] = blenddesc;
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
	gpipeline4_.InputLayout.pInputElementDescs = inputLayout_.data();
	gpipeline4_.InputLayout.NumElements = (uint32_t)inputLayout_.size();

	//図形の形状を三角形に設定
	gpipeline4_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline4_.NumRenderTargets = 2;//描画対象は2つ
	gpipeline4_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0～255指定のRGBA
	gpipeline4_.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0～255指定のRGBA
	gpipeline4_.SampleDesc.Count = 1;//１ピクセルにつき１回サンプリング

#pragma endregion 通常描画ポリゴン内を塗りつぶし(三角形)減算合成

#pragma region グラフィックスパイプライン5設定

//グラフィックスパイプライン辺

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline5_.VS.pShaderBytecode = common_->getVsBlob()->GetBufferPointer();
	gpipeline5_.VS.BytecodeLength = common_->getVsBlob()->GetBufferSize();

	/*gpipeline5.GS.pShaderBytecode = common->getGsBlob()->GetBufferPointer();
	gpipeline5.GS.BytecodeLength = common->getGsBlob()->GetBufferSize();*/

	gpipeline5_.PS.pShaderBytecode = common_->getPsBlob()->GetBufferPointer();
	gpipeline5_.PS.BytecodeLength = common_->getPsBlob()->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline5_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	gpipeline5_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	gpipeline5_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	//gpipeline5.RasterizerState.DepthClipEnable = true;//深度クリッピングを有効に


	//ブレンドステートの設定
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc5 = gpipeline5_.BlendState.RenderTarget[0];
	gpipeline5_.BlendState.RenderTarget[1] = blenddesc;
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
	gpipeline5_.InputLayout.pInputElementDescs = inputLayout_.data();
	gpipeline5_.InputLayout.NumElements = (uint32_t)inputLayout_.size();

	//図形の形状を三角形に設定
	gpipeline5_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline5_.NumRenderTargets = 2;//描画対象は2つ
	gpipeline5_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0～255指定のRGBA
	gpipeline5_.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0～255指定のRGBA
	gpipeline5_.SampleDesc.Count = 1;//１ピクセルにつき１回サンプリング

#pragma endregion 通常描画ポリゴン内を塗りつぶし(三角形)色反転

}

void Sprite::rootParamGeneration()
{
	//定数バッファを増やしたら増やすところがある
#pragma region ルートパラメータ

	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER newRootParam{};
	//色
	newRootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//定数バッファビュー
	newRootParam.Descriptor.ShaderRegister = 0;//定数バッファ番号
	newRootParam.Descriptor.RegisterSpace = 0;//デフォルト値
	newRootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える

	rootParam_.push_back(newRootParam);

	//画像データ用
	newRootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//種類
	newRootParam.DescriptorTable.pDescriptorRanges = &descriptorRange_;//デスクリプタレンジ
	newRootParam.DescriptorTable.NumDescriptorRanges = 1;//デスクリプタレンジ数
	newRootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダから見える

	rootParam_.push_back(newRootParam);

	//色
	newRootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//定数バッファビュー
	newRootParam.Descriptor.ShaderRegister = 1;//定数バッファ番号
	newRootParam.Descriptor.RegisterSpace = 0;//デフォルト値
	newRootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える

	rootParam_.push_back(newRootParam);

#pragma endregion 
}

void Sprite::rootsignatureGeneration()
{
	//定数バッファを増やしたらルートパラメータを書き換えパラメータ数を書き換える
#pragma region ルートシグネチャ設定

	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParam_.data();
	rootSignatureDesc.NumParameters = (uint32_t)rootParam_.size();//ルートパラメータ数
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

void Sprite::constantBuffGeneration()
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
	constMapTransform_->mat*= matProjection_;

#pragma endregion
}

D3D12_RESOURCE_DESC Sprite::constBuffResourceGeneration(uint32_t size)
{
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (uint64_t)((size + 0xff) & ~0xff);//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	return cbResourceDesc;
}

void Sprite::setColor(const Vector4& color)
{

	constMapMaterial_->color = {color.x,color.y,color.z,color.w };

}

void Sprite::descriptorRangeGeneration()
{
#pragma region デスクリプタレンジの設定
	descriptorRange_.NumDescriptors = 1;//一度の描画に使うテクスチャが１枚なので1
	descriptorRange_.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange_.BaseShaderRegister = 0;//テクスチャレジスタ0番
	descriptorRange_.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

#pragma endregion
}

void Sprite::textureSamplerGeneration()
{
	//テクスチャサンプラーの設定
#pragma region 

	//テクスチャサンプラーの設定
	sampleDesc_.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//横繰り返し(タイリング)
	sampleDesc_.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//縦繰り返し(タイリング)
	sampleDesc_.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//奥行繰り返し(タイリング)
	sampleDesc_.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//ボーダーの時は黒
	sampleDesc_.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//全てリニア補間
	sampleDesc_.MaxLOD = D3D12_FLOAT32_MAX;//ミップマップ最大値
	sampleDesc_.MinLOD = 0.0f;//ミップマップ最小値
	sampleDesc_.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampleDesc_.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//ピクセルシェーダからのみ使用可能

#pragma endregion
}



void Sprite::vertexMap()
{

	vertBuff_->Map(0, nullptr, (void**)&vertMap_);
	//全頂点に対して
	for (uint16_t i = 0; i < vertices_.size(); i++)
	{
		vertMap_[i] = vertices_[i];//座標をコピー
	}

	//つながりを解除
	vertBuff_->Unmap(0, nullptr);
}

void Sprite::AdjustTexSize(std::string textureIndex)
{

	//情報取得
	textureSize_.x = static_cast<float>(STexture_->GetTexBuff(textureIndex)->GetDesc().Width);
	textureSize_.y = static_cast<float>(STexture_->GetTexBuff(textureIndex)->GetDesc().Height);

}

Matrix4x4 Sprite::matScaleGeneration(const Vector3& scale)
{
	//スケーリング行列を宣言
	Matrix4x4 matScale;
	matScale.IdentityMatrix();

	//スケーリング倍率を行列に設定
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1;

	return matScale;
}

Matrix4x4 Sprite::matRotateXGeneration(float rotateX)
{
	//X軸回転行列を宣言
	Matrix4x4 matRotateX;
	matRotateX.IdentityMatrix();

	//回転角を行列に設定(ラジアン)
	matRotateX.m[0][0] = 1;
	matRotateX.m[1][1] = cosf(rotateX);
	matRotateX.m[1][2] = sinf(rotateX);
	matRotateX.m[2][1] = -sinf(rotateX);
	matRotateX.m[2][2] = cosf(rotateX);
	matRotateX.m[3][3] = 1;

	return matRotateX;
}

Matrix4x4 Sprite::matRotateYGeneration(float rotateY)
{
	//Y軸回転行列を宣言
	Matrix4x4 matRotateY;
	matRotateY.IdentityMatrix();

	//回転角を行列に設定(ラジアン)
	matRotateY.m[0][0] = cosf(rotateY);
	matRotateY.m[0][2] = -sinf(rotateY);
	matRotateY.m[1][1] = 1;
	matRotateY.m[2][0] = sinf(rotateY);
	matRotateY.m[2][2] = cosf(rotateY);
	matRotateY.m[3][3] = 1;

	return matRotateY;
}

Matrix4x4 Sprite::matRotateZGeneration(float rotateZ)
{
	//Z軸回転行列を宣言
	Matrix4x4 matRotateZ;
	matRotateZ.IdentityMatrix();

	//回転角を行列に設定(ラジアン)
	matRotateZ.m[0][0] = cosf(rotateZ);
	matRotateZ.m[0][1] = sinf(rotateZ);
	matRotateZ.m[1][0] = -sinf(rotateZ);
	matRotateZ.m[1][1] = cosf(rotateZ);
	matRotateZ.m[2][2] = 1;
	matRotateZ.m[3][3] = 1;

	return matRotateZ;
}

Matrix4x4 Sprite::matRotateGeneration(const Vector3& rotate)
{
	//X軸回転行列を宣言
	Matrix4x4 matRotateX = matRotateXGeneration(rotate.x);

	//Y軸回転行列を宣言
	Matrix4x4 matRotateY = matRotateYGeneration(rotate.y);

	//Z軸回転行列を宣言
	Matrix4x4 matRotateZ = matRotateZGeneration(rotate.z);

	//回転軸合成行列を宣言
	Matrix4x4 matRotate;
	matRotate.IdentityMatrix();

	//計算した角度を計算(順番は回転させるモデルによって変える)


	matRotate = matRotateZ * matRotateX * matRotateY;

	return matRotate;



}

Matrix4x4 Sprite::matMoveGeneration(const Vector3& translation)
{
	//移動するための行列を用意
	Matrix4x4 matMove;
	matMove.IdentityMatrix();

	//行列に移動量を代入
	matMove.m[3][0] = translation.x;
	matMove.m[3][1] = translation.y;
	matMove.m[3][2] = translation.z;

	return matMove;
}

void Sprite::matWorldGeneration()
{
	//スケール行列更新
	matScale_ = matScaleGeneration({scale_.x,scale_.y,1});

	//回転行列更新
	matRotate_ = matRotateGeneration({0,0,rotate_ });

	//平行移動行列更新
	matTrans_ = matMoveGeneration({pos_.x,pos_.y,0});

	//ワールド行列更新
	matWorld_.IdentityMatrix();
	matWorld_ = matScale_ * matRotate_ * matTrans_;



}

void Sprite::indicesBuffGeneration()
{
#pragma region インデックスバッファ

	//リソース設定
	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeIB_;//インデックス情報が入る分のサイズ
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//インデックスバッファの生成

	result_ = DirectXInit::GetInstance()->Getdev()->CreateCommittedResource(
		&heapprop_,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc_,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_)
	);

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result_ = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対して
	for (uint16_t i = 0; i < indices_.size(); i++)
	{
		indexMap[i] = indices_[i];//インデックスをコピー
	}

	//マッピング解除
	indexBuff_->Unmap(0, nullptr);

#pragma endregion 頂点を通る順番を決める場所

#pragma region インデックスバッファビュー

	//インデックスバッファビューの生成

	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB_;

#pragma endregion

}
