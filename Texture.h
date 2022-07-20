#pragma once
//D3Dコンパイラ
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include<vector>
#include<string>

#include<d3d12.h>
#include<dxgi1_6.h>

//数学ライブラリ(最適化されすぎて使いにくいらしいのであとで自作しろって)
#include<DirectXMath.h>
using namespace DirectX;




//const float PI = 3.141592653589;

#include <DirectXTex.h>

//ComPtr用インクルード
#include <wrl.h>

class Texture
{
public:
	Texture();
	~Texture();

	void Init(ID3D12Device* dev);

	void Draw(ID3D12GraphicsCommandList* cmdList);

	//画像イメージデータ
	void imageDataGeneration();

	//テクスチャバッファ
	void textureBuffGeneraion(ID3D12Device* dev);

	//シェーダリソースビュー
	void SRVGeneraion(ID3D12Device* dev);

public:

	

private:

	HRESULT result;

	UINT incremantSize;

	//画像データ等
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	TexMetadata metadata2{};
	ScratchImage scratchImg2{};

	D3D12_RESOURCE_DESC textureResourceDesc{};
	D3D12_RESOURCE_DESC textureResourceDesc2{};

	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff2 = nullptr;

	
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> srvHeap = nullptr;
};

