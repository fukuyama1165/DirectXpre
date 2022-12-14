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

	

	

	void instanceDelete();

	int loadTexture(const char filename[]);
	int loadTexture(std::string filename);

private:

	Texture() = default;
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	static Texture* instance;

	//画像イメージデータ(古いほうなのでできれば引数違いを使いましょう)
	void imageDataGeneration(const char filename[]);
	void imageDataGeneration(std::string filename);

	//テクスチャバッファ(古いほうなのでできれば引数違いを使いましょう)
	void textureBuffGeneraion(ID3D12Device* dev);

	//シェーダリソースビュー(古いほうなので引数違いを使いましょうSRVヒープが生成二回行われる可能性があるので注意)
	void SRVGeneraion(ID3D12Device* dev);

	//画像イメージデータ
	void imageDataGeneration2(const char filename[], TexMetadata& metadataBuff, ScratchImage& scratchImgBuff);

	//テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource > textureBuffGeneraion2(ID3D12Device* dev, TexMetadata& metadataBuff, ScratchImage& scratchImgBuff, D3D12_RESOURCE_DESC& textureResourceDescBuff, Microsoft::WRL::ComPtr<ID3D12Resource > newTexBuff);

	//シェーダリソースビュー
	void SRVGeneraion(ID3D12Device* dev, D3D12_RESOURCE_DESC& textureResourceDesc, Microsoft::WRL::ComPtr<ID3D12Resource > newTexBuff);

public:



private:

	HRESULT result;

	UINT incremantSize;

	Microsoft::WRL::ComPtr <ID3D12Device> dev;

	//画像データ等
	std::vector<TexMetadata> metadata{};
	std::vector<ScratchImage> scratchImg{};

	std::vector < D3D12_RESOURCE_DESC> textureResourceDesc{};
	D3D12_RESOURCE_DESC textureResourceDesc2{};

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> texBuff;
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff2 = nullptr;

	std::vector<std::string>textureFileName{};

	bool isReturn = false;
	int sufferTexNum = 0;


	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> srvHeap = nullptr;
};
