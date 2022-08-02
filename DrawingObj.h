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

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

//キーボードやコントローラーなどの入力するヘッダとライブラリのリンク
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")

#pragma comment(lib,"dxguid.lib")

//const float PI = 3.141592653589;

#include <DirectXTex.h>

#include "matrix4x4.h"
#include "Float3.h"
#include "Object3D.h"
#include "Texture.h"
#include "Float4.h"

//ComPtr用インクルード
#include <wrl.h>

class DrawingObj
{
public:
	//初期化をここで適当にエラーをはかないと思う値を入れている基本的に書き換えても問題ないはず(設定が破綻してなければ)
	DrawingObj(const float windowWidth, const float windowHeight);

	//座標を指定できるが引数大杉
	DrawingObj(const float windowWidth, const float windowHeight,XMFLOAT3 vertexPos1, XMFLOAT3 vertexPos2, XMFLOAT3 vertexPos3, XMFLOAT3 vertexPos4, XMFLOAT2 vertexUv1 = { 0.0f,1.0f }, XMFLOAT2 vertexUv2 = { 0.0f,0.0f }, XMFLOAT2 vertexUv3 = { 1.0f,1.0f }, XMFLOAT2 vertexUv4 = { 1.0f,0.0f });

	~DrawingObj();

	//描画初期化処理関数
	void basicInit(ID3D12Device* dev);//basicPS読み込み
	void colorChangeInit(ID3D12Device* dev);//colorChangePS読み込み

	//頂点データ構造体
	struct Vertex
	{
		XMFLOAT3 pos;//xyz座標
		XMFLOAT3 normal;//法線ベクトル
		XMFLOAT2 uv;//uv座標
	};

	//頂点バッファ生成
	void vertexBuffGeneration(ID3D12Device* dev);

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
	void constantBuffGeneration1(ID3D12Device* dev);

	//インデックスデータ関連(インデックスビューもここ)
	void indicesBuffGeneration(ID3D12Device* devconst);

	//画像イメージデータ
	void imageDataGeneration();

	//テクスチャバッファ
	void textureBuffGeneraion(ID3D12Device* dev);

	//シェーダリソースビュー
	void SRVGeneraion(ID3D12Device* dev);

	//GPUに頂点データを転送する関数
	void vertexMap();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	/// <param name="PipeLineRuleFlag">描画方法を変更するかどうか(現在はワイヤーフレームか塗りつぶし)trueは塗りつぶし</param>
	/// <param name="ChangeSquareFlag">三角形で描画するか四角形に描画する(trueは四角形)</param>
	void Draw(ID3D12GraphicsCommandList* cmdList,bool PipeLineRuleFlag, bool ChangeSquareFlag, bool ChangeTexure);


	//アフィン変換そのものの関数(2D)
	XMFLOAT3 Afin(XMFLOAT3 box, float moveX, float moveY, float rotate, float scaleX, float scaleY);

	//Objに対してアフィン変換をかける関数
	void ObjAfin(float moveX, float moveY, float rotate, float scaleX, float scaleY);

	//定数バッファを変更する関数
	void constBuffColorUpdata(float Red,float Green,float Blue);
	void constBuffPosMUpdata(float X,float Y,float Z);

	//ビュー変換行列更新
	//void matViewUpdata(Float3 eye, Float3 target, Float3 up);
	void matViewUpdata(Float3 eye, Float3 target, Float3 up);

	//定数バッファの行列を更新する関数
	void constTransformMatUpdata();
	void constTransformMatUpdata1();

	//ワールド座標更新
	void matWorldUpdata();
	void matWorldUpdata1();

	Matrix4x4 matScaleGeneration(Float3 scale);

	Matrix4x4 matRotateXGeneration(float rotateX);
	Matrix4x4 matRotateYGeneration(float rotateY);
	Matrix4x4 matRotateZGeneration(float rotateZ);

	Matrix4x4 matRotateGeneration(Float3 rotate);

	Matrix4x4 matMoveGeneration(Float3 translation);

	Matrix4x4 matViewGeneration(Float3 eye, Float3 target, Float3 up);

	Matrix4x4 perspectiveProjectionGeneration(float FovAngleY, float NearZ, float FarZ);

	//スケール変更行列
	void SetScale(Float3 scale);
	//角度変更行列
	void SetRotate(Float3 rotate);
	//平行移動行列
	void SetTrans(Float3 TransForm);

	void obj3DUpdate();

	/// <summary>
	/// sinとcosを返す関数
	/// </summary>
	/// <param name="Sin">sinの値が入る引数</param>
	/// <param name="Cos">cosの値が入る引数</param>
	/// <param name="angle">Θの値</param>
	void sinCos(float& Sin, float& Cos, float angle);

	Float3 float3Dat(Float3 A, Float3 B);

	void colorMap(float R,float G,float B);

	void deleteTexture();

private:

	//画面サイズ
	float Win_width;
	float Win_height;

	//頂点データ(増やしたいならここも増やしておく)
	Vertex vertices[24];

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

	//定数バッファ用データ構造体(3D変換行列)
	struct ConstBufferDataTransform
	{
		XMMATRIX mat;//3D変換行列
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
	unsigned short indices[36];
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

	Matrix4x4 matWorld;

	Matrix4x4 matScale;
	Matrix4x4 matRotate;
	Matrix4x4 matTrans;

	Float3 Scale_;
	Float3 Rotate_;
	Float3 Trans_;

	////透視投影行列
	//XMMATRIX matProjection;

	////ビュー変換行列
	//XMMATRIX matView;
	//XMFLOAT3 eye_;//視点座標
	//XMFLOAT3 target_;//注視点座標
	//XMFLOAT3 up_;//上方向ベクトル

	//XMMATRIX matWorld;
	//XMMATRIX matWorld1;

	//XMMATRIX matScale;
	//XMMATRIX matRotate;
	//XMMATRIX matTrans;

	//XMFLOAT3 Scale_;
	//XMFLOAT3 Rotate_;
	//XMFLOAT3 Trans_;

	//3Dオブジェクトの数
	const static size_t kObjectConst = 50;
	
	

	//3Dオブジェクトの配列
	Object3D object3Ds[kObjectConst];

	static Texture* texture;

	

};
