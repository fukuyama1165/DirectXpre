#pragma once

/**
 * @file Object3D.h
 * @brief 3Dのオブジェクト
 * @author フクヤマ
 * @date 2023_12/29
 */
#include <fstream>
#include <sstream>

//数学ライブラリ(最適化されすぎて使いにくいらしいのであとで自作しろって)
#include<DirectXMath.h>

#include "DirectXInit.h"

#include "matrix4x4.h"
#include "Vector3.h"
#include "Quaternion.h"


#include "Texture.h"
#include "Vector4.h"
#include "camera.h"

#include "Util.h"

#include <unordered_map>

#include "LightGroup.h"

#include "CollisionInfo.h"

#include "CollisionManager.h"

#include "FBXLoader.h"
#include "Shader.h"
#include "PipeLine.h"

enum BillboardMode
{
	nothingBillboard,//掛けない
	AllBillboard,//全ての方向
	YBillboard,//Y軸ビルボード

};

class Object3D
{
public:
	Object3D();
	~Object3D();

	//描画初期化処理関数
	void basicInit();//basicPS読み込み
	void colorChangeInit();//colorChangePS読み込み
	void objDrawInit(const std::string& directoryPath, const char filename[], bool smoothing = false);//.objを読み込んで描画
	void boarPolygonInit();//板ポリ用の初期化

	//FBX(gltf)用の初期化
	void FBXInit();

	void SetMatrix(const Matrix4x4& mat);

	void SetScale(const Vector3&  scale);
	void SetRotate(const Vector3& rotate);
	void SetPos(const Vector3& pos);
	void SetParent(Object3D* parent);

	void SetColor(Vector4 color) { color_ = color; };

	Vector4 GetColor() { return color_; };

	/// <summary>
	/// コライダーのセット
	/// </summary>
	/// <param name="Collider">コライダー</param>
	void SetCollider(BaseCollider* Collider);

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	void OnCollision(const CollisionInfo& info) { info; };

	Matrix4x4 GetWorldMat();

	Object3D* GetParent();

	Vector3 GetWorldPos();

	Vector3 GetScale();
	Vector3 GetRotate();
	Vector3 GetPos();

	void matWorldGeneration();

	//更新
	void Update();

	/// <summary>
	/// テクスチャを含めた描画
	/// </summary>
	/// <param name="ChangeTexure">loadTextureで帰ってきた値を入れると読み込んだ画像が出るはず</param>
	/// <param name="PipeLineRuleFlag">パイプラインステートを変化させるフラグ</param>
	/// <param name="ChangeSquareFlag">頂点３つだけ使って描画するように変化させるフラグ</param>
	void Draw(const std::string& ChangeTexure = "basketballMan", const bool& PipeLineRuleFlag = true, const bool& ChangeSquareFlag = true);

	//FBX((gltf)用の描画(これを使うにはFBXInitで初期化しておく必要がある)
	void FBXDraw(AnimationModel& model, const bool& PipeLineRuleFlag = true);

	std::string loadTexture(const std::string& filename,std::string handle);

	void deleteTexture();

	static void SetLight(LightGroup* light)
	{
		Object3D::SLightGroup_ = light;
	}

	void SetMaterialTiring(Vector2 tiring) { material_.material_.tile_ = tiring; };

private:	

	void constTransformMatUpdata(const Vector3& eye, const Matrix4x4& matView, const Matrix4x4& matProjection);	

	//ただ四角や三角形の為の頂点データ設定関数

	void basicVertexInit();
	void colorChangeVertexInit();

	//頂点データ構造体
	struct Vertex
	{
		DirectX::XMFLOAT3 pos_;//xyz座標
		DirectX::XMFLOAT3 normal_;//法線ベクトル
		DirectX::XMFLOAT2 uv_;//uv座標
	};

	//頂点バッファ生成
	void vertexBuffGeneration();
	void vertexBuffObjGeneration();
	void objVertexBuffGeneration(const std::string& directoryPath, const char filename[],bool smoothing);

	//頂点シェーダの読み込みとコンパイル
	void vertexShaderGeneration();//basicVS読み込み
	void vertexShaderGeneration2();//vertexMoveVS読み込み
	void vertexShaderGeneration3();//OBJVS読み込み
	void vertexShaderGeneration4();//Phong_toon_rim_VS読み込み

	void vertexShaderFBXGeneration();//OBJVS読み込み

	//ピクセルシェーダの読み込みとコンパイル
	void pixelShaderGeneration();//basicPS読み込み
	void pixelShaderGeneration2();//colorChangePS読み込み
	void pixelShaderGeneration3();//OBJPS読み込み
	void pixelShaderGeneration4();//Phong_toon_rim_PS読み込み

	void pixelShaderFBXGeneration();//OBJPS読み込み

	//頂点レイアウトの設定
	void vertexLayout();

	//デスクリプタレンジの設定
	void descriptorRangeGeneration();

	//ルートパラメータの設定(定数バッファとシェーダについて)
	void rootParamGeneration();

	//テクスチャサンプラーの設定
	void textureSamplerGeneration();

	//ルートシグネチャ
	void rootsignatureGeneration();

	//定数バッファ
	void constantBuffGeneration();
	void constantBuffFBXGeneration();

	//インデックスデータ関連(インデックスビューもここ)
	void indicesBuffGeneration();

	void loadMaterial(const std::string& filename, const std::string& directoryPath);
	
	/// <summary>
	/// マテリアルのテクスチャ読み込み
	/// </summary>
	/// <returns>成否</returns>
	bool LoadMaterialTexture(const std::string& filename, std::string handle);


public:

	Vector3 Scale_;
	Vector3 Rotate_;
	//クオータニオン使う場合のクオータニオンを入れる場所(これを使うなら必ずクオータニオンを使うフラグを立てること)
	Quaternion quaternionRot_;
	Vector3 pos_;

	int16_t billboardMode_ = BillboardMode::nothingBillboard;

	const char* name_ = nullptr;

	static LightGroup* SLightGroup_;

	Vector3 forward_ = { 0.0f,0.0f,1.0f };

	//自分で持っている変数のクオータニオンを使って行列計算するか(クオータニオンを直で使いたい場合)
	bool useQuaternion = false;

	Vector2 tiring_ = { 1,1 };
	
	Material material_;

	//ライトを使うか
	float useLight_ = 1;
private:
	


	//定数バッファ用データ構造体(3D変換行列)
	struct ConstBufferDataTransform
	{
		//Matrix4x4 mat;//3D変換行列
		Matrix4x4 viewProj_;//ビュープロジェクション行列
		Matrix4x4 world_;//ワールド行列
		Vector3 cameraPos_;//カメラ座標(ワールド座標)
		float useLight;//ライトを使うか
		Vector4 color;
	};

	struct ConstBufferDataB1
	{
		Vector3 ambient_;//アンビエント影響度
		float pad1_;//パディング
		Vector3 diffuse_;//ディフューズ影響度
		float pad2_;//パディング
		Vector3 specular_;//スペキュラー影響度
		float alpha_;//アルファ
		Vector2 tile;
		float pad3_;
		float pad4_;
	};

	Matrix4x4 matScale_;
	Matrix4x4 matRotate_;
	Matrix4x4 matTrans_;

	Matrix4x4 matWorld_;

	Object3D* parent_ = nullptr;

	Vector4 color_ = { 1,1,1,1 };

	//頂点データ(増やしたいならここも増やしておく)
	std::vector <Vertex> vertices_;

	//頂点データサイズ
	uint32_t sizeVB_ = 0;

	HRESULT result_ = S_OK;

	//頂点バッファ用変数
	D3D12_HEAP_PROPERTIES heapprop_{};//ヒープ設定
	D3D12_RESOURCE_DESC resDesc_{};//リソース設定
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	Vertex* vertMap_ = nullptr;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	//頂点シェーダオブジェクト
	Shader vs;
	//ピクセルシェーダオブジェクト
	Shader ps;

	//エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_ = nullptr;

	//頂点レイアウト(要素を増やすなら配列数を増やす)
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout_;

	//グラフィックスパイプラインの各ステージの設定をする構造体を用意
	PipeLine pipeline_;
	PipeLine gpipeline2_;

	//デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange_{};

	//ルートパラメータ(定数バッファの数が増えたら配列の要素数を増やして設定をしている関数の中身にも追加すること)
	std::vector<D3D12_ROOT_PARAMETER> rootParam_ = {};

	//テクスチャサンプラー
	D3D12_STATIC_SAMPLER_DESC sampleDesc_{};

	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature_;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp_{};
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc_{};

	//定数バッファ用のリソース設定関数
	D3D12_RESOURCE_DESC constBuffResourceGeneration(uint32_t size);

	//定数バッファそのもの

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;

	//マッピングするときのポインタ
	ConstBufferDataTransform* constMapTransform_ = nullptr;
	ConstBufferDataB1* constMapB1_ = nullptr;

	//インデックスデータ
	std::vector< unsigned short> indices_;
	//インデックスデータ全体のサイズ
	uint32_t sizeIB_ = 0;
	//インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_ = nullptr;
	//インデックスビュー
	D3D12_INDEX_BUFFER_VIEW ibView_{};
	
	//頂点法線スムージング用データ
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData_;	

	std::string materialTextureNum_ = "";

};