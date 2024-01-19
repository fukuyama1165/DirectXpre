#pragma once

/**
 * @file Texture.h
 * @brief テクスチャデータを読み込みをしキーで登録を行える
 * @author フクヤマ
 * @date 2023_11/10
 */
//D3Dコンパイラ
#include<d3dcompiler.h>

#include<vector>
#include<string>

#include<d3d12.h>
#include<dxgi1_6.h>

//数学ライブラリ(最適化されすぎて使いにくいらしいのであとで自作しろって)
#include<DirectXMath.h>

#include <vector>
#include <memory>

#include <DirectXTex.h>

//ComPtr用インクルード
#include <wrl.h>

#include <cstdint>

#include <map>

struct TextureDeta
{

	//ヒープの位置
	D3D12_CPU_DESCRIPTOR_HANDLE srvCpuHandle= D3D12_CPU_DESCRIPTOR_HANDLE();
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle= D3D12_GPU_DESCRIPTOR_HANDLE();

	//ヒープの何番目にあるか
	uint32_t heapPositionIndex = 0;

	//テクスチャ情報
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff = nullptr;

	//ファイルパス
	std::string fileName = "";

};

class Texture
{
public:

	static Texture* GetInstance();

	void Init();

	/// <summary>
	/// テクスチャー描画
	/// </summary>
	/// <param name="tex">loadTextureで帰ってきた値</param>
	void Draw(std::string handle);

	std::string loadTexture(const std::string& filename, std::string handle = "");

	/// <summary>
	/// テクスチャを読み込んで登録する(動画読み込み用)
	/// </summary>
	/// <param name="new_xres">横幅</param>
	/// <param name="new_yres">縦幅</param>
	/// <param name="new_format">フォーマット</param>
	/// <param name="is_dynamic">ダイナミックか(フラグを変更する用)</param>
	/// <param name="filename">確認用のファイル名</param>
	/// <param name="handle">登録するテクスチャハンドル</param>
	/// <returns></returns>
	std::string loadTexture(float new_xres, float new_yres, DXGI_FORMAT new_format, bool is_dynamic, const std::string& filename, std::string handle = "");

	ID3D12Resource* GetTexBuff(std::string index) { return textureDetas_[index]->texBuff.Get(); };

private:

	Texture() = default;
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	

	void SRVHeapGeneraion();

	//画像イメージデータ
	void imageDataGeneration(const std::string& filename, DirectX::TexMetadata& metadataBuff, DirectX::ScratchImage& scratchImgBuff);

	//テクスチャバッファ
	void textureBuffGeneraion(DirectX::TexMetadata& metadataBuff, DirectX::ScratchImage& scratchImgBuff, D3D12_RESOURCE_DESC& textureResourceDescBuff);

	//シェーダリソースビュー
	void SRVGeneraion(const D3D12_RESOURCE_DESC& textureResourceDesc, Microsoft::WRL::ComPtr<ID3D12Resource> newTexBuff, const std::string& filename, std::string& handle);

	//拡張子を返す関数
	std::string SeparateFilePath(const std::string& filePath);

public:



private:

	HRESULT result_ = S_OK;

	

	//テクスチャデータの連想配列
	std::map<std::string, std::shared_ptr<TextureDeta>> textureDetas_;

	//画像データ等
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff2_ = nullptr;

	//初期化したかどうか
	static bool SInitFlag_;

	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle_{};

	//srvヒープ保存用
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> srvHeap_ = nullptr;

	//使う大きさ
	static const size_t kMaxSRVCount = 2056;
};
