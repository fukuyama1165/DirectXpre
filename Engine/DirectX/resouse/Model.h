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
#include "Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <unordered_map>


bool DoTheImportThing(const std::string& pFile);

class Model
{
public:
	Model();
	~Model();

private:

	//頂点バッファ
	void vertexBuffObjGeneration();

	//インデックスデータ関連(インデックスビューもここ)
	void indicesBuffGeneration();

private:

	//頂点データ構造体
	struct Vertex
	{
		Vector3 pos_;//xyz座標
		Vector3 normal_;//法線ベクトル
		Vector3 uv_;//uv座標
	};

	struct Material
	{
		std::string name_;//マテリアル名
		XMFLOAT3 ambient_;//アンビエント影響度
		XMFLOAT3 diffuse_;//ディフューズ影響度
		XMFLOAT3 specular_;//スペキュラー影響度
		float alpha_;//アルファ
		std::string textureFilename_;//テクスチャファイル名

		//コンストラクタ
		Material()
		{
			ambient_ = { 0.3f, 0.3f, 0.3f };
			diffuse_ = { 0.0f, 0.0f, 0.0f };
			specular_ = { 0.0f, 0.0f, 0.0f };
			alpha_ = 1.0f;
		}

	};

	//頂点データ(増やしたいならここも増やしておく)
	std::vector <Vertex> vertices_;

	//頂点データサイズ
	uint32_t sizeVB_;

	HRESULT result_;

	//頂点バッファ用変数
	D3D12_HEAP_PROPERTIES heapprop_{};//ヒープ設定
	D3D12_RESOURCE_DESC resDesc_{};//リソース設定
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	Vertex* vertMap_ = nullptr;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	//インデックスデータ
	std::vector< unsigned short> indices_;
	//インデックスデータ全体のサイズ
	uint32_t sizeIB_;
	//インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_ = nullptr;
	//インデックスビュー
	D3D12_INDEX_BUFFER_VIEW ibView_{};

	//頂点法線スムージング用データ
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData_;

	//テクスチャ
	static Texture* STexture_;

	//マテリアル
	Material material_;

};

Model::Model()
{
}

Model::~Model()
{
}
