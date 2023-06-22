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
#include "Material.h"

#include <unordered_map>


//一か所に持てる最大ボーン数?
static const uint32_t SNUM_BONES_PER_VERTEX = 4;

//最大ボーン数
static const uint32_t SMAX_BONE = 128;

//頂点データ
struct AnimationVertex
{

	Vector3 pos_;
	Vector3 normal_;
	Vector2 uv_;
	uint32_t ids_[SNUM_BONES_PER_VERTEX] = {};
	float weights_[SNUM_BONES_PER_VERTEX] = {};

};

//定数バッファ用
struct ConstBuffSkin
{
	Matrix4x4 boneMats_[SMAX_BONE] = {};
};

//ウエイトの情報
struct SetWeight
{
	uint32_t id_;
	float weight_;
};

//ボーンの情報
struct Bone
{
	//どこのやつか
	std::string name_;

	//初期位置
	Matrix4x4 offsetMatrix_;

	//最終位置?
	Matrix4x4 finalMatrix_;

};

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

public:

	//頂点データ(増やしたいならここも増やしておく)
	std::vector <Vertex> vertices_;


	//インデックスデータ
	std::vector< unsigned short> indices_;

};

//アニメーション用のメッシュ
class AnimationMesh
{
public:
	AnimationMesh();
	~AnimationMesh();

	void Init();

	

private:

	void SetSizeVB();

	void SetSizeIB();

	//頂点バッファ
	void VertexBuffObjGeneration(const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC Resdesc, D3D12_RESOURCE_STATES state);
	void VertexBuffMap();
	void VertexBuffViewGeneration();

	//インデックスデータ関連(インデックスビューもここ)
	
	void IndicesBuffGeneration(const D3D12_HEAP_PROPERTIES& HeapProp, D3D12_HEAP_FLAGS flag, const D3D12_RESOURCE_DESC Resdesc, D3D12_RESOURCE_STATES state);
	void IndicesBuffMap();
	void IndicesBuffViewGeneration(DXGI_FORMAT format);

private:
	
	


	//頂点データサイズ
	uint32_t sizeVB_;

	HRESULT result_;

	
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	AnimationVertex* vertMap_ = nullptr;
	
	

	//インデックスデータ全体のサイズ
	uint32_t sizeIB_;
	//インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_ = nullptr;
	

	//頂点法線スムージング用データ
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData_;

	

public:

	

	//頂点データ(増やしたいならここも増やしておく)
	std::vector <AnimationVertex> vertices_;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};



	//インデックスデータ
	std::vector< unsigned short> indices_;

	//インデックスビュー
	D3D12_INDEX_BUFFER_VIEW ibView_{};

	//テクスチャ
	static Texture* STexture_;

	//どのテクスチャか判断するための値(後でやり方を変える)
	std::vector <uint32_t> textureHandle = {};

	//マテリアル(複数に対応するため)
	std::vector<Material> material_;

};