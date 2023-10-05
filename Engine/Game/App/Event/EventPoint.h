#pragma once
#include <cstdint>
#include "Vector3.h"
#include <vector>
#include "Object3D.h"

enum EventType
{
	moveEvent,
	BattleEvent
};

struct EventSeting
{
	uint16_t eventType = moveEvent;
	Vector3 movePoint = { 0,0,0 };
	Vector3 movePointRot = { 0,0,0 };
	float moveSpeed = 1;
	uint16_t enemyNum = 0;
	uint16_t enemyMaxSpawn = 0;
	std::vector<Vector3> enemySpawnPos;
	std::vector<Vector3> enemyMovePos;
	std::vector<float> enemyMoveSpeed;
	std::vector<float> enemySpawnInterval;
	std::vector<std::string> enemyTypes;

};

class EventPoint
{
public:
	EventPoint();
	EventPoint(EventSeting seting);
	~EventPoint();

	void Initlize();

	void Update();

	void Draw(AnimationModel* model);

	//���Ԃ̎擾
	float GetTimeCount() { return timeCount_; };

	//�I�������
	bool GetIsFinished() { return IsFinished_; };

	//�I���t���O��ݒ�
	void SetIsFinished(bool isFinished) { IsFinished_ = isFinished; };

	//�C�x���g�̃^�C�v���擾
	uint16_t GetEventType() { return seting_.eventType; };

	//�ړ��ʒu�擾
	Vector3 GetMovePoint() { return movePoint_; };

	//�C�x���g�̊p�x�擾(�o�g���C�x���g�̌����Ƃ�)
	Vector3 GetMovePointRot() { return movePointRot_; };

	//�C�x���g�̐ݒ�擾
	EventSeting GetEventSeting() { return seting_; };


private:

	//�ړ�����ʒu
	Vector3 movePoint_ = {};

	//�����p�x
	Vector3 movePointRot_ = {};

	//�҂�����
	float timeCount_ = 0;

	//�{�̂̈ʒu
	Object3D obj_;

	//�I�������
	bool IsFinished_ = false;

	//�G�̌����̃J�E���^�[
	uint32_t count_ = 0;

	//�C�x���g�̐ݒ�
	EventSeting seting_;

};

