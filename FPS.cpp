#include "FPS.h"


FPS::FPS()
{

}

FPS::~FPS()
{

}

void FPS::initialize()
{

	reference = std::chrono::steady_clock::now();

}

void FPS::update()
{

	const std::chrono::microseconds kMinTime(uint16_t(1000000.0f / 60.0f));

	const std::chrono::microseconds kMinCheckTime(uint16_t(1000000.0f / 65.0f));

	//���ݎ��Ԃ��擾����
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	//�O��L�^����o�ߎ��Ԃ��擾����
	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference);

	//1/60�b(���킸���ɒZ������)�o���Ă��Ȃ��ꍇ
	if (elapsed < kMinTime)
	{

		while (std::chrono::steady_clock::now()-reference<kMinTime)
		{
			//1�}�C�N���b�X���[�v
			std::this_thread::sleep_for(std::chrono::microseconds(1));

		}

	}

	//���݂̎��Ԃ��L�^����
	reference = std::chrono::steady_clock::now();

}