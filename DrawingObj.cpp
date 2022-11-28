#include "DrawingObj.h"



using namespace Microsoft::WRL;





DrawingObj::DrawingObj(const float windowWidth,const float windowHeight)
{
	/*Win_width = windowWidth;
	Win_height = windowHeight;
	texture = Texture::GetInstance();*/
	

	//頂点データ(四点分の座標)
					//  x     y    z   法線  u    v
	//前
	//vertices[0] = { { -50.0f,-50.0f,50.0f },{},{0.0f,1.0f}};//左下
	//vertices[1] = { {  50.0f,-50.0f,50.0f },{},{0.0f,0.0f} };//左上
	//vertices[2] = { { -50.0f, 50.0f,50.0f },{},{1.0f,1.0f} };//右下
	//vertices[3] = { {  50.0f, 50.0f,50.0f },{},{1.0f,0.0f} };//右上

	////後
	//vertices[4] = { { -50.0f,-50.0f,-50.0f },{},{0.0f,1.0f} };//左下
	//vertices[5] = { { -50.0f, 50.0f,-50.0f },{},{0.0f,0.0f} };//左上
	//vertices[6] = { {  50.0f,-50.0f,-50.0f },{},{1.0f,1.0f} };//右下
	//vertices[7] = { {  50.0f, 50.0f,-50.0f },{},{1.0f,0.0f} };//右上

	////左
	//vertices[8]  = { { -50.0f,-50.0f,-50.0f },{},{0.0f,1.0f} };//左下
	//vertices[9]  = { { -50.0f,-50.0f, 50.0f },{},{0.0f,0.0f} };//左上
	//vertices[10] = { { -50.0f, 50.0f,-50.0f },{},{1.0f,1.0f} };//右下
	//vertices[11] = { { -50.0f, 50.0f, 50.0f },{},{1.0f,0.0f} };//右上

	////右
	//vertices[12] = { { 50.0f,-50.0f,-50.0f },{},{0.0f,1.0f} };//左下
	//vertices[13] = { { 50.0f, 50.0f,-50.0f },{},{0.0f,0.0f} };//左上
	//vertices[14] = { { 50.0f,-50.0f, 50.0f },{},{1.0f,1.0f} };//右下
	//vertices[15] = { { 50.0f, 50.0f, 50.0f },{},{1.0f,0.0f} };//右上

	////上
	//vertices[16] = { { -50.0f,50.0f,-50.0f },{},{0.0f,1.0f} };//左下
	//vertices[17] = { { -50.0f,50.0f, 50.0f },{},{0.0f,0.0f} };//左上
	//vertices[18] = { {  50.0f,50.0f,-50.0f },{},{1.0f,1.0f} };//右下
	//vertices[19] = { {  50.0f,50.0f, 50.0f },{},{1.0f,0.0f} };//右上

	////下
	//vertices[20] = { { -50.0f,-50.0f,-50.0f },{},{0.0f,1.0f} };//左下
	//vertices[21] = { {  50.0f,-50.0f,-50.0f },{},{0.0f,0.0f} };//左上
	//vertices[22] = { { -50.0f,-50.0f, 50.0f },{},{1.0f,1.0f} };//右下
	//vertices[23] = { {  50.0f,-50.0f, 50.0f },{},{1.0f,0.0f} };//右上

	////インデックスデータ
	////前
	//indices[0] = 0;
	//indices[1] = 1;
	//indices[2] = 2;
	//indices[3] = 2;
	//indices[4] = 1;
	//indices[5] = 3;

	////後
	//indices[6] = 4;
	//indices[7] = 5;
	//indices[8] = 6;
	//indices[9] = 6;
	//indices[10] = 5;
	//indices[11] = 7;

	////左
	//indices[12] = 8;
	//indices[13] = 9;
	//indices[14] = 10;
	//indices[15] = 10;
	//indices[16] = 9;
	//indices[17] = 11;

	////右
	//indices[18] = 12;
	//indices[19] = 13;
	//indices[20] = 14;
	//indices[21] = 14;
	//indices[22] = 13;
	//indices[23] = 15;

	////下
	//indices[24] = 16;
	//indices[25] = 17;
	//indices[26] = 18;
	//indices[27] = 18;
	//indices[28] = 17;
	//indices[29] = 19;

	////上
	//indices[30] = 20;
	//indices[31] = 21;
	//indices[32] = 22;
	//indices[33] = 22;
	//indices[34] = 21;
	//indices[35] = 23;
	//
	//
	////頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	//sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	////インデックスデータ全体のサイズ
	//sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	////ビュー変換行列
	//eye_ = { 0, 0, -400.0f };//視点座標
	//target_ = { 0, 0, 0 };//注視点座標
	//up_ = { 0, 1, 0 };//上方向ベクトル
}

DrawingObj::DrawingObj(const float windowWidth, const float windowHeight,XMFLOAT3 vertexPos1, XMFLOAT3 vertexPos2, XMFLOAT3 vertexPos3, XMFLOAT3 vertexPos4, XMFLOAT2 vertexUv1, XMFLOAT2 vertexUv2, XMFLOAT2 vertexUv3, XMFLOAT2 vertexUv4)
{
	/*Win_width = windowWidth;
	Win_height = windowHeight;
	texture = Texture::GetInstance();;*/

	//頂点データ(四点分の座標)
					//  x     y    z      u    v
	//vertices[0] = { vertexPos1,{},vertexUv1 };//左下
	//vertices[1] = { vertexPos2,{},vertexUv2 };//左上
	//vertices[2] = { vertexPos3,{},vertexUv3 };//右下
	//vertices[3] = { vertexPos4,{},vertexUv4 };//右上

	////インデックスデータ
	//indices[0] = 0;
	//indices[1] = 1;
	//indices[2] = 2;
	//indices[3] = 1;
	//indices[4] = 2;
	//indices[5] = 3;



	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	//sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	////インデックスデータ全体のサイズ
	//sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	////ビュー変換行列
	//eye_ = { 0, 0, -100 };//視点座標
	//target_ = { 0, 0, 0 };//注視点座標
	//up_ = { 0, 1, 0 };//上方向ベクトル
}


DrawingObj::~DrawingObj()
{
	
}




//void DrawingObj::constantBuffGeneration1(ID3D12Device* dev)
//{
//#pragma region 定数バッファ
//
//	//定数バッファの生成用の設定
//	//ヒープ設定
//	//D3D12_HEAP_PROPERTIES cbHeapProp{};
//	//cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用
//
//
//	////リソース設定
//	//D3D12_RESOURCE_DESC cbResourceDesc{};
//
//	//cbResourceDesc = constBuffResourceGeneration(sizeof(ConstBufferDataTransform));
//
//	////定数バッファの生成
//	//result = dev->CreateCommittedResource(
//	//	&cbHeapProp,//ヒープ設定
//	//	D3D12_HEAP_FLAG_NONE,
//	//	&cbResourceDesc,//リソース設定
//	//	D3D12_RESOURCE_STATE_GENERIC_READ,
//	//	nullptr,
//	//	IID_PPV_ARGS(&constBuffTransform1)
//	//);
//	//assert(SUCCEEDED(result));
//
//
//	////定数バッファのマッピング
//
//	//result = constBuffTransform1->Map(0, nullptr, (void**)&constMapTransform1);//マッピング
//	//assert(SUCCEEDED(result));
//
//	////行列に単位行列を代入
//	///*matWorld.IdentityMatrix();
//
//	//matScale.IdentityMatrix();
//
//	//matRotate.IdentityMatrix();
//
//	//matTrans.IdentityMatrix();
//
//	//constMapTransform->mat.IdentityMatrix();*/
//
//	///*constMapTransform->mat.r[0].m128_f32[0] = 2.0f / Win_width;
//	//constMapTransform->mat.r[1].m128_f32[1] = -2.0f / Win_height;
//	//constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
//	//constMapTransform->mat.r[3].m128_f32[1] = 1.0f;*/
//
//	////平行投射行列の計算
//	//constMapTransform->mat = XMMatrixOrthographicOffCenterLH(0.0f,Win_width,Win_height, 0.0f, 0.0f, 1.0f);
//
//	//matWorldUpdata1();
//
//	//constTransformMatUpdata1();
//
//#pragma endregion
//}


void DrawingObj::imageDataGeneration()
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
	

	////WICテクスチャのロード
	//result = LoadFromWICFile(
	//	L"Resources/hokehoke.png",
	//	WIC_FLAGS_NONE,
	//	&metadata,
	//	scratchImg
	//);

	//ScratchImage mipChain{};
	////ミップマップの生成
	//result = GenerateMipMaps(
	//	scratchImg.GetImages(),
	//	scratchImg.GetImageCount(),
	//	scratchImg.GetMetadata(),
	//	TEX_FILTER_DEFAULT,
	//	0,
	//	mipChain
	//);

	//if (SUCCEEDED(result))
	//{
	//	scratchImg = std::move(mipChain);
	//	metadata = scratchImg.GetMetadata();
	//}

	////読み込んだディフューズテクスチャをSRGBとして扱う
	//metadata.format = MakeSRGB(metadata.format);

#pragma endregion
}

void DrawingObj::textureBuffGeneraion(ID3D12Device* dev)
{
#pragma region テクスチャバッファ設定

	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	////リソース設定
	//D3D12_RESOURCE_DESC textureResourceDesc{};
	//textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//textureResourceDesc.Format = metadata.format;
	//textureResourceDesc.Width = metadata.width;//幅
	//textureResourceDesc.Height = (UINT)metadata.height;//高さ
	//textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	//textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	//textureResourceDesc.SampleDesc.Count = 1;

#pragma endregion

#pragma region テクスチャバッファの生成

	//テクスチャバッファの生成
	//
	//result = dev->CreateCommittedResource(
	//	&textureHeapProp,
	//	D3D12_HEAP_FLAG_NONE,
	//	&textureResourceDesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&texBuff)
	//);

	////ミップマップで置き換える
	////全ミップマップについて
	//for (size_t i = 0; i < metadata.mipLevels; i++)
	//{
	//	//ミップマップレベルを指定してイメージを取得
	//	const Image* img = scratchImg.GetImage(i, 0, 0);
	//	//テクスチャバッファにデータ転送
	//	result = texBuff->WriteToSubresource(
	//		(UINT)i,
	//		nullptr,//全領域へコピー
	//		img->pixels,//元データアドレス
	//		(UINT)img->rowPitch,//1ラインアドレス
	//		(UINT)img->slicePitch//1枚サイズ
	//	);
	//	assert(SUCCEEDED(result));
	//}

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

void DrawingObj::SRVGeneraion(ID3D12Device* dev)
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
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	//srvDesc.Format = resDesc.Format;//RGBA float
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	//srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//ハンドルの指す位置にシェーダリソースビュー作成
	//dev->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);

#pragma endregion
}

void DrawingObj::vertexMap()
{

	//vertBuff->Map(0, nullptr, (void**)&vertMap);
	////全頂点に対して
	//for (int i = 0; i < _countof(vertices); i++)
	//{
	//	vertMap[i] = vertices[i];//座標をコピー
	//}

	////つながりを解除
	//vertBuff->Unmap(0, nullptr);
}

void DrawingObj::Draw(ID3D12GraphicsCommandList* cmdList,bool PipeLineRuleFlag, bool ChangeSquareFlag,bool ChangeTexure)
{

	//パイプラインステートとルートシグネチャの設定
	//作ったパイプラインステートとルートシグネチャをセットする
	//決めたルールで描画をお願いするところ
	//D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;

	//if (PipeLineRuleFlag)
	//{
	//	cmdList->SetPipelineState(pipelinestate.Get());
	//}
	//else
	//{
	//	cmdList->SetPipelineState(pipelinestate2.Get());
	//}

	//cmdList->SetGraphicsRootSignature(rootsignature.Get());

	////頂点バッファビューの設定
	//cmdList->IASetVertexBuffers(0, 1, &vbView);



	////定数バッファビュー(CBV)の設定コマンド
	//cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	//cmdList->SetGraphicsRootConstantBufferView(1, constBuffMaterial2->GetGPUVirtualAddress());


	//SRVヒープの設定コマンド
	//cmdList->SetDescriptorHeaps(1, &texture.srvHeap);
	////SRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	//srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
	////SRVヒープの先頭にあるSRVをルートパラメータ２番に設定
	//cmdList->SetGraphicsRootDescriptorTable(2, srvGpuHandle);

	/*texture->Draw(cmdList, ChangeTexure);
	cmdList->IASetIndexBuffer(&ibView);
	for (int i = 0; i < _countof(object3Ds); i++)
	{
		object3Ds[0].Draw(cmdList, vbView, ibView, _countof(indices),ChangeSquareFlag);
	}*/

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

XMFLOAT3 DrawingObj::Afin(XMFLOAT3 box, float moveX, float moveY, float rotate, float scaleX, float scaleY)
{
	//float ansBuff[3] = {};
	//float ansBuff2[3] = {};
	//float ansBuff3[3] = {};
	//float ansBuff4[3] = {};
	//float ans[3] = {};

	//float moveA[3][3] = {
	//	{1.0f,0.0f, moveX},
	//	{0.0f,1.0f, moveY},
	//	{0.0f,0.0f, 1.0f}
	//};
	//float rotateA[3][3] = {
	//	{cosf(rotate * (PI / 180)),-sinf(rotate * (PI / 180)), 0.0f},
	//	{sinf(rotate * (PI / 180)),cosf(rotate * (PI / 180)), 0.0f},
	//	{0.0f,0.0f, 1.0f}
	//};

	//float scaleA[3][3] = {
	//	{scaleX,0.0f, 0.0f},
	//	{0.0f,scaleY, 0.0f},
	//	{0.0f,0.0f, 1.0f}
	//};

	//ans[0] = box.x;
	//ans[1] = box.y;
	//ans[2] = 1.0f;

	///*ansBuff[0] = moveA[0][0] * vertices[0].pos.x + moveA[0][1] * vertices[0].pos.y + -ans[0] * 1.0f;
	//ansBuff[1] = moveA[1][0] * vertices[0].pos.x + moveA[1][1] * vertices[0].pos.y + -ans[1] * 1.0f;
	//ansBuff[2] = moveA[2][0] * vertices[0].pos.x + moveA[2][1] * vertices[0].pos.y + ans[2] * 1.0f;*/


	//ansBuff2[0] = rotateA[0][0] * ansBuff[0] + rotateA[0][1] * ansBuff[1] + rotateA[0][2] * ansBuff[2];
	//ansBuff2[1] = rotateA[1][0] * ansBuff[0] + rotateA[1][1] * ansBuff[1] + rotateA[1][2] * ansBuff[2];
	//ansBuff2[2] = rotateA[2][0] * ansBuff[0] + rotateA[2][1] * ansBuff[1] + rotateA[2][2] * ansBuff[2];

	//ansBuff3[0] = scaleA[0][0] * ansBuff2[0] + scaleA[0][1] * ansBuff2[1] + scaleA[0][2] * ansBuff2[2];
	//ansBuff3[1] = scaleA[1][0] * ansBuff2[0] + scaleA[1][1] * ansBuff2[1] + scaleA[1][2] * ansBuff2[2];
	//ansBuff3[2] = scaleA[2][0] * ansBuff2[0] + scaleA[2][1] * ansBuff2[1] + scaleA[2][2] * ansBuff2[2];

	//ansBuff4[0] = moveA[0][0] * ansBuff3[0] + moveA[0][1] * ansBuff3[1] + ans[0] * ansBuff3[2];
	//ansBuff4[1] = moveA[1][0] * ansBuff3[0] + moveA[1][1] * ansBuff3[1] + ans[1] * ansBuff3[2];
	//ansBuff4[2] = moveA[2][0] * ansBuff3[0] + moveA[2][1] * ansBuff3[1] + ans[2] * ansBuff3[2];

	//box.x = moveA[0][0] * ansBuff4[0] + moveA[0][1] * ansBuff4[1] + moveA[0][2] * ansBuff4[2];
	//box.y = moveA[1][0] * ansBuff4[0] + moveA[1][1] * ansBuff4[1] + moveA[1][2] * ansBuff4[2];

	return box;

}

//Objに対してアフィン変換をかける関数
void DrawingObj::ObjAfin(float moveX, float moveY, float rotate, float scaleX, float scaleY)
{
#pragma region アフィン変換

	//原点は最後に変換する(元座標が変わってしまうため)
	/*vertices[1].pos = Afin(vertices[1].pos, moveX, moveY, rotate, scaleX, scaleY);
	vertices[2].pos = Afin(vertices[2].pos, moveX, moveY, rotate, scaleX, scaleY);
	vertices[3].pos = Afin(vertices[3].pos, moveX, moveY, rotate, scaleX, scaleY);
	vertices[0].pos = Afin(vertices[0].pos, moveX, moveY, rotate, scaleX, scaleY);*/

#pragma endregion

#pragma region アフィン変換後の頂点データをシェーダに転送

	vertexMap();

#pragma endregion
}

//定数バッファを変更する関数
void DrawingObj::constBuffColorUpdata(float Red, float Green, float Blue)
{
	//constMapMaterial->color = XMFLOAT4(Red, Green, Blue, 1.0f);
}

void DrawingObj::constBuffPosMUpdata(float X, float Y, float Z)
{
	//constMapMaterial2->posM = XMFLOAT4(X, Y, Z, 0.0f);
}

void DrawingObj::matViewUpdata(Float3 eye, Float3 target, Float3 up)
{
	//eye_ = eye;
	//target_ = target;
	//up_ = up;

	///*matView = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));*/
	//matView = matViewGeneration(eye_, target_, up_);

	//for (size_t i = 0; i < _countof(object3Ds); i++)
	//{
	//	object3Ds[i].Update(matView, matProjection);
	//}
	
}

void DrawingObj::constTransformMatUpdata()
{
	//constMapTransform0->mat = matWorld * matView * matProjection;
}

void DrawingObj::constTransformMatUpdata1()
{
	//constMapTransform1->mat = matWorld1 * matView * matProjection;
}

void DrawingObj::matWorldUpdata()
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

	/*for (size_t i = 0; i < _countof(object3Ds); i++)
	{
		object3Ds[i].Update(matView, matProjection);
	}*/
	
}

void DrawingObj::matWorldUpdata1()
{

	

}


void DrawingObj::SetScale(Float3 scale)
{
	//object3Ds[0].SetScale(scale);
}

void DrawingObj::SetRotate(Float3 rotate)
{
	//object3Ds[0].SetRotate(rotate);
}

void DrawingObj::SetTrans(Float3 TransForm)
{
	//object3Ds[0].SetPos(TransForm);
}

void DrawingObj::obj3DUpdate()
{
	/*for (size_t i = 0; i < _countof(object3Ds); i++)
	{
		object3Ds[i].Update(matView, matProjection);
	}*/
}

Matrix4x4 DrawingObj::matScaleGeneration(Float3 scale)
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

Matrix4x4 DrawingObj::matRotateXGeneration(float rotateX)
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

Matrix4x4 DrawingObj::matRotateYGeneration(float rotateY)
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

Matrix4x4 DrawingObj::matRotateZGeneration(float rotateZ)
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

Matrix4x4 DrawingObj::matRotateGeneration(Float3 rotate)
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

Matrix4x4 DrawingObj::matMoveGeneration(Float3 translation)
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


void DrawingObj::colorMap(float R, float G, float B)
{
	//constMapMaterial->color = XMFLOAT4(R, G, B, 0.5f);
}