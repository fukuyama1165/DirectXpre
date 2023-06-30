

void Object3D::graphicPipelineGeneration()
{
#pragma region グラフィックスパイプライン設定

	//グラフィックスパイプライン辺

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
	gpipeline_.VS.BytecodeLength = vsBlob_->GetBufferSize();
	gpipeline_.PS.pShaderBytecode = psBlob_->GetBufferPointer();
	gpipeline_.PS.BytecodeLength = psBlob_->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	gpipeline_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	gpipeline_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	gpipeline_.RasterizerState.DepthClipEnable = true;//深度クリッピングを有効に


	//ブレンドステートの設定
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline_.BlendState.RenderTarget[0];
	gpipeline_.BlendState.RenderTarget[1] = blenddesc;
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region ブレンドステートの共通設定(アルファ値)

	blenddesc.BlendEnable = true;//ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//デストの値を0%使う

#pragma endregion


#pragma region 加算合成

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

#pragma region 半透明合成

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