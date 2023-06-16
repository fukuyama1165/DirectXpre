#include "FBXLoader.h"

AnimationModel::~AnimationModel()
{
}

bool AnimationModel::Load(std::string filename, std::string fileType)
{
	//インスタンスを取得
	Assimp::Importer importer;

	//ファイルパスをつくるよ～
	std::string baseDirectory = "Resources\\";
	std::string extend = "." + fileType;
	filename = baseDirectory + filename + "\\" + filename + extend;

	//指定したファイルの読み込みらしい
	const aiScene* scene = importer.ReadFile(filename,
		aiProcess_CalcTangentSpace |//インポートしたメッシュのタンジェントとかを計算してくれるらしい
		aiProcess_Triangulate |//三角面化してくれる
		aiProcess_JoinIdenticalVertices |//インポートしたメッシュの同一の頂点データセットを識別、結合する
		aiProcess_SortByPType |//2つ以上のプリミティブタイプを持つメッシュを均質なサブメッシュに分割する
		aiProcess_MakeLeftHanded |//左手座標系に
		aiProcess_FlipUVs |//UV座標を左上を原点にする
		aiProcess_FlipWindingOrder//CCWカリングに適した時計回りの面の順序にする
	);

	//読み込みに失敗したらエラーをはいて止まる?
	if (nullptr != scene)
	{
		//DoTheImportThing(importer.GetErrorString());
		return false;
	}

	//中身に触れるらしい

	//ボーンの情報の部分らしい
	//aiNode* a = scene->mRootNode;

	//データのよみこみ


	return false;
}