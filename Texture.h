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


#include <vector>

//const float PI = 3.141592653589;

#include <DirectXTex.h>

//ComPtr用インクルード
#include <wrl.h>

class Texture
{
public:

	static Texture* GetInstance();

	void Init(ID3D12Device* dev);

	/// <summary>
	/// テクスチャー描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	/// <param name="tex">loadTextureで帰ってきた値</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, int tex);

	//画像イメージデータ
	void imageDataGeneration(const char filename[]);

	//テクスチャバッファ
	void textureBuffGeneraion(ID3D12Device* dev);

	//シェーダリソースビュー
	void SRVGeneraion(ID3D12Device* dev);

	void instanceDelete();

	int loadTexture(const char filename[]);

private:

	Texture() = default;
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	static Texture* instance;

public:



private:

	HRESULT result;

	UINT incremantSize;

	Microsoft::WRL::ComPtr <ID3D12Device> dev;

	//画像データ等
	std::vector<TexMetadata> metadata{};
	std::vector<ScratchImage> scratchImg{};
	TexMetadata metadata2{};
	ScratchImage scratchImg2{};

	std::vector < D3D12_RESOURCE_DESC> textureResourceDesc{};
	D3D12_RESOURCE_DESC textureResourceDesc2{};

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> texBuff;
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff2 = nullptr;


	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> srvHeap = nullptr;
};
