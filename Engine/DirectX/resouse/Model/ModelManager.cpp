#include "ModelManager.h"

std::string ModelManager::Load(std::string filename, std::string fileType, std::string handle, std::string materialName, std::string materialType)
{

	auto itr = std::find_if(models_.begin(), models_.end(), [&](const std::pair<std::string, ModelData>& p) {
		return p.second.fileName == filename and p.second.materialName == materialName;//条件
		});
	//見つかったらそれを返す
	if (itr != models_.end()) {
		return itr->first;
	}

	std::shared_ptr<AnimationModel> newModel = std::make_shared<AnimationModel>();

	newModel->Load(filename, fileType, materialName, materialType);

	models_[handle].model = std::move(newModel);
	models_[handle].fileName = filename;
	models_[handle].materialName = materialName;

	return handle;

}


AnimationModel* ModelManager::SearchModelData(std::string handle)
{

	//探すよ
	auto itr = std::find_if(models_.begin(), models_.end(), [&](const std::pair<std::string, ModelData>& p) {
		return p.first == handle;//条件
		});
	//見つかったらそれを返す
	if (itr != models_.end()) {
		return models_[handle].model.get();
	}

	//ないもの探そうとしてるやつは白いボックスを返すよ
	std::string whiteBox = "whiteBox";

	return models_[whiteBox].model.get();

}


void ModelManager::RegisterModel(std::string handle, std::shared_ptr<AnimationModel> model)
{
	//なかみないなら返すよ
	if (model == nullptr)return;

	models_[handle].model = std::move(model);
}

void ModelManager::eraseModel(std::string handle)
{

	//探すよ
	auto itr = std::find_if(models_.begin(), models_.end(), [&](const std::pair<std::string, ModelData>& p) {
		return p.first == handle;//条件
		});
	//見つかったらそれを消す
	if (itr != models_.end()) {
		models_.erase(handle);
	}

	//ないなら何もない
}