#pragma once
#include <xaudio2.h>

#pragma comment(lib,"xaudio2.lib")

//�t�@�C���ǂݍ��݂ɕK�v�炵��
#include <fstream>

//ComPtr�p�C���N���[�h
#include <wrl.h>

#include <vector>
#include <memory>
#include <map>

#include <cstdint>


enum class AudioType {
	Wave, Other
};

//
struct AudioData {
	//������ǂݍ��ݖh���p
	std::string filepath;
	AudioType type;
};


//�����f�[�^
struct SoundData:public AudioData
{

	//�g�`�t�H�[�}�b�g
	WAVEFORMATEX wfex_;

	//�o�b�t�@�̐擪�A�h���X
	BYTE* pBuffer_;

	//�o�b�t�@�̃T�C�Y
	uint32_t BufferSize_;

	~SoundData() {
		if (pBuffer_ != nullptr) {
			delete[] pBuffer_;
		}
	}

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

	static Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;

	//�T�E���h�f�[�^�̘A�z�z��
	std::map<std::string, std::shared_ptr<SoundData>> soundDatas_;

	//shared_ptr�ɂ���
	//unique_ptr�ƈႢ���L�҂���l�ł͂Ȃ����
	// �������\�[�X���݂�ȂŎg���Ă����
	// ���\�[�X���g���Ă������N�����Ȃ��Ȃ��delete���Ă����
	//

	IXAudio2MasteringVoice* masterVoice_ = nullptr;

	HRESULT result_=S_OK;

	//�l�̃R�[�h���Q�l�ɂ���
	struct PlayingInfo {
		std::string handle;
		IXAudio2SourceVoice* pSource;
	};
	std::vector<PlayingInfo> playingList;

	//�֐�
public:

	static XAudio* GetInstance() {
		static XAudio instance;
		return &instance;
	}

	void Init();

	static std::string SoundLoadWave(const char* filename,std::string handle="");

	//��������
	static void PlaySoundData(const std::string handle, const float& volume = 1.0f, const bool loop = false);

	static void StapSoundData(const std::string handle);

	static void Final() {
		GetInstance()->Finalize();
	}

private:

	XAudio() = default;
	~XAudio();

	XAudio(const XAudio&) = delete;
	XAudio& operator=(const XAudio&) = delete;

	void Finalize();

};