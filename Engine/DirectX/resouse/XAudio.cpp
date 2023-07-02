#include "XAudio.h"

//asssert使うだけならこれ
#include <cassert>

Microsoft::WRL::ComPtr<IXAudio2> XAudio::xAudio2_ = nullptr;

XAudio::~XAudio()
{

}

void XAudio::Init()
{

	//xaudio2エンジンのインスタンスを作る
	result_ = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);

	assert(SUCCEEDED(result_));

	//masterVoiceについて
	// 全てのオーディオの宛先
	// これで音を鳴らすならこれを使うらしい
	// これは上のインスタンスを解放すると一緒に消えるらしいのでdeleteしないように
	//マスターボイスを生成
	result_ = xAudio2_->CreateMasteringVoice(&masterVoice_);

}

std::string XAudio::SoundLoadWave(const char* filename, std::string handle)
{

	//一回読み込んだことがあるファイルはそのまま返す
	auto itr = std::find_if(GetInstance()->soundDatas_.begin(), GetInstance()->soundDatas_.end(), [&](const std::pair<std::string, std::shared_ptr<AudioData>>& p) {
		return p.second->filepath == filename;//条件
		});
	//見つかったらそれを返す
	if (itr != GetInstance()->soundDatas_.end()) {
		return itr->first;
	}

	//ファイル入力ストリームのインスタンス
	std::ifstream file;

	//.wavファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);

	//ファイルオープン失敗を検出する
	assert(file.is_open());

	//RIFFヘッターの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	//ファイルがRIFFかチェック
	if (strncmp(riff.chunk_.id_, "RIFF", 4) != 0)
	{
		assert(0);
	}

	//タイプがWAVEかチェック
	if (strncmp(riff.type_, "WAVE", 4) != 0)
	{
		assert(0);
	}

	//formatチャンクの読み込み
	FormatChunk format = {};

	//ヘッターの部分だけを読み込んでidなどを取得する
	file.read((char*)&format, sizeof(ChunkHeader));

	//idがfmtかチェック
	if (strncmp(format.chunk_.id_, "fmt ", 4) != 0)
	{
		assert(0);
	}

	//問題ないなら全部読み込む
	assert(format.chunk_.size_ <= sizeof(format.fmt_));
	file.read((char*)&format.fmt_, format.chunk_.size_);

	//Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	//dataのとこまで飛ばす所
	while (!file.fail() && strncmp(data.id_, "data", 4) != 0) {
		file.seekg(data.size_, std::ios_base::cur);
		file.read((char*)&data, sizeof(data));
	}

	if (file.fail()) {
		assert(0);
	}

	//Dataチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[data.size_];
	file.read(pBuffer, data.size_);

	//waveファイルを閉じる
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

	//波形フォーマットをもとにsourceVoiceを生成

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

		//再生する波形データの設定
		XAUDIO2_BUFFER buf{};
		buf.pAudioData = waveData->pBuffer_;
		buf.AudioBytes = waveData->BufferSize_;
		buf.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;
		buf.Flags = XAUDIO2_END_OF_STREAM;

		//波形データの再生
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