#include "XAudio.h"

//asssert�g�������Ȃ炱��
#include <cassert>

Microsoft::WRL::ComPtr<IXAudio2> XAudio::xAudio2_ = nullptr;

XAudio::~XAudio()
{

}

void XAudio::Init()
{

	//xaudio2�G���W���̃C���X�^���X�����
	result_ = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);

	assert(SUCCEEDED(result_));

	//masterVoice�ɂ���
	// �S�ẴI�[�f�B�I�̈���
	// ����ŉ���炷�Ȃ炱����g���炵��
	// ����͏�̃C���X�^���X���������ƈꏏ�ɏ�����炵���̂�delete���Ȃ��悤��
	//�}�X�^�[�{�C�X�𐶐�
	result_ = xAudio2_->CreateMasteringVoice(&masterVoice_);

}

std::string XAudio::SoundLoadWave(const char* filename, std::string handle)
{

	//���ǂݍ��񂾂��Ƃ�����t�@�C���͂��̂܂ܕԂ�
	auto itr = std::find_if(GetInstance()->soundDatas_.begin(), GetInstance()->soundDatas_.end(), [&](const std::pair<std::string, std::shared_ptr<AudioData>>& p) {
		return p.second->filepath == filename;//����
		});
	//���������炻���Ԃ�
	if (itr != GetInstance()->soundDatas_.end()) {
		return itr->first;
	}

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
	if (strncmp(riff.chunk_.id_, "RIFF", 4) != 0)
	{
		assert(0);
	}

	//�^�C�v��WAVE���`�F�b�N
	if (strncmp(riff.type_, "WAVE", 4) != 0)
	{
		assert(0);
	}

	//format�`�����N�̓ǂݍ���
	FormatChunk format = {};

	//�w�b�^�[�̕���������ǂݍ����id�Ȃǂ��擾����
	file.read((char*)&format, sizeof(ChunkHeader));

	//id��fmt���`�F�b�N
	if (strncmp(format.chunk_.id_, "fmt ", 4) != 0)
	{
		assert(0);
	}

	//���Ȃ��Ȃ�S���ǂݍ���
	assert(format.chunk_.size_ <= sizeof(format.fmt_));
	file.read((char*)&format.fmt_, format.chunk_.size_);

	//Data�`�����N�̓ǂݍ���
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	//data�̂Ƃ��܂Ŕ�΂���
	while (!file.fail() && strncmp(data.id_, "data", 4) != 0) {
		file.seekg(data.size_, std::ios_base::cur);
		file.read((char*)&data, sizeof(data));
	}

	if (file.fail()) {
		assert(0);
	}

	//Data�`�����N�̃f�[�^��(�g�`�f�[�^)�̓ǂݍ���
	char* pBuffer = new char[data.size_];
	file.read(pBuffer, data.size_);

	//wave�t�@�C�������
	file.close();

	std::shared_ptr<SoundData> soundData = std::make_shared<SoundData>();

	soundData->filepath = filename;
	soundData->wfex_ = format.fmt_;
	soundData->pBuffer_ = reinterpret_cast<BYTE*>(pBuffer);
	soundData->BufferSize_ = data.size_;
	

	if (handle.empty())
	{
		handle = filename;
	}

	GetInstance()->soundDatas_[handle] = soundData;

	return handle;

}

void XAudio::PlaySoundData(const std::string handle, const float& volume, const bool loop)
{
	HRESULT result;
	XAudio* instance = GetInstance();

	//�g�`�t�H�[�}�b�g�����Ƃ�sourceVoice�𐶐�

	if (instance->soundDatas_.find(handle) == instance->soundDatas_.end()) {
		return;
	}

	std::shared_ptr<SoundData> data = instance->soundDatas_[handle];

	if (data->type == AudioType::Wave)
	{
		std::shared_ptr<SoundData> waveData = std::static_pointer_cast<SoundData>(data);

		IXAudio2SourceVoice* pSourceVoice = nullptr;
		result = xAudio2_->CreateSourceVoice(&pSourceVoice, &waveData->wfex_);
		assert(SUCCEEDED(result));

		//�Đ�����g�`�f�[�^�̐ݒ�
		XAUDIO2_BUFFER buf{};
		buf.pAudioData = waveData->pBuffer_;
		buf.AudioBytes = waveData->BufferSize_;
		buf.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;
		buf.Flags = XAUDIO2_END_OF_STREAM;

		//�g�`�f�[�^�̍Đ�
		result = pSourceVoice->SubmitSourceBuffer(&buf);
		assert(SUCCEEDED(result));
		result = pSourceVoice->SetVolume(volume);
		assert(SUCCEEDED(result));
		result = pSourceVoice->Start();
		assert(SUCCEEDED(result));

		if (loop)
		{
			instance->playingList.push_back({ handle,pSourceVoice });
		}

	}
	
	


}

void XAudio::StapSoundData(const std::string handle)
{
	XAudio* instance = GetInstance();
	for (auto itr = instance->playingList.begin(); itr != instance->playingList.end();) {
		PlayingInfo info = *itr;
		if (info.handle == handle) {
			info.pSource->Stop();
			info.pSource->DestroyVoice();
			itr = instance->playingList.erase(itr);
			continue;
		}
		itr++;
	}
}

void XAudio::Finalize()
{
	masterVoice_->DestroyVoice();
	xAudio2_.Reset();
	soundDatas_.clear();
}