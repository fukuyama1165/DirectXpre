#include "XAudio.h"

//asssert�g�������Ȃ炱��
#include <cassert>

void XAudio::Init()
{

	//xaudio2�G���W���̃C���X�^���X�����
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	assert(SUCCEEDED(result));

	//masterVoice�ɂ���
	// �S�ẴI�[�f�B�I�̈���
	// ����ŉ���炷�Ȃ炱����g���炵��
	// ����͏�̃C���X�^���X���������ƈꏏ�ɏ�����炵���̂�delete���Ȃ��悤��
	//�}�X�^�[�{�C�X�𐶐�
	result = xAudio2->CreateMasteringVoice(&masterVoice);

}

SoundData XAudio::SoundLoadWave(const char* filename)
{

	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;

	//.wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(filename, std::ios_base::binary);

	//�t�@�C���I�[�v�����s�����o����
	assert(file.is_open());

	//RIFF�w�b�^�[�̓ǂݍ���
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	//�t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}

	//�^�C�v��WAVE���`�F�b�N
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	//format�`�����N�̓ǂݍ���
	FormatChunk format = {};

	//�w�b�^�[�̕���������ǂݍ����id�Ȃǂ��擾����
	file.read((char*)&format, sizeof(ChunkHeader));

	//id��fmt���`�F�b�N
	if (strncmp(format.chunk.id, "fmt", 4) != 0)
	{
		assert(0);
	}

	//���Ȃ��Ȃ�S���ǂݍ���
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	//Data�`�����N�̓ǂݍ���
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	//JUNK�`�����N�����o�����ꍇ
	if (strncmp(data.id, "JUNK", 4) == 0)
	{

		//JUNK�`�����N�Ƃ̓f�[�^�̊J�n�ʒu������悭�z�u���邽�߂̃_�~�[�f�[�^�炵��

		//�ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, std::ios_base::cur);

		//�ēǂݍ���
		file.read((char*)&data, sizeof(data));

	}

	//data���ǂ������f
	if (strncmp(data.id, "data", 4) != 0)
	{
		assert(0);
	}

	//Data�`�����N�̃f�[�^��(�g�`�f�[�^)�̓ǂݍ���
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//wave�t�@�C�������
	file.close();

	SoundData a;

	return a;

}