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
#include "Material.h"

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
#pragma warning(push)
#pragma warning(disable: 4324)
typedef struct Node
{
	//名前(たぶん動かす際にこの部分のやつ動かすために必要になる？)
	std::string name_;

	//メッシュ
	std::vector<std::unique_ptr<AnimationMesh>> meshes_;

	//ここから単体の行列やローカル座標

	//たぶんXMVECTORでしか受け取れない？
	//大きさ
	DirectX::XMVECTOR scale_ = { 1,1,1,0 };
	//角度
	DirectX::XMVECTOR rotation_ = { 0,0,0,0 };
	//位置
	DirectX::XMVECTOR translation_ = { 0,0,0,1 };

	//ローカル座標行列
	Matrix4x4 transform_ = Matrix4x4::Identity();

	//グローバル(ワールド)座標行列?
	Matrix4x4 globalTransform_ = Matrix4x4::Identity();

	//グローバル(ワールド)座標の逆行列
	Matrix4x4 globalInverseTransform_ = Matrix4x4::Identity();

	//親のアニメーション行列?
	Matrix4x4 animaetionParentMat_ = Matrix4x4::Identity();

	//親オブジェクト
	Node* parent_ = nullptr;

}Node;
#pragma warning(pop)




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
	std::string materialName_;

	//aisceneを持ってくる
	
	bool Load(std::string filename, std::string fileType, std::string materialName = "", std::string materialType = "png");

	void Draw()const;

	/// <summary>
	/// aisceneに入ってるノードの情報をコピーする
	/// </summary>
	/// <param name="node">ノード(追加したいやつ)</param>
	/// <param name="scene">全体(どの位置にいるか判断するため?)</param>
	/// <param name="targetParent">親子関係があるなら入れる</param>
	void CopyNodeMesh(const aiNode* node,const aiScene* scene, Node* targetParent = nullptr);

	/// <summary>
	/// メッシュの情報を入れる
	/// </summary>
	/// <param name="mesh">入れたいメッシュ</param>
	/// <param name="scene">読み込んだaiScene</param>
	/// <param name="model">保存する場所</param>
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, AnimationMesh& model);

	/// <summary>
	/// マテリアルにあるテクスチャを読み込むやつ
	/// </summary>
	/// <param name="material">該当するマテリアル</param>
	/// <param name="type">受け取りたいやつの条件?</param>
	/// <param name="model">保存する場所</param>
	void LoadMaterialTextures(aiMaterial* material, aiTextureType type, AnimationMesh& model);

	//void MeshAssignment(const aiMesh* mesh, const aiScene* scene);



};