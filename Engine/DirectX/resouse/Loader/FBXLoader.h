#pragma once

//D3Dコンパイラ
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include<d3d12.h>
#pragma comment(lib,"d3d12.lib")

#include <vector>
#include <memory>

#include "DirectXInit.h"

#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "matrix4x4.h"
#include "Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <unordered_map>

#include "Model.h"

//人のコード見て作った

//部位のtransformだと思う
struct NodeUnit
{
	std::string name_;
	std::vector<Vector3> position_;
	std::vector<double> positionTime_;
	std::vector<Vector3> rotation_;
	std::vector<double> totationTime_;
	std::vector<Vector3> scale_;
	std::vector<double> scaleTime_;

};

//アニメーションの動きの為の構造体(進捗とか名前とか)
struct Animation
{

	std::string name_;
	double duration_;//間隔
	double ticksPerSecond_;//経過?時間
	std::vector< NodeUnit> channels_;//たぶん部位単位の位置を持ってる

};

//一単位だと思う(頂点みたいの？)
struct Node
{
	//名前(たぶん動かす際にこの部分のやつ動かすために必要になる？)
	std::string name_;

	//モデルって書いてあるけど単位的にメッシュになる
	std::vector<std::unique_ptr<Model>> meshes_;

	//ここから単体の行列やローカル座標

	//たぶんXMVECTORでしか受け取れない？
	//大きさ
	DirectX::XMVECTOR scale_ = { 1,1,1,0 };
	//角度
	DirectX::XMVECTOR rotation_ = { 0,0,0,0 };
	//位置
	DirectX::XMVECTOR translation_ = { 0,0,0,1 };

	//ローカル座標行列
	DirectX::XMMATRIX transform_ = DirectX::XMMatrixIdentity();

	//グローバル(ワールド)座標行列?
	DirectX::XMMATRIX globalTransform_ = DirectX::XMMatrixIdentity();

	//グローバル(ワールド)座標の逆行列
	DirectX::XMMATRIX globalInverseTransform_ = DirectX::XMMatrixIdentity();

	//親のアニメーション行列?
	DirectX::XMMATRIX animaetionParentMat_ = DirectX::XMMatrixIdentity();

	//親オブジェクト
	Node* parent_ = nullptr;

};

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

//これをobject3Dに持たせることで位置を移動させたりする
class AnimationModel
{

public:
	//textureのインスタンスをもらってくる
	DirectXInit* directXinit_ = DirectXInit::GetInstance();
	Texture* texture_ = Texture::GetInstance();

	//頂点位置群？
	std::vector<std::unique_ptr<Node>> nodes_;
	std::vector<std::unique_ptr<Animation>> animations_;
	std::vector<Bone> bones;

	//デストラクタ
	~AnimationModel();

	//ファイル名
	std::string filename_;

	//aisceneを持ってくる
	//読み込み
	bool Load(std::string filename, std::string fileType);

	/// <summary>
	/// aisceneに入ってるノードの情報をコピーする
	/// </summary>
	/// <param name="node">ノード(追加したいやつ)</param>
	/// <param name="scene">全体(どの位置にいるか判断するため?)</param>
	/// <param name="targetParent">親子関係があるなら入れる</param>
	void CopyNodeMesh(const aiNode* node,const aiScene* scene,const Node* targetParent = nullptr);

	void MeshAssignment(const aiMesh* mesh,const aiScene* scene)

};