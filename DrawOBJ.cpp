#include "DrawOBJ.h"



using namespace Microsoft::WRL;

const float PI = 3.141592653589f;

Texture* DrawOBJ::texture = nullptr;


DrawOBJ::DrawOBJ(const float windowWidth, const float windowHeight)
{
	Win_width = windowWidth;
	Win_height = windowHeight;

	texture = Texture::GetInstance();

	//頂点データ(四点分の座標)
	//  x     y    z   法線  u    v
	


	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	sizeVB = static_cast<UINT>(sizeof(vertices) * vertices.size());
	//インデックスデータ全体のサイズ
	sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());

	//ビュー変換行列
	eye_ = { 0, 0, -400.0f };//視点座標
	target_ = { 0, 0, 0 };//注視点座標
	up_ = { 0, 1, 0 };//上方向ベクトル
}


DrawOBJ::~DrawOBJ()
{

}

void DrawOBJ::basicInit(ID3D12Device* dev)
{
	vertexBuffGeneration(dev);

	vertexShaderGeneration();

	pixelShaderGeneration();

	vertexLayout();

	graphicPipelineGeneration();

	descriptorRangeGeneration();

	rootParamGeneration();

	textureSamplerGeneration();

	rootsignatureGeneration(dev);

	constantBuffGeneration(dev);


	indicesBuffGeneration(dev);

	/*imageDataGeneration();

	textureBuffGeneraion(dev);*/

	texture->Init(dev);

	//SRVGeneraion(dev);

}

void DrawOBJ::colorChangeInit(ID3D12Device* dev)
{
	vertexBuffGeneration(dev);

	vertexShaderGeneration2();

	pixelShaderGeneration2();

	vertexLayout();

	graphicPipelineGeneration();

	descriptorRangeGeneration();

	rootParamGeneration();

	textureSamplerGeneration();

	rootsignatureGeneration(dev);

	constantBuffGeneration(dev);

	indicesBuffGeneration(dev);

	/*imageDataGeneration();

	textureBuffGeneraion(dev);*/

	texture->Init(dev);

	//SRVGeneraion(dev);

}

void DrawOBJ::deleteTexture()
{
	if (texture != nullptr)
	{
		texture->instanceDelete();
	}
}

void DrawOBJ::vertexBuffGeneration(ID3D12Device* dev)
{

#pragma region OBj読み込み

	std::vector<XMFLOAT3> positions;
	std::vector<XMFLOAT3> normals;
	std::vector<XMFLOAT2> texcoords;

	//ファイルストリーム
	std::ifstream file;

	//0bjファイルを開く
	file.open("Resources/obj/triangle_tex.obj");

	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	std::string line;
	while (std::getline(file,line))
	{

		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream lineStream(line);

		//半角スペース区切りで行の先頭文字列を取得
		std::string key;
		std::getline(lineStream, key, ' ');

		//先頭文字列がvなら頂点座標
		if (key == "v")
		{

			//x,y,z座標読み込み
			XMFLOAT3 position{};
			lineStream >> position.x;
			lineStream >> position.y;
			lineStream >> position.z;

			//座標データに追加
			positions.emplace_back(position);


		}

		//先頭文字列がvtならテクスチャ
		if (key == "vt")
		{

			//U,V成分読み込み
			XMFLOAT2 texcoord{};
			lineStream >> texcoord.x;
			lineStream >> texcoord.y;

			//V方向反転
			texcoord.y = 1.0f - texcoord.y;

			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);

		}

		//先頭文字列がvnなら法線ベクトル
		if (key == "vn")
		{

			//x,y,z座標読み込み
			XMFLOAT3 normal{};
			lineStream >> normal.x;
			lineStream >> normal.y;
			lineStream >> normal.z;

			//座標データに追加
			normals.emplace_back(normal);


		}

		//先頭文字列がfならポリゴン(三角形)
		if (key == "f")
		{

			//半角スペース区切りで行の続きを読み込む
			std::string indexString;

			while (std::getline(lineStream, indexString, ' '))
			{

				//頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream indexStream(indexString);

				unsigned short indexPosition;
				unsigned short indexNormal;
				unsigned short indexTexcoord;
				indexStream >> indexPosition;

				//スラッシュを飛ばす
				indexStream.seekg(1, std::ios_base::cur);
				indexStream >> indexTexcoord;

				//スラッシュを飛ばす
				indexStream.seekg(1, std::ios_base::cur);
				indexStream >> indexNormal;

				//頂点データの追加
				Vertex vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);

				indices.emplace_back((unsigned short)indices.size());

			}

		}

	}

	file.close();

	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	sizeVB = static_cast<UINT>(sizeof(vertices) * vertices.size());
	//インデックスデータ全体のサイズ
	sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());

#pragma endregion


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

#pragma endregion

#pragma region 法線ベクトル計算

	for (int i = 0; i < indices.size() / 3; i++)
	{//三角形1つごとに計算していく

		//三角形のインデックスを取り出して,一時的な変数に入れる
		unsigned short indices0 = indices[i * 3 + 0];
		unsigned short indices1 = indices[i * 3 + 1];
		unsigned short indices2 = indices[i * 3 + 2];

		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[indices0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[indices1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[indices2].pos);

		//p0→p1ベクトル、p0→p2ベクトルを計算(ベクトルの減算)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		//外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);

		//正規化(長さを1にする)
		normal = XMVector3Normalize(normal);

		//求めた法線を頂点データに代入
		XMStoreFloat3(&vertices[indices0].normal, normal);
		XMStoreFloat3(&vertices[indices1].normal, normal);
		XMStoreFloat3(&vertices[indices2].normal, normal);

	}

#pragma endregion


#pragma region 頂点バッファへのデータ転送

	//頂点バッファへのデータ転送
	//GPU上のバッファに対応した仮想メモリを取得
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}
	
	//std::copy(indices.begin(),indices.end(),indexmap)


	//つながりを解除
	//vertBuff->Unmap(0, nullptr);

#pragma endregion

#pragma region 頂点バッファビューの生成

	//頂点バッファビューの作成(GPUへの道しるべ)辺

	//頂点バッファビューの作成

	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices);

#pragma endregion

}

void DrawOBJ::vertexShaderGeneration()
{
#pragma region 頂点シェーダファイルの読み込みとコンパイル

	//頂点シェーダファイルの読み込み辺
	//頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicVS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main",//エントリーポイント名
		"vs_5_0",//シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&vsBlob,
		&errorBlob
	);

#pragma endregion 

#pragma region 頂点シェーダの読み込み時のエラーを表示する場所

	//頂点シェーダの読み込み時のエラーを表示する場所
	if (FAILED(result))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容をウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void DrawOBJ::vertexShaderGeneration2()
{
#pragma region 頂点シェーダファイルの読み込みとコンパイル

	//頂点シェーダファイルの読み込み辺
	//頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/vertexMoveVS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main",//エントリーポイント名
		"vs_5_0",//シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&vsBlob,
		&errorBlob
	);

#pragma endregion 

#pragma region 頂点シェーダの読み込み時のエラーを表示する場所

	//頂点シェーダの読み込み時のエラーを表示する場所
	if (FAILED(result))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容をウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void DrawOBJ::pixelShaderGeneration()
{
#pragma region ピクセルシェーダの読み込みとコンパイル

	//ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicPS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main",//エントリーポイント名
		"ps_5_0",//シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&psBlob,
		&errorBlob
	);

#pragma endregion

#pragma region ピクセルシェーダの読み込み時のエラーを表示する場所

	//ピクセルシェーダの読み込み時のエラーを表示する場所
	if (FAILED(result))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容をウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void DrawOBJ::pixelShaderGeneration2()
{
#pragma region ピクセルシェーダの読み込みとコンパイル

	//ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/colorChangePS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main",//エントリーポイント名
		"ps_5_0",//シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&psBlob,
		&errorBlob
	);

#pragma endregion

#pragma region ピクセルシェーダの読み込み時のエラーを表示する場所

	//ピクセルシェーダの読み込み時のエラーを表示する場所
	if (FAILED(result))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容をウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void DrawOBJ::vertexLayout()
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
	{//法線ベクトル
		"NORMAL",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
	};

	inputLayout[2] =
	{//UV座標
		"TEXCOORD",
		0,
		DXGI_FORMAT_R32G32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
	};

#pragma endregion
}

void DrawOBJ::graphicPipelineGeneration()
{
#pragma region グラフィックスパイプライン設定

	//グラフィックスパイプライン辺

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	gpipeline.VS.BytecodeLength = vsBlob->GetBufferSize();
	gpipeline.PS.pShaderBytecode = psBlob->GetBufferPointer();
	gpipeline.PS.BytecodeLength = psBlob->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	gpipeline.RasterizerState.DepthClipEnable = true;//深度クリッピングを有効に


	//ブレンドステートの設定
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];
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
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状を三角形に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline.NumRenderTargets = 1;//描画対象は１つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1;//１ピクセルにつき１回サンプリング

#pragma endregion 通常描画ポリゴン内を塗りつぶし(三角形)

#pragma region グラフィックスパイプライン２の設定

//グラフィックスパイプライン辺


	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline2.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	gpipeline2.VS.BytecodeLength = vsBlob->GetBufferSize();
	gpipeline2.PS.pShaderBytecode = psBlob->GetBufferPointer();
	gpipeline2.PS.BytecodeLength = psBlob->GetBufferSize();

	//サンプルマスクとラスタライザステートの設定
	gpipeline2.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
	gpipeline2.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	gpipeline2.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;//ワイヤーフレーム描画
	gpipeline2.RasterizerState.DepthClipEnable = true;//深度クリッピングを有効に

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

#pragma region 半透明合成

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

#pragma endregion 通常描画ワイヤーフレーム描画(三角形)

}

void DrawOBJ::descriptorRangeGeneration()
{
#pragma region デスクリプタレンジの設定
	descriptorRange.NumDescriptors = 1;//一度の描画に使うテクスチャが１枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;//テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

#pragma endregion
}

void DrawOBJ::rootParamGeneration()
{
#pragma region ルートパラメータ

	//ルートパラメータの設定

	//色
	rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//定数バッファビュー
	rootParam[0].Descriptor.ShaderRegister = 0;//定数バッファ番号
	rootParam[0].Descriptor.RegisterSpace = 0;//デフォルト値
	rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える

	//位置
	rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//定数バッファビュー
	rootParam[1].Descriptor.ShaderRegister = 1;//定数バッファ番号
	rootParam[1].Descriptor.RegisterSpace = 0;//デフォルト値
	rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える

	//画像データ用
	rootParam[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//種類
	rootParam[2].DescriptorTable.pDescriptorRanges = &descriptorRange;//デスクリプタレンジ
	rootParam[2].DescriptorTable.NumDescriptorRanges = 1;//デスクリプタレンジ数
	rootParam[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダから見える

	rootParam[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//定数バッファビュー
	rootParam[3].Descriptor.ShaderRegister = 2;//定数バッファ番号
	rootParam[3].Descriptor.RegisterSpace = 0;//デフォルト値
	rootParam[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える

#pragma endregion 定数バッファを増やしたら増やすところがある
}

void DrawOBJ::textureSamplerGeneration()
{
#pragma region テクスチャサンプラーの設定

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

void DrawOBJ::rootsignatureGeneration(ID3D12Device* dev)
{
#pragma region ルートシグネチャ設定

	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParam;//ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParam);//ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &sampleDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	//ルートシグネチャの生成
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));


	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature.Get();

	//パイプライン2にもルートシグネチャをセット
	gpipeline2.pRootSignature = rootsignature.Get();

#pragma endregion 定数バッファを増やしたらルートパラメータを書き換えパラメータ数を書き換える

#pragma region デプスステンシルステート

	//デプスステンシルステートの設定
	gpipeline.DepthStencilState.DepthEnable = true;//深度テストを行う
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//小さければ合格
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット

	gpipeline2.DepthStencilState.DepthEnable = true;//深度テストを行う
	gpipeline2.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
	gpipeline2.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//小さければ合格
	gpipeline2.DSVFormat = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット



#pragma endregion


#pragma region パイプラインステートの生成

	//パイプラインステートの生成

	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

	//パイプラインステート２の生成

	result = dev->CreateGraphicsPipelineState(&gpipeline2, IID_PPV_ARGS(&pipelinestate2));

#pragma endregion

}

void DrawOBJ::constantBuffGeneration(ID3D12Device* dev)
{
#pragma region 定数バッファ

	//定数バッファの生成用の設定
	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用


	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};

	cbResourceDesc = constBuffResourceGeneration(sizeof(ConstBufferDataMaterial));

	//定数バッファの生成
	result = dev->CreateCommittedResource(
		&cbHeapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial)
	);
	assert(SUCCEEDED(result));

	cbResourceDesc = constBuffResourceGeneration(sizeof(ConstBufferDataMaterial2));

	//定数バッファの生成
	result = dev->CreateCommittedResource(
		&cbHeapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial2)
	);
	assert(SUCCEEDED(result));

	//cbResourceDesc = constBuffResourceGeneration(sizeof(ConstBufferDataTransform));

	////定数バッファの生成
	//result = dev->CreateCommittedResource(
	//	&cbHeapProp,//ヒープ設定
	//	D3D12_HEAP_FLAG_NONE,
	//	&cbResourceDesc,//リソース設定
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuffTransform0)
	//);
	//assert(SUCCEEDED(result));


	////定数バッファのマッピング
	//
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);//マッピング
	assert(SUCCEEDED(result));

	//マッピングをするとGPUのVRSMがCPUと連動する
	//Unmapをするとつながりが解除されるが定数バッファは書き換えることが多いので
	//しなくても大丈夫

	//値を書き込むと自動的に転送される



	constMapMaterial->color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);


	result = constBuffMaterial2->Map(0, nullptr, (void**)&constMapMaterial2);//マッピング
	assert(SUCCEEDED(result));

	constMapMaterial2->posM = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	//result = constBuffTransform0->Map(0, nullptr, (void**)&constMapTransform0);//マッピング
	//assert(SUCCEEDED(result));

	//行列に単位行列を代入
	/*matWorld.IdentityMatrix();

	matScale.IdentityMatrix();

	matRotate.IdentityMatrix();

	matTrans.IdentityMatrix();

	constMapTransform->mat.IdentityMatrix();*/

	/*constMapTransform->mat.r[0].m128_f32[0] = 2.0f / Win_width;
	constMapTransform->mat.r[1].m128_f32[1] = -2.0f / Win_height;
	constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
	constMapTransform->mat.r[3].m128_f32[1] = 1.0f;*/

	//平行投射行列の計算
	//constMapTransform->mat = XMMatrixOrthographicOffCenterLH(0.0f,Win_width,Win_height, 0.0f, 0.0f, 1.0f);

	//透視投影行列の計算

	matProjection = perspectiveProjectionGeneration((45.0f * (PI / 180)), 0.1f, 1000.0f);

	matView = matViewGeneration(eye_, target_, up_);



	//ビュー変換行列


	 /*Scale_ = { 1.0f,1.0f,1.0f };
	 Rotate_ = { 0.0f,0.0f,0.0f };
	 Trans_ = { 0.0f,0.0f,0.0f };

	 matWorldUpdata();

	 constTransformMatUpdata();*/

	for (int i = 0; i < _countof(object3Ds); i++)
	{
		//初期化
		object3Ds[i].Init(dev);

		if (i > 0)
		{
			//ひとつ前のオブジェクトを親オブジェクトとする
			object3Ds[i].SetParent(&object3Ds[i - 1]);

			//親オブジェクトの9割の大きさ
			object3Ds[i].SetScale({ 0.9f,0.9f,0.9f });

			//親オブジェクトに対してz軸周りに30度回転
			object3Ds[i].SetRotate({ 0.0f,0.0f,XMConvertToRadians(30.0f) });

			//親オブジェクトに対してz方向-8.0ずらす
			object3Ds[i].SetPos({ 0.0f,0.0f,-8.0f });

			object3Ds[i].Update();

		}

	}

	for (size_t i = 0; i < _countof(object3Ds); i++)
	{
		//object3Ds[i].Update(matView, matProjection);
	}

#pragma endregion
}

void DrawOBJ::constantBuffGeneration1(ID3D12Device* dev)
{
#pragma region 定数バッファ

	//定数バッファの生成用の設定
	//ヒープ設定
	//D3D12_HEAP_PROPERTIES cbHeapProp{};
	//cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用


	////リソース設定
	//D3D12_RESOURCE_DESC cbResourceDesc{};

	//cbResourceDesc = constBuffResourceGeneration(sizeof(ConstBufferDataTransform));

	////定数バッファの生成
	//result = dev->CreateCommittedResource(
	//	&cbHeapProp,//ヒープ設定
	//	D3D12_HEAP_FLAG_NONE,
	//	&cbResourceDesc,//リソース設定
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuffTransform1)
	//);
	//assert(SUCCEEDED(result));


	////定数バッファのマッピング

	//result = constBuffTransform1->Map(0, nullptr, (void**)&constMapTransform1);//マッピング
	//assert(SUCCEEDED(result));

	////行列に単位行列を代入
	///*matWorld.IdentityMatrix();

	//matScale.IdentityMatrix();

	//matRotate.IdentityMatrix();

	//matTrans.IdentityMatrix();

	//constMapTransform->mat.IdentityMatrix();*/

	///*constMapTransform->mat.r[0].m128_f32[0] = 2.0f / Win_width;
	//constMapTransform->mat.r[1].m128_f32[1] = -2.0f / Win_height;
	//constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
	//constMapTransform->mat.r[3].m128_f32[1] = 1.0f;*/

	////平行投射行列の計算
	//constMapTransform->mat = XMMatrixOrthographicOffCenterLH(0.0f,Win_width,Win_height, 0.0f, 0.0f, 1.0f);

	//matWorldUpdata1();

	//constTransformMatUpdata1();

#pragma endregion
}

D3D12_RESOURCE_DESC DrawOBJ::constBuffResourceGeneration(int size)
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

void DrawOBJ::indicesBuffGeneration(ID3D12Device* dev)
{
#pragma region インデックスバッファ

	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;//インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//インデックスバッファの生成

	result = dev->CreateCommittedResource(
		&heapprop,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対して
	for (int i = 0; i < indices.size(); i++)
	{
		indexMap[i] = indices[i];//インデックスをコピー
	}

	//マッピング解除
	indexBuff->Unmap(0, nullptr);

#pragma endregion 頂点を通る順番を決める場所

#pragma region インデックスバッファビュー

	//インデックスバッファビューの生成

	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

#pragma endregion

}

void DrawOBJ::imageDataGeneration()
{
#pragma region 画像イメージデータの作成

	//自作したイメージデータ
	////横方向ピクセル数
	//const size_t textureWidth = 256;
	////縦方向ピクセル数
	//const size_t textureHeight = 256;
	////配列の要素数
	//const size_t imageDataCount = textureWidth * textureHeight;
	////画像イメージデータ配列
	//XMFLOAT4* imageData = new XMFLOAT4[imageDataCount];//必ず後で解放する

	//for (size_t i = 0; i < imageDataCount; i++)
	//{
	//	imageData[i].x = 0.0f;//R
	//	imageData[i].y = 1.0f;//G
	//	imageData[i].z = 0.0f;//B
	//	imageData[i].w = 1.0f;//A

	//}

	//画像読み込みして画像イメージデータを生成


	//WICテクスチャのロード
	result = LoadFromWICFile(
		L"Resources/hokehoke.png",
		WIC_FLAGS_NONE,
		&metadata,
		scratchImg
	);

	ScratchImage mipChain{};
	//ミップマップの生成
	result = GenerateMipMaps(
		scratchImg.GetImages(),
		scratchImg.GetImageCount(),
		scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain
	);

	if (SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

#pragma endregion
}

void DrawOBJ::textureBuffGeneraion(ID3D12Device* dev)
{
#pragma region テクスチャバッファ設定

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;//幅
	textureResourceDesc.Height = (UINT)metadata.height;//高さ
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

#pragma endregion

#pragma region テクスチャバッファの生成

	//テクスチャバッファの生成

	result = dev->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);

	//ミップマップで置き換える
	//全ミップマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		//ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,//全領域へコピー
			img->pixels,//元データアドレス
			(UINT)img->rowPitch,//1ラインアドレス
			(UINT)img->slicePitch//1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

	//result = texBuff->WriteToSubresource(
	//	0,
	//	nullptr,//全領域へコピー
	//	imageData,//元データアドレス
	//	sizeof(XMFLOAT4) * textureWidth,//1ラインサイズ
	//	sizeof(XMFLOAT4) * imageDataCount//全サイズ
	//);

	//delete[] imageData;

#pragma endregion ここで画像イメージデータをdeleteしている(コメントアウト中)
}

void DrawOBJ::SRVGeneraion(ID3D12Device* dev)
{
#pragma region シェーダーリソースビューの為のデスクリプタヒープ生成

	//SRVの最大個数
	const size_t kMaxSRVCount = 2056;

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//設定をもとにSRV用デスクリプタヒープを生成

	//result = dev->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	//assert(SUCCEEDED(result));

	////デスクリプタハンドル(ヒープ内の操作する場所指定に使う)
	////SRVヒープの先頭ハンドルを取得
	//D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

#pragma endregion

#pragma region シェーダリソースビュー

	//シェーダリソースビュー
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	srvDesc.Format = resDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//ハンドルの指す位置にシェーダリソースビュー作成
	//dev->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);

#pragma endregion
}

void DrawOBJ::vertexMap()
{

	vertBuff->Map(0, nullptr, (void**)&vertMap);
	//全頂点に対して
	for (int i = 0; i < vertices.size(); i++)
	{
		vertMap[i] = vertices[i];//座標をコピー
	}

	//つながりを解除
	vertBuff->Unmap(0, nullptr);
}

void DrawOBJ::Draw(ID3D12GraphicsCommandList* cmdList, bool PipeLineRuleFlag, bool ChangeSquareFlag, bool ChangeTexure)
{

	//パイプラインステートとルートシグネチャの設定
	//作ったパイプラインステートとルートシグネチャをセットする
	//決めたルールで描画をお願いするところ
	//D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;

	if (PipeLineRuleFlag)
	{
		cmdList->SetPipelineState(pipelinestate.Get());
	}
	else
	{
		cmdList->SetPipelineState(pipelinestate2.Get());
	}

	cmdList->SetGraphicsRootSignature(rootsignature.Get());

	//頂点バッファビューの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);



	//定数バッファビュー(CBV)の設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffMaterial2->GetGPUVirtualAddress());


	//SRVヒープの設定コマンド
	//cmdList->SetDescriptorHeaps(1, &texture.srvHeap);
	////SRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	//srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
	////SRVヒープの先頭にあるSRVをルートパラメータ２番に設定
	//cmdList->SetGraphicsRootDescriptorTable(2, srvGpuHandle);

	texture->Draw(cmdList, ChangeTexure);

	cmdList->IASetIndexBuffer(&ibView);

	for (int i = 0; i < _countof(object3Ds); i++)
	{
		//object3Ds[0].Draw(cmdList, vbView, ibView, indices.size(), ChangeSquareFlag);
	}

	////定数バッファビュー(CBV)の設定コマンド(一番最初の引数は"ルートパラメータ"の要素番号である)
	//cmdList->SetGraphicsRootConstantBufferView(3, constBuffTransform0->GetGPUVirtualAddress());

	////描画コマンド
	//if (ChangeSquareFlag)
	//{
	//	//四角形に描画
	//	cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
	//}
	//else
	//{
	//	cmdList->DrawInstanced(3, 1, 0, 0);
	//}

	////定数バッファビュー(CBV)の設定コマンド(一番最初の引数は"ルートパラメータ"の要素番号である)
	//cmdList->SetGraphicsRootConstantBufferView(3, constBuffTransform1->GetGPUVirtualAddress());

	////描画コマンド
	//if (ChangeSquareFlag)
	//{
	//	//四角形に描画
	//	cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
	//}
	//else
	//{
	//	cmdList->DrawInstanced(3, 1, 0, 0);
	//}
}

//定数バッファを変更する関数
void DrawOBJ::constBuffColorUpdata(float Red, float Green, float Blue)
{
	constMapMaterial->color = XMFLOAT4(Red, Green, Blue, 1.0f);
}

void DrawOBJ::constBuffPosMUpdata(float X, float Y, float Z)
{
	constMapMaterial2->posM = XMFLOAT4(X, Y, Z, 0.0f);
}

void DrawOBJ::matViewUpdata(Float3 eye, Float3 target, Float3 up)
{
	eye_ = eye;
	target_ = target;
	up_ = up;

	/*matView = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));*/
	matView = matViewGeneration(eye_, target_, up_);

	for (size_t i = 0; i < _countof(object3Ds); i++)
	{
		//object3Ds[i].Update(matView, matProjection);
	}

}

void DrawOBJ::constTransformMatUpdata()
{
	//constMapTransform0->mat = matWorld * matView * matProjection;
}

void DrawOBJ::constTransformMatUpdata1()
{
	//constMapTransform1->mat = matWorld1 * matView * matProjection;
}

void DrawOBJ::matWorldUpdata()
{

	////スケール行列更新
	//matScale = matScaleGeneration(Scale_);

	////回転行列更新
	//matRotate = matRotateGeneration(Rotate_);

	////平行移動行列更新
	//matTrans = matMoveGeneration(Trans_);

	////ワールド行列更新
	//matWorld.IdentityMatrix();
	//matWorld *= matScale*matRotate*matTrans;

	for (size_t i = 0; i < _countof(object3Ds); i++)
	{
		//object3Ds[i].Update(matView, matProjection);
	}

}

void DrawOBJ::matWorldUpdata1()
{



}


void DrawOBJ::SetScale(Float3 scale)
{
	object3Ds[0].SetScale(scale);
}

void DrawOBJ::SetRotate(Float3 rotate)
{
	object3Ds[0].SetRotate(rotate);
}

void DrawOBJ::SetTrans(Float3 TransForm)
{
	object3Ds[0].SetPos(TransForm);
}

void DrawOBJ::obj3DUpdate()
{
	for (size_t i = 0; i < _countof(object3Ds); i++)
	{
		//object3Ds[i].Update(matView, matProjection);
	}
}

Matrix4x4 DrawOBJ::matScaleGeneration(Float3 scale)
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

Matrix4x4 DrawOBJ::matRotateXGeneration(float rotateX)
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

Matrix4x4 DrawOBJ::matRotateYGeneration(float rotateY)
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

Matrix4x4 DrawOBJ::matRotateZGeneration(float rotateZ)
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

Matrix4x4 DrawOBJ::matRotateGeneration(Float3 rotate)
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

	matRotateX *= matRotateY;

	matRotateZ *= matRotateX;

	matRotate = matRotateZ;

	return matRotate;



}

Matrix4x4 DrawOBJ::matMoveGeneration(Float3 translation)
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

Matrix4x4 DrawOBJ::matViewGeneration(Float3 eye, Float3 target, Float3 up)
{

	Float3 zVer = target - eye;

	zVer.normalize();

	Float3 xVer = up.cross(zVer);

	xVer.normalize();

	Float3 yVer = zVer.cross(xVer);


	Matrix4x4 cameraRotateMat = {};

	cameraRotateMat.IdentityMatrix();

	cameraRotateMat.m[0][0] = xVer.x;
	cameraRotateMat.m[0][1] = xVer.y;
	cameraRotateMat.m[0][2] = xVer.z;

	cameraRotateMat.m[1][0] = yVer.x;
	cameraRotateMat.m[1][1] = yVer.y;
	cameraRotateMat.m[1][2] = yVer.z;

	cameraRotateMat.m[2][0] = zVer.x;
	cameraRotateMat.m[2][1] = zVer.y;
	cameraRotateMat.m[2][2] = zVer.z;

	cameraRotateMat.m[3][0] = eye.x;
	cameraRotateMat.m[3][1] = eye.y;
	cameraRotateMat.m[3][2] = eye.z;

	Matrix4x4 ans = {};

	ans = cameraRotateMat.InverseMatrix();

	Float3 eyeDis = {};
	eyeDis = { target.x - eye.x,target.y - eye.y,target.z - eye.z };

	Float3 R2 = eyeDis.normalize();

	Float3 R0 = up.cross(R2);
	R0 = R0.normalize();

	Float3 R1 = R2.cross(R0);

	Float3 NegEyePosition = { -eye.x,-eye.y,-eye.z };

	Float3 D0 = float3Dat(R0, NegEyePosition);
	Float3 D1 = float3Dat(R1, NegEyePosition);
	Float3 D2 = float3Dat(R2, NegEyePosition);

	Matrix4x4 M;
	M.m[0][0] = R0.x;
	M.m[0][1] = R0.y;
	M.m[0][2] = R0.z;
	M.m[0][3] = D0.x;

	M.m[1][0] = R1.x;
	M.m[1][1] = R1.y;
	M.m[1][2] = R1.z;
	M.m[1][3] = D1.x;

	M.m[2][0] = R2.x;
	M.m[2][1] = R2.y;
	M.m[2][2] = R2.z;
	M.m[2][3] = D2.x;

	M.m[3][0] = 0.0f;
	M.m[3][1] = 0.0f;
	M.m[3][2] = 0.0f;
	M.m[3][3] = 1.0f;

	//M = XMMatrixTranspose(M);

	//// x.x,x.y,y.x,y.y
	//XMVECTOR vTemp1 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(1, 0, 1, 0));
	//// x.z,x.w,y.z,y.w
	//XMVECTOR vTemp3 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(3, 2, 3, 2));
	//// z.x,z.y,w.x,w.y
	//XMVECTOR vTemp2 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(1, 0, 1, 0));
	//// z.z,z.w,w.z,w.w
	//XMVECTOR vTemp4 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(3, 2, 3, 2));

	Float4 vTemp1 = { M.m[0][0],M.m[0][1],M.m[1][0],M.m[1][1] };
	Float4 vTemp2 = { M.m[0][2],M.m[0][3],M.m[1][2],M.m[1][3] };
	Float4 vTemp3 = { M.m[2][0],M.m[2][1],M.m[3][0],M.m[3][1] };
	Float4 vTemp4 = { M.m[2][2],M.m[2][3],M.m[3][2],M.m[3][3] };

	//XMMATRIX mResult;
	//// x.x,y.x,z.x,w.x
	//mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
	//// x.y,y.y,z.y,w.y
	//mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
	//// x.z,y.z,z.z,w.z
	//mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
	//// x.w,y.w,z.w,w.w
	//mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));

	Matrix4x4 mResult;
	mResult.m[0][0] = vTemp1.x;
	mResult.m[0][1] = vTemp1.z;
	mResult.m[0][2] = vTemp3.x;
	mResult.m[0][3] = vTemp3.z;

	mResult.m[1][0] = vTemp1.y;
	mResult.m[1][1] = vTemp1.w;
	mResult.m[1][2] = vTemp3.y;
	mResult.m[1][3] = vTemp3.w;

	mResult.m[2][0] = vTemp2.x;
	mResult.m[2][1] = vTemp2.z;
	mResult.m[2][2] = vTemp4.x;
	mResult.m[2][3] = vTemp4.z;

	mResult.m[3][0] = vTemp2.y;
	mResult.m[3][1] = vTemp2.w;
	mResult.m[3][2] = vTemp4.y;
	mResult.m[3][3] = vTemp4.w;

	//XMMATRIX matvi;

	//XMFLOAT3 eye2;
	//XMFLOAT3 target2;
	//XMFLOAT3 up2;

	////ビュー変換行列
	//eye2 = { eye.x, eye.y, eye.z };//視点座標
	//target2 = { target.x, target.y, target.z };//注視点座標
	//up2 = { up.x, up.y, up.z };//上方向ベクトル

	//matvi = XMMatrixLookAtLH(XMLoadFloat3(&eye2), XMLoadFloat3(&target2), XMLoadFloat3(&up2));

	return mResult;
}

Matrix4x4 DrawOBJ::perspectiveProjectionGeneration(float FovAngleY, float NearZ, float FarZ)
{
	Matrix4x4 ans = {};

	/*ans.IdentityMatrix();

	float fReciprocalWidth = 1.0f / (ViewRight - 0);
	float fReciprocalHeight = 1.0f / (0 - ViewBottom);

	float fRange = 1.0f / (FarZ - NearZ);

	ans.m[0][0] = {fReciprocalWidth * 2};

	ans.m[1][1] = { fReciprocalHeight * 2 };

	ans.m[2][2] = { fRange };

	ans.m[3][0] = { -(0 + ViewRight) * fReciprocalWidth };
	ans.m[3][1] = { -(0 + ViewBottom) * fReciprocalHeight };
	ans.m[3][2] = { fRange * -NearZ };*/

	float SinFov;
	float CosFov;
	float AspectRatio = Win_width / Win_height;
	sinCos(SinFov, CosFov, 0.5f * FovAngleY);

	float fRange = FarZ / (FarZ - NearZ);

	float Height = CosFov / SinFov;


	ans.m[0][0] = Height / AspectRatio;

	ans.m[1][1] = Height;

	ans.m[2][2] = fRange;
	ans.m[2][3] = 1.0f;

	ans.m[3][2] = (-fRange) * NearZ;

	return ans;

}

void DrawOBJ::sinCos(float& Sin, float& Cos, float angle)
{
	Sin = sinf(angle);
	Cos = cosf(angle);
}

Float3 DrawOBJ::float3Dat(Float3 A, Float3 B)
{
	Float3 num = { A.x * B.x,A.y * B.y,A.z * B.z };

	num.x = num.x + num.y + num.z;

	num = { num.x,num.x,num.x };

	return num;

}

void DrawOBJ::colorMap(float R, float G, float B)
{
	constMapMaterial->color = XMFLOAT4(R, G, B, 0.5f);
}
