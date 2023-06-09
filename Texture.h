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

#include <cstdint>

class Texture
{
public:

	static Texture* GetInstance();

	void Init();

	/// <summary>
	/// テクスチャー描画
	/// </summary>
	/// <param name="tex">loadTextureで帰ってきた値</param>
	void Draw(uint32_t tex);

	

	

	void instanceDelete();

	uint32_t loadTexture(const char filename[]);
	uint32_t loadTexture(std::string filename);

	ID3D12Resource* GetTexBuff(uint32_t index)const { return texBuff_[index].Get(); }

private:

	Texture() = default;
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	static Texture* instance;

	void SRVHeapGeneraion();

	//画像イメージデータ(古いほうなのでできれば引数違いを使いましょう)
	void imageDataGeneration(const char filename[]);
	void imageDataGeneration(std::string filename);

	//テクスチャバッファ(古いほうなのでできれば引数違いを使いましょう)
	void textureBuffGeneraion();

	//画像イメージデータ
	void imageDataGeneration(const char filename[], TexMetadata& metadataBuff, ScratchImage& scratchImgBuff);
	void imageDataGeneration(std::string filename, TexMetadata& metadataBuff, ScratchImage& scratchImgBuff);

	//テクスチャバッファ
	void textureBuffGeneraion(TexMetadata& metadataBuff, ScratchImage& scratchImgBuff, D3D12_RESOURCE_DESC& textureResourceDescBuff);

	//シェーダリソースビュー
	void SRVGeneraion(const D3D12_RESOURCE_DESC& textureResourceDesc, Microsoft::WRL::ComPtr<ID3D12Resource> newTexBuff);

public:



private:

	HRESULT result_;

	uint32_t incremantSize_;

	Microsoft::WRL::ComPtr <ID3D12Device> dev_;

	//画像データ等
	std::vector<TexMetadata> metadata_{};
	std::vector<ScratchImage> scratchImg_{};

	std::vector < D3D12_RESOURCE_DESC> textureResourceDesc_{};
	D3D12_RESOURCE_DESC textureResourceDesc2_{};

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> texBuff_;
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff2_ = nullptr;

	std::vector<std::string>textureFileName_{};

	bool isReturn_ = false;
	uint32_t sufferTexNum_ = 0;
	uint32_t TexNum_ = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle_;


	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> srvHeap_ = nullptr;
};
