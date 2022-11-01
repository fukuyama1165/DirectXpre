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

#include "matrix4x4.h"
#include "Float3.h"

//ComPtr用インクルード
#include <wrl.h>

#include <DirectXTex.h>

#include "Texture.h"
#include "Float4.h"


class Object3D
{
public:
	Object3D();
	~Object3D();

	void Init(ID3D12Device* dev);

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList, bool PipeLineRuleFlag, bool ChangeSquareFlag, bool ChangeTexure);

	void constTransformMatUpdata(Matrix4x4 matView, Matrix4x4 matProjection);

	void SetScale(Float3 scale);
	void SetRotate(Float3 rotate);
	void SetPos(Float3 pos);
	void SetParent(Object3D* parent);


	Matrix4x4 matScaleGeneration(Float3 scale);

	Matrix4x4 matRotateXGeneration(float rotateX);
	Matrix4x4 matRotateYGeneration(float rotateY);
	Matrix4x4 matRotateZGeneration(float rotateZ);

	Matrix4x4 matRotateGeneration(Float3 rotate);

	Matrix4x4 matMoveGeneration(Float3 translation);

	Float3 GetScale();
	Float3 GetRotate();
	Float3 GetPos();

	Matrix4x4 GetWorldMat();

	Object3D* GetParent();

	//描画初期化処理関数
	void basicInit(ID3D12Device* dev, float WinWidth, float WinHeight);//basicPS読み込み
	void colorChangeInit(ID3D12Device* dev, float WinWidth, float WinHeight);//colorChangePS読み込み
	void objDrawInit(ID3D12Device* dev, const char filename[]);//.objを読み込んで描画

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
	void objVertexBuffGeneration(ID3D12Device* dev, const char filename[]);

	//頂点シェーダの読み込みとコンパイル
	void vertexShaderGeneration();//basicVS読み込み
	void vertexShaderGeneration2();//vertexMoveVS読み込み

	//ピクセルシェーダの読み込みとコンパイル
	void pixelShaderGeneration();//basicPS読み込み
	void pixelShaderGeneration2();//colorChangePS読み込み

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

	Matrix4x4 matViewGeneration(Float3 eye, Float3 target, Float3 up);

	Matrix4x4 perspectiveProjectionGeneration(float FovAngleY, float NearZ, float FarZ);

	/// <summary>
	/// sinとcosを返す関数
	/// </summary>
	/// <param name="Sin">sinの値が入る引数</param>
	/// <param name="Cos">cosの値が入る引数</param>
	/// <param name="angle">Θの値</param>
	void sinCos(float& Sin, float& Cos, float angle);

	Float3 float3Dat(Float3 A, Float3 B);

	void deleteTexture();

	void matViewUpdata(Float3 eye, Float3 target, Float3 up);

private:

	//画面サイズ
	float Win_width;
	float Win_height;

	//定数バッファ用データ構造体(3D変換行列)
	struct ConstBufferDataTransform
	{
		Matrix4x4 mat;//3D変換行列
	};

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;

	ConstBufferDataTransform* constMapTransform = nullptr;

	Float3 Scale_;
	Float3 Rotate_;
	Float3 Trans_;

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
	D3D12_INPUT_ELEMENT_DESC inputLayout[3];

	//グラフィックスパイプラインの各ステージの設定をする構造体を用意
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline2{};

	//デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	//ルートパラメータ(定数バッファの数が増えたら配列の要素数を増やして設定をしている関数の中身にも追加すること)
	D3D12_ROOT_PARAMETER rootParam[4] = {};

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

	//定数バッファ用データ構造体(マテリアル)
	struct ConstBufferDataMaterial
	{
		XMFLOAT4 color;//色(RGBA)
	};

	struct ConstBufferDataMaterial2
	{
		XMFLOAT4 posM;//位置移動に使う(XYZ);
	};


	//定数バッファそのもの
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial2 = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform0 = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform1 = nullptr;

	//マッピングするときのポインタ
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	ConstBufferDataMaterial2* constMapMaterial2 = nullptr;
	ConstBufferDataTransform* constMapTransform0 = nullptr;
	ConstBufferDataTransform* constMapTransform1 = nullptr;

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
	ScratchImage scratchImg{};

	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff = nullptr;

	//ID3D12DescriptorHeap* srvHeap = nullptr;

	//透視投影行列
	Matrix4x4 matProjection;

	//ビュー変換行列
	Matrix4x4 matView;
	Float3 eye_;//視点座標
	Float3 target_;//注視点座標
	Float3 up_;//上方向ベクトル

	static Texture* texture;

};


