#include "ExplosionObjManager.h"
#include "ModelManager.h"
#include "XAudio.h"

ExplosionObjManager* ExplosionObjManager::GetInstance()
{
	static ExplosionObjManager instance;

	if (instance.model_ == nullptr)
	{
		instance.Init();
	}

	return &instance;
}

ExplosionObjManager::~ExplosionObjManager()
{

}

void ExplosionObjManager::PopExplosionObj(Vector3 pos, int32_t explosioneventNum, Vector3 size, Vector3 explosionSize, float explosionTime)
{
	std::unique_ptr<ExplosionObj> newExplosionObj = std::make_unique<ExplosionObj>();

	newExplosionObj->Init(pos, explosioneventNum, size, explosionSize, explosionTime);
	//爆発するオブジェクトを登録
	objs_.push_back(std::move(newExplosionObj));
}

void ExplosionObjManager::Init()
{
	ModelManager::GetInstance()->Load("testGLTFBall", "gltf", "whiteBall", "white1x1");

	explosionSound_ = XAudio::GetInstance()->SoundLoadWave("Resources/sound/enemydown.wav");

	model_ = ModelManager::GetInstance()->SearchModelData("whiteBall");
}

void ExplosionObjManager::UpDate(int32_t EventNum)
{
	//終わってるやつを消す
	objs_.remove_if([](std::unique_ptr<ExplosionObj>& obj)
	{
		return !obj->isAlive_;
	});

	for (std::unique_ptr<ExplosionObj>& obj : objs_)
	{
		obj->Update(explosionSound_,EventNum);
	}
}

void ExplosionObjManager::Draw()
{
	for (std::unique_ptr<ExplosionObj>& obj : objs_)
	{
		obj->Draw(model_);
	}
}