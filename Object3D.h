#pragma once
//D3Dコンパイラ
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include<d3d12.h>
#include<dxgi1_6.h>

//数学ライブラリ(最適化されすぎて使いにくいらしいのであとで自作しろって)
#include<DirectXMath.h>
using namespace DirectX;

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "DirectXInit.h"

#include "matrix4x4.h"
#include "Vector3.h"
#include "Quaternion.h"

//ComPtr用インクルード
#include <wrl.h>

#include <DirectXTex.h>

#include "Texture.h"
#include "Vector4.h"
#include "camera.h"

#include "PI.h"

#include <unordered_map>

#include "LightGroup.h"

#include "CollisionInfo.h"

#include "CollisionManager.h"

class BaseCollider;

class Object3D
{
public:
	Object3D();
	~Object3D();

	//描画初期化処理関数
	void basicInit();//basicPS読み込み
	void colorChangeInit();//colorChangePS読み込み
	void objDrawInit(const std::string directoryPath, const char filename[], bool smoothing = false);//.objを読み込んで描画

	void SetScale(Vector3 scale);
	void SetRotate(Vector3 rotate);
	void SetPos(Vector3 pos);
	void SetParent(Object3D* parent);

	/// <summary>
	/// コライダーのセット
	/// </summary>
	/// <param name="Collider">コライダー</param>
	void SetCollider(BaseCollider* Collider);

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	virtual void OnCollision(const CollisionInfo& info) {};

	Matrix4x4 GetWorldMat();

	Object3D* GetParent();

	Vector3 GetWorldPos();

	Vector3 GetScale();
	Vector3 GetRotate();
	Vector3 GetPos();

	void matWorldGeneration();

	void Update(Camera camera);

	/// <summary>
	/// テクスチャを含めた描画
	/// </summary>
	/// <param name="ChangeTexure">loadTextureで帰ってきた値を入れると読み込んだ画像が出るはず</param>
	/// <param name="PipeLineRuleFlag">パイプラインステートを変化させるフラグ</param>
	/// <param name="ChangeSquareFlag">頂点３つだけ使って描画するように変化させるフラグ</param>
	void Draw(int ChangeTexure = 1, bool PipeLineRuleFlag = true, bool ChangeSquareFlag = true);

	int loadTexture(const char filename[]);
	int loadTexture(std::string filename);

	void deleteTexture();

	static void SetLight(LightGroup* light)
	{
		Object3D::lightGroup = light;
	}



private:
	//使ってない
	void Init(ID3D12Device* dev);

	void constTransformMatUpdata(Vector3 eye, Matrix4x4 matView, Matrix4x4 matProjection);

	
	


	Matrix4x4 matScaleGeneration(Vector3 scale);

	Matrix4x4 matRotateXGeneration(float rotateX);
	Matrix4x4 matRotateYGeneration(float rotateY);
	Matrix4x4 matRotateZGeneration(float rotateZ);

	Matrix4x4 matRotateGeneration(Vector3 rotate);

	Matrix4x4 matMoveGeneration(Vector3 translation);

	

	

	

	

	//ただ四角や三角形の為の頂点データ設定関数

	void basicVertexInit();
	void colorChangeVertexInit();

	//頂点データ構造体
	struct Vertex
	{
		XMFLOAT3 pos;//xyz座標
		XMFLOAT3 normal;//法線ベクトル
		XMFLOAT2 uv;//uv座標
	};

	//頂点バッファ生成
	void vertexBuffGeneration(ID3D12Device* dev);
	void vertexBuffObjGeneration(ID3D12Device* dev);
	void objVertexBuffGeneration(ID3D12Device* dev, const std::string directoryPath, const char filename[],bool smoothing);

	//頂点シェーダの読み込みとコンパイル
	void vertexShaderGeneration();//basicVS読み込み
	void vertexShaderGeneration2();//vertexMoveVS読み込み
	void vertexShaderGeneration3();//OBJVS読み込み

	//ピクセルシェーダの読み込みとコンパイル
	void pixelShaderGeneration();//basicPS読み込み
	void pixelShaderGeneration2();//colorChangePS読み込み
	void pixelShaderGeneration3();//OBJPS読み込み

	//頂点レイアウトの設定
	void vertexLayout();

	//グラフィックスパイプラインの設定
	void graphicPipelineGeneration();

	//デスクリプタレンジの設定
	void descriptorRangeGeneration();

	//ルートパラメータの設定(定数バッファとシェーダについて)
	void rootParamGeneration();

	//テクスチャサンプラーの設定
	void textureSamplerGeneration();

	//ルートシグネチャ
	void rootsignatureGeneration(ID3D12Device* dev);

	//定数バッファ
	void constantBuffGeneration(ID3D12Device* dev);

	//インデックスデータ関連(インデックスビューもここ)
	void indicesBuffGeneration(ID3D12Device* devconst);

	

	

	

	void matViewUpdata(Vector3 eye, Vector3 target, Vector3 up);

	void loadMaterial(std::string filename, const std::string directoryPath);

	
	/// <summary>
	/// マテリアルのテクスチャ読み込み
	/// </summary>
	/// <returns>成否</returns>
	bool LoadMaterialTexture(std::string filename);


	Matrix4x4 QuaternionMatRotateGeneration(Vector3 rotate);

public:

	Vector3 Scale_;
	Vector3 Rotate_;
	Vector3 Trans_;

protected:

	//画面サイズ
	static float Win_width;
	static float Win_height;

	

	const char* name = nullptr;
	BaseCollider* collider = nullptr;


	//定数バッファ用データ構造体(3D変換行列)
	struct ConstBufferDataTransform
	{
		//Matrix4x4 mat;//3D変換行列
		Matrix4x4 viewProj;//ビュープロジェクション行列
		Matrix4x4 world;//ワールド行列
		XMFLOAT3 cameraPos;//カメラ座標(ワールド座標)
	};

	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;//アンビエント影響度
		float pad1;//パディング
		XMFLOAT3 diffuse;//ディフューズ影響度
		float pad2;//パディング
		XMFLOAT3 specular;//スペキュラー影響度
		float alpha;//アルファ
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


	static LightGroup* lightGroup;
	

	Matrix4x4 matScale;
	Matrix4x4 matRotate;
	Matrix4x4 matTrans;

	Matrix4x4 matWorld;

	Object3D* parent_ = nullptr;

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

	//頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr;

	//ピクセルシェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr;

	//エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	//頂点レイアウト(要素を増やすなら配列数を増やす)
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;

	//グラフィックスパイプラインの各ステージの設定をする構造体を用意
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline2{};

	//デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	//ルートパラメータ(定数バッファの数が増えたら配列の要素数を増やして設定をしている関数の中身にも追加すること)
	std::vector<D3D12_ROOT_PARAMETER> rootParam = {};

	//テクスチャサンプラー
	D3D12_STATIC_SAMPLER_DESC sampleDesc{};

	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature;

	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate2 = nullptr;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};

	//定数バッファ用のリソース設定関数
	D3D12_RESOURCE_DESC constBuffResourceGeneration(int size);




	//定数バッファそのもの

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1 = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;


	//マッピングするときのポインタ
	ConstBufferDataTransform* constMapTransform = nullptr;
	ConstBufferDataB1* constMapB1 = nullptr;


	//インデックスデータ
	std::vector< unsigned short> indices;
	//インデックスデータ全体のサイズ
	UINT sizeIB;
	//インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;
	//インデックスビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};

	//画像データ等
	TexMetadata metadata{};
	//ScratchImage scratchImg{};

	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff = nullptr;

	//ID3D12DescriptorHeap* srvHeap = nullptr;

	//頂点法線スムージング用データ
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;

	static Texture* texture;

	Material material;

	int materialTextureNum = -1;

	Vector3 forward = { 0.0f,0.0f,1.0f };

};