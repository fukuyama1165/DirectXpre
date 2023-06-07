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
	std::string name;
	std::vector<Vector3> position;
	std::vector<double> positionTime;
	std::vector<Vector3> rotation;
	std::vector<double> totationTime;
	std::vector<Vector3> scale;
	std::vector<double> scaleTime;

};

//アニメーションの動きの為の構造体(進捗とか名前とか)
struct Animation
{

	std::string name;
	double duration;//間隔
	double ticksPerSecond;//経過?時間
	std::vector< NodeUnit> channels;//たぶん部位単位の位置を持ってる

};

//一単位だと思う(頂点みたいの？)
struct Node
{
	//名前(たぶん動かす際にこの部分のやつ動かすために必要になる？)
	std::string name;

	//モデルって書いてあるけど単位的にメッシュになる
	std::vector<std::unique_ptr<Model>> meshes;

	//ここから単体の行列やローカル座標

	//たぶんXMVECTORでしか受け取れない？
	//大きさ
	DirectX::XMVECTOR scale = { 1,1,1,0 };
	//角度
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	//位置
	DirectX::XMVECTOR translation = { 0,0,0,1 };

	//ローカル座標行列
	DirectX::XMMATRIX transform = DirectX::XMMatrixIdentity();

	//グローバル(ワールド)座標行列?
	DirectX::XMMATRIX globalTransform = DirectX::XMMatrixIdentity();

	//グローバル(ワールド)座標の逆行列
	DirectX::XMMATRIX globalInverseTransform = DirectX::XMMatrixIdentity();

	//親のアニメーション行列?
	DirectX::XMMATRIX animaetionParentMat = DirectX::XMMatrixIdentity();

	//親オブジェクト
	Node* parent = nullptr;

};

//一か所に持てる最大ボーン数?
static const unsigned short int NUM_BONES_PER_VERTEX = 4;

//最大ボーン数
static const unsigned short int MAX_BONE = 128;

//頂点データ
struct AnimationVertex
{

	Vector3 pos;
	Vector3 normal;
	Vector2 uv;
	unsigned int ids[NUM_BONES_PER_VERTEX] = {};
	float weights[NUM_BONES_PER_VERTEX] = {};

};

//定数バッファ用
struct ConstBuffSkin
{
	Matrix4x4 boneMats[MAX_BONE] = {};
};

//ウエイトの情報
struct SetWeight
{
	unsigned int id;
	float weight;
};

//ボーンの情報
struct Bone
{
	//どこのやつか
	std::string name;

	//初期位置
	Matrix4x4 offsetMatrix;

	//最終位置?
	Matrix4x4 finalMatrix;

};

//これをobject3Dに持たせることで位置を移動させたりする
class AnimationModel
{

public:
	//textureのインスタンスをもらってくる
	DirectXInit* directXinit = DirectXInit::GetInstance();
	Texture* texture = Texture::GetInstance(directXinit->Getdev().Get());

	//頂点位置群？
	std::vector<std::unique_ptr<Node>> nodes;
	std::vector<std::unique_ptr<Animation>> animations;
	std::vector<Bone> bones;

	//デストラクタ
	~AnimationModel();

	//ファイル名
	std::string filename;

	//aisceneを持ってくる
	//読み込み
	bool Load(std::string filename, std::string fileType);

	/// <summary>
	/// aisceneに入ってるノードの情報をコピーする
	/// </summary>
	/// <param name="node">ノード(追加したいやつ)</param>
	/// <param name="scene">全体(どの位置にいるか判断するため?)</param>
	/// <param name="targetParent">親子関係があるなら入れる</param>
	void CopyNodeMesh(aiNode* node, aiScene* scene, Node* targetParent = nullptr);

	void MeshAssignment(aiMesh* mesh,const aiScene* scene,)

};