#pragma once
#include "Model.h"
#include "FBXLoader.h"

class ModelManager
{
public:

	static ModelManager* GetInstance() {
		static ModelManager instance;
		if (instance.models_.empty())
		{
			instance.Load("testFBX", "gltf", "whiteBox", "white1x1");
		}

		return &instance;
	};
	
	/// <summary>
	/// モデルの登録
	/// </summary>
	/// <param name="filename">読み込むファイル</param>
	/// <param name="fileType">読み込むファイルタイプ</param>
	/// <param name="handle">呼び出すときのハンドル</param>
	/// <param name="materialName">マテリアルが読み込めなかった時用の画像の名前(gltfを読み込むときは必須、必ずモデルと同じ位置に置くこと!!!)</param>
	/// <param name="materialType">画像のファイルタイプ</param>
	/// <returns>ハンドル</returns>
	std::string Load(std::string filename, std::string fileType, std::string handle = "", std::string materialName = "", std::string materialType = "png");
	
	/// <summary>
	/// 登録されたモデルを探す(なければ白いボックス返す)
	/// </summary>
	/// <param name="handle">登録したときのハンドル</param>
	/// <returns>モデルのデータ</returns>
	AnimationModel* SearchModelData(std::string handle);

	/// <summary>
	/// もうすでに読み込んでしまったモデルの登録
	/// </summary>
	/// <param name="handle">呼び出し用のハンドル</param>
	/// <param name="model">もうすでにあるモデル</param>
	void RegisterModel(std::string handle, std::shared_ptr<AnimationModel> model);

	/// <summary>
	/// 指定した登録済みモデルの登録解除
	/// </summary>
	/// <param name="handle">ハンドル</param>
	void eraseModel(std::string handle);

private:
	ModelManager() = default;
	~ModelManager() {};

	ModelManager(const ModelManager&) = delete;
	ModelManager& operator=(const ModelManager&) = delete;


	//登録用の構造体
	struct ModelData
	{

		std::string fileName = "";
		std::string materialName = "";
		std::shared_ptr<AnimationModel> model;

	};


private:


	//モデルデータの連想配列
	std::map<std::string, ModelData> models_;

};