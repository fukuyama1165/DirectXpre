#pragma once
//D3Dコンパイラ
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include<d3d12.h>
#pragma comment(lib,"d3d12.lib")

#include <vector>

#include "DirectXInit.h"

#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "matrix4x4.h"
#include "Texture.h"

#include <unordered_map>


class Material
{
public:
	Material();
	~Material();

	void MaterialConstBuffInit();

	void Update();

	void VertexBuffObjSetting(D3D12_HEAP_TYPE heaptype, D3D12_RESOURCE_DIMENSION dimension, uint32_t sizeVB, uint16_t height, uint16_t DepthOrArraySize, int16_t MipLevels, uint16_t SampleDescCount, D3D12_TEXTURE_LAYOUT layput);

	void IndicesBuffSetting(D3D12_RESOURCE_DIMENSION dimension, uint32_t sizeIB, uint16_t height, uint16_t DepthOrArraySize, int16_t MipLevels, uint16_t SampleDescCount, D3D12_TEXTURE_LAYOUT layput);

	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient_;//アンビエント影響度
		float pad1_;//パディング
		XMFLOAT3 diffuse_;//ディフューズ影響度
		float pad2_;//パディング
		XMFLOAT3 specular_;//スペキュラー影響度
		float alpha_;//アルファ
	};

	struct MaterialObj
	{
		std::string name_;//マテリアル名
		XMFLOAT3 ambient_ = {};//アンビエント影響度
		XMFLOAT3 diffuse_ = {};//ディフューズ影響度
		XMFLOAT3 specular_ = {};//スペキュラー影響度
		float alpha_ = 1.0f;//アルファ
		std::string textureFilename_;//テクスチャファイル名

		//コンストラクタ
		MaterialObj()
		{
			ambient_ = { 0.3f, 0.3f, 0.3f };
			diffuse_ = { 0.0f, 0.0f, 0.0f };
			specular_ = { 0.0f, 0.0f, 0.0f };
			alpha_ = 1.0f;
		}

	};

	//定数バッファ用
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1_ = nullptr;

	ConstBufferDataB1* constMapB1_ = nullptr;

	D3D12_HEAP_PROPERTIES cbHeapProp_{};//ヒープ設定

	MaterialObj material_;

	//頂点バッファ用変数
	D3D12_HEAP_PROPERTIES heapprop_{};//ヒープ設定
	D3D12_RESOURCE_DESC resDesc_{};//リソース設定

private:

	HRESULT result_;

};

