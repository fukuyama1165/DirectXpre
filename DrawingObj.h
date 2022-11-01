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

	

	//スケール変更行列
	void SetScale(Float3 scale);
	//角度変更行列
	void SetRotate(Float3 rotate);
	//平行移動行列
	void SetTrans(Float3 TransForm);

	void obj3DUpdate();

	

	void colorMap(float R,float G,float B);

	

private:

	

	

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
	//Object3D object3Ds[kObjectConst];

	

	

};
