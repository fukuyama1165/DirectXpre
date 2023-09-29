#include "EventPointManager.h"
#include "ModelManager.h"
#include "Enemy.h"
#include "Easing.h"
#include "WinApp.h"


EventPointManager* EventPointManager::GetInstance()
{
	static EventPointManager instance;

	if (instance.eventModel_ == nullptr)
	{
		instance.Initlize();
	}

	return &instance;
}
EventPointManager::~EventPointManager()
{
}

void EventPointManager::LoadEventData(std::string fileName)
{

	eventSetings_.clear();

	//������p�ӂ��Ă����p�X���������Ċ��S�ɒʂ�p�X�ɂ���
	const std::string fullPath = SDefaultEventPath_ + fileName + SDefaultEventExtension_;

	//�t�@�C���X�g���[��
	std::ifstream file;

	// �t�@�C�����J��
	file.open(fullPath);

	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	//JSON�����񂩂�𓀂����f�[�^
	nlohmann::json deserialized;

	//��
	file >> deserialized;

	//�������C�x���g�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"�𕶎���Ƃ��Ď擾
	std::string name = deserialized["name"].get<std::string>();

	//���������ǂ����`�F�b�N
	assert(name.compare("event") == 0);

	//"events"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& events : deserialized["events"])
	{

		EventScanning(deserialized, events);
	}

	//return std::move(levelData);

	if (eventSetings_.empty())
	{
		eventSetings_.push_back(EventSeting());
	}

	eventPoint_ = EventPoint(eventSetings_[0]);

	eventAllEnd_ = false;
	
}

void EventPointManager::EventScanning(nlohmann::json deserialized, nlohmann::json& Event)
{


	//type���Ȃ���Ύ~�߂�
	assert(Event.contains("type"));
	//�^�C�v���擾
	std::string type = Event["type"].get<std::string>();



	//moveEvent�Ȃ�
	if (type.compare("moveEvent") == 0)
	{


		
		EventSeting eventData;

		eventData.eventType = EventType::moveEvent;

		//�ݒ�̃p�����[�^�ǂݍ���
		nlohmann::json& seting = Event["seting"];

		//�ړ�����ꏊ�ǂݍ���
		eventData.movePoint.x = (float)seting["movePoint"][0];
		eventData.movePoint.y = (float)seting["movePoint"][1];
		eventData.movePoint.z = (float)seting["movePoint"][2];

		//�X�s�[�h�̃Z�b�g
		eventData.moveSpeed = (float)seting["moveSpeed"];

		eventSetings_.push_back(eventData);


	}
	else if (type.compare("BattleEvent") == 0)
	{

		EventSeting eventData;

		//�ݒ�̃p�����[�^�ǂݍ���
		nlohmann::json& seting = Event["seting"];

		//�������Ɖ�ʓ��̍ő吔���Z�b�g
		eventData.enemyMaxSpawn = seting["enemyMaxSpawn"];
		eventData.enemyNum = seting["enemyNum"];

		//�C�x���g�̃^�C�v���Z�b�g
		eventData.eventType = BattleEvent;

		//�G�l�~�[�̐�������
		for (uint16_t i = 0; i < (uint16_t)seting["enemyNum"]; i++)
		{
			//�ݒ肳��ĂȂ�����݂悤�Ƃ����炻��������΂Ȃ��悤��
			if ((float)seting["spawnPoint"].size() <= i or
				(float)seting["spawnInterval"].size() <= i or
				(float)seting["enemyType"].size() <= i or
				(float)seting["enemySpeed"].size() <= i or
				(float)seting["enemyMovePos"] <= i) continue;


			//�N���ꏊ���Z�b�g
			eventData.enemySpawnPos.push_back({ (float)seting["spawnPoint"][i][0],(float)seting["spawnPoint"][i][1] ,(float)seting["spawnPoint"][i][2] });

			//�N���Ԋu���Z�b�g
			eventData.enemySpawnInterval.push_back((float)seting["spawnInterval"][i]);

			//�G�l�~�[�̎�ނ��Z�b�g
			eventData.enemyTypes.push_back(seting["enemyType"][i].get<std::string>());

			//�G�l�~�[�������ꍇ�����Ƃ��̑��x���Z�b�g
			eventData.enemyMoveSpeed.push_back((float)seting["enemySpeed"][i]);

			//�G�l�~�[�������ꍇ�̓����ʒu
			eventData.enemyMovePos.push_back({ (float)seting["enemyMovePos"][i][0],(float)seting["enemyMovePos"][i][1] ,(float)seting["enemyMovePos"][i][2] });

		}

		eventSetings_.push_back(eventData);

	}




}

void EventPointManager::SetDebugMoveEvent(Vector3 point1, Vector3 point2, Vector3 point3, Vector3 point4)
{
	EventSeting debugPoint;

	debugPoint.movePoint = point1;

	eventSetings_.push_back(debugPoint);

	debugPoint.movePoint = point2;

	eventSetings_.push_back(debugPoint);

	debugPoint.movePoint = point3;

	eventSetings_.push_back(debugPoint);

	debugPoint.movePoint = point4;

	eventSetings_.push_back(debugPoint);

	eventAllEnd_ = false;

}

void EventPointManager::SetDebug1MoveEvent(Vector3 point1, float Speed1)
{
	EventSeting debugPoint;

	debugPoint.movePoint = point1;

	debugPoint.moveSpeed = Speed1;

	eventSetings_.push_back(debugPoint);

	eventAllEnd_ = false;

}

void EventPointManager::SetDebugBattleEvent(Vector3 point1, float Speed1, float interval1, Vector3 point2, float Speed2, float interval2, Vector3 point3, float Speed3, float interval3, Vector3 point4, float Speed4)
{

	EventSeting debugPoint;

	debugPoint.enemyMaxSpawn = 5;
	debugPoint.enemyNum = 4;
	
	debugPoint.eventType = BattleEvent;

	debugPoint.enemySpawnPos.push_back(point1);
	debugPoint.enemySpawnPos.push_back(point2);
	debugPoint.enemySpawnPos.push_back(point3);
	debugPoint.enemySpawnPos.push_back(point4);

	debugPoint.enemySpawnInterval.push_back(0);
	debugPoint.enemySpawnInterval.push_back(interval1);
	debugPoint.enemySpawnInterval.push_back(interval2);
	debugPoint.enemySpawnInterval.push_back(interval3);

	debugPoint.enemyTypes.push_back(EnemyType::Attack);
	debugPoint.enemyTypes.push_back(EnemyType::moveOnly);
	debugPoint.enemyTypes.push_back(EnemyType::Attack);
	debugPoint.enemyTypes.push_back(EnemyType::moveAttack);

	debugPoint.enemyMoveSpeed.push_back(Speed1);
	debugPoint.enemyMoveSpeed.push_back(0.1f);
	debugPoint.enemyMoveSpeed.push_back(Speed3);
	debugPoint.enemyMoveSpeed.push_back(Speed4);
	Speed2;

	debugPoint.enemyMovePos.push_back({ 10, 10, 50 });
	debugPoint.enemyMovePos.push_back({ -10, 10, 50 });
	debugPoint.enemyMovePos.push_back({ 0, 10, 50 });
	debugPoint.enemyMovePos.push_back({ 20, 10, 50 });

	
	eventSetings_.push_back(debugPoint);

	eventAllEnd_ = false;


}

void EventPointManager::Initlize()
{

	eventModel_ = ModelManager::GetInstance()->SearchModelData("whiteBox");

	eventPoint_.SetIsFinished(true);

	eventAllEnd_ = false;

	Texture::GetInstance()->loadTexture("Resources/NEXT.png", "NEXT");
	Texture::GetInstance()->loadTexture("Resources/wait.png", "WAIT");

	nextSprite_.initialize("NEXT");
	waitSprite_.initialize("WAIT");

	waitSprite_.pos_ = { (float)WinApp::SWindowWidth_ / 2,(float)WinApp::SWindowHeight_ / 2 };

}

void EventPointManager::Update()
{

	if (eventSetings_.empty())
	{
		return;
	}

	

	if (!nextTime_)
	{

		if (eventPoint_.GetIsFinished() and eventSetings_.size() > eventCount_)
		{
			beforeEventPointType_ = eventPoint_.GetEventType();
			eventPoint_ = EventPoint(eventSetings_[eventCount_]);
			eventCount_++;
			if (eventCount_ > 1 and (beforeEventPointType_!=eventPoint_.GetEventType() or (beforeEventPointType_==EventType::moveEvent and eventPoint_.GetEventType()==EventType::moveEvent)))
			{
				nextTime_ = true;
				nextMoveTime_ = 0;
				nextMoveTime2_ = 0;
			}
		}
		else if (eventPoint_.GetIsFinished())
		{
			eventAllEnd_ = true;
		}
	
	}

	if (!nextTime_ or eventPoint_.GetEventType() == EventType::moveEvent)
	{
		eventPoint_.Update();
	}

	if(nextTime_)
	{
		//nextSprite_.pos_ = easeOutQuad(Vector2{ -nextSprite_.GetTextureSize().x,WinApp::SWindowHeight_ / 2}, Vector2{WinApp::SWindowWidth_+ nextSprite_.GetTextureSize().x/2 ,WinApp::SWindowHeight_ / 2}, nextMoveTime_ / nextMoveMaxTime_);

		if (eventPoint_.GetEventType() == EventType::BattleEvent)
		{

			if (nextMoveTime_ < nextMoveMaxTime_)
			{
				nextSprite_.pos_ = easeOutQuad(Vector2{ -nextSprite_.GetTextureSize().x,(float)WinApp::SWindowHeight_ / 2 }, Vector2{ (float)WinApp::SWindowWidth_ / 2,(float)WinApp::SWindowHeight_ / 2 }, nextMoveTime_ / nextMoveMaxTime_);
				nextMoveTime_++;
			}
			else if (nextMoveTime2_ < nextMoveMaxTime2_)
			{
				nextSprite_.pos_ = easeInQuint(Vector2{ (float)WinApp::SWindowWidth_ / 2,(float)WinApp::SWindowHeight_ / 2 }, Vector2{ (float)WinApp::SWindowWidth_ + nextSprite_.GetTextureSize().x / 2,(float)WinApp::SWindowHeight_ / 2 }, nextMoveTime2_ / nextMoveMaxTime2_);
				nextMoveTime2_++;

			}
			else
			{
				nextTime_ = false;
			}

			nextSprite_.Update();
		}
		else if(eventPoint_.GetEventType() == EventType::moveEvent)
		{
			if (!eventPoint_.GetIsFinished())
			{
				//float a = easeOutQuad(0.0f, 20.0f, nextMoveTime_ / nextMoveMaxTime_);

				waitSprite_.setColor({ 1,1,1,sinf(nextMoveTime_) });
				nextMoveTime_++;
			}
			else
			{
				nextTime_ = false;
			}

			waitSprite_.Update();
		}
	}

}

void EventPointManager::Draw()
{
	//eventPoint_.Draw(eventModel_);

	if (nextTime_ && eventPoint_.GetEventType() == EventType::BattleEvent)
	{
		nextSprite_.Draw();
	}
	else if(nextTime_ && eventPoint_.GetEventType() == EventType::moveEvent)
	{
		waitSprite_.Draw();
	}
}