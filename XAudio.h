#pragma once
#include <xaudio2.h>

#pragma comment(lib,"xaudio2.lib")

//�t�@�C���ǂݍ��݂ɕK�v�炵��
#include <fstream>

//ComPtr�p�C���N���[�h
#include <wrl.h>

//�����f�[�^
struct SoundData
{

	//�g�`�t�H�[�}�b�g
	WAVEFORMATEX wfex;

	//�o�b�t�@�̐擪�A�h���X
	BYTE* pBuffer;

	//�o�b�t�@�̃T�C�Y
	unsigned int BufferSize;


};

class XAudio
{

	//�\����
public:



private:

	//.wav �t�@�C����ǂݍ��ނƂ�
	//�o�C�i���œǂݍ��ނ炵��
	//�̂Ń`�����N(�f�[�^�P��?)���ƂɃf�[�^����������̂�
	//���g�𕪂�����炵��
	//

	//�`�����N�w�b�^�[
	struct ChunkHeader
	{

		//�`�����N����ID
		char id[4];

		//�`�����N�T�C�Y
		int32_t size;

	};

	//RIFF�w�b�^�`�����N
	struct RiffHeader
	{

		//RIFF
		ChunkHeader chunk;

		//���g���q�̊m�F���Ă�?
		//WAVE
		char type[4];

	};

	//FMT�`�����N
	struct FormatChunk
	{

		//"fmt"
		ChunkHeader chunk;

		//�g�`�t�H�[�}�b�g
		WAVEFORMATEX fmt;

	};




	//�ϐ�
public:

private:

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2;

	IXAudio2MasteringVoice* masterVoice;

	HRESULT result;

	//�֐�
public:

	void Init();

	SoundData SoundLoadWave(const char* filename);

private:

};