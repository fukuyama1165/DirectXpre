#include "ModelManager.h"

std::string ModelManager::Load(std::string filename, std::string fileType, std::string handle, std::string materialName, std::string materialType)
{

	auto itr = std::find_if(models_.begin(), models_.end(), [&](const std::pair<std::string, ModelData>& p) {
		return p.second.fileName == filename and p.second.materialName == materialName;//ğŒ
		});
	//Œ©‚Â‚©‚Á‚½‚ç‚»‚ê‚ğ•Ô‚·
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

	//’T‚·‚æ
	auto itr = std::find_if(models_.begin(), models_.end(), [&](const std::pair<std::string, ModelData>& p) {
		return p.first == handle;//ğŒ
		});
	//Œ©‚Â‚©‚Á‚½‚ç‚»‚ê‚ğ•Ô‚·
	if (itr != models_.end()) {
		return models_[handle].model.get();
	}

	//‚È‚¢‚à‚Ì’T‚»‚¤‚Æ‚µ‚Ä‚é‚â‚Â‚Í”’‚¢ƒ{ƒbƒNƒX‚ğ•Ô‚·‚æ
	std::string whiteBox = "whiteBox";

	return models_[whiteBox].model.get();

}


void ModelManager::RegisterModel(std::string handle, std::shared_ptr<AnimationModel> model)
{
	//‚È‚©‚İ‚È‚¢‚È‚ç•Ô‚·‚æ
	if (model == nullptr)return;

	models_[handle].model = std::move(model);
}

void ModelManager::eraseModel(std::string handle)
{

	//’T‚·‚æ
	auto itr = std::find_if(models_.begin(), models_.end(), [&](const std::pair<std::string, ModelData>& p) {
		return p.first == handle;//ğŒ
		});
	//Œ©‚Â‚©‚Á‚½‚ç‚»‚ê‚ğÁ‚·
	if (itr != models_.end()) {
		models_.erase(handle);
	}

	//‚È‚¢‚È‚ç‰½‚à‚È‚¢
}