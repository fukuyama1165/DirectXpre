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
	void vertexBuffObjGeneration(ID3D12Device* dev);

	//インデックスデータ関連(インデックスビューもここ)
	void indicesBuffGeneration(ID3D12Device* dev);

private:

	//頂点データ構造体
	struct Vertex
	{
		Vector3 pos;//xyz座標
		Vector3 normal;//法線ベクトル
		Vector3 uv;//uv座標
	};

	struct Material
	{
		std::string name;//マテリアル名
		XMFLOAT3 ambient;//アンビエント影響度
		XMFLOAT3 diffuse;//ディフューズ影響度
		XMFLOAT3 specular;//スペキュラー影響度
		float alpha;//アルファ
		std::string textureFilename;//テクスチャファイル名

		//コンストラクタ
		Material()
		{
			ambient = { 0.3f, 0.3f, 0.3f };
			diffuse = { 0.0f, 0.0f, 0.0f };
			specular = { 0.0f, 0.0f, 0.0f };
			alpha = 1.0f;
		}

	};

	//頂点データ(増やしたいならここも増やしておく)
	std::vector <Vertex> vertices;

	//頂点データサイズ
	UINT sizeVB;

	HRESULT result;

	//頂点バッファ用変数
	D3D12_HEAP_PROPERTIES heapprop{};//ヒープ設定
	D3D12_RESOURCE_DESC resDesc{};//リソース設定
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
	Vertex* vertMap = nullptr;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	//インデックスデータ
	std::vector< unsigned short> indices;
	//インデックスデータ全体のサイズ
	UINT sizeIB;
	//インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;
	//インデックスビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};

	//頂点法線スムージング用データ
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;

	//テクスチャ
	static Texture* texture;

	//マテリアル
	Material material;

};

Model::Model()
{
}

Model::~Model()
{
}
