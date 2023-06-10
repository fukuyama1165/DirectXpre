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
	WAVEFORMATEX wfex_;

	//�o�b�t�@�̐擪�A�h���X
	BYTE* pBuffer_;

	//�o�b�t�@�̃T�C�Y
	unsigned int BufferSize_;


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
		char id_[4];

		//�`�����N�T�C�Y
		int32_t size_;

	};

	//RIFF�w�b�^�`�����N
	struct RiffHeader
	{

		//RIFF
		ChunkHeader chunk_;

		//���g���q�̊m�F���Ă�?
		//WAVE
		char type_[4];

	};

	//FMT�`�����N
	struct FormatChunk
	{

		//"fmt"
		ChunkHeader chunk_;

		//�g�`�t�H�[�}�b�g
		WAVEFORMATEX fmt_;

	};




	//�ϐ�
public:

private:

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;

	IXAudio2MasteringVoice* masterVoice_;

	HRESULT result_;

	//�֐�
public:

	void Init();

	SoundData SoundLoadWave(const char* filename);

private:

};