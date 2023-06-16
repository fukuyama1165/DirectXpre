#include "XAudio.h"

//asssert使うだけならこれ
#include <cassert>

Microsoft::WRL::ComPtr<IXAudio2> XAudio::xAudio2_ = nullptr;

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

SoundData XAudio::SoundLoadWave(const char* filename)
{

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

	////JUNKチャンクを検出した場合
	//if (strncmp(data.id_, "JUNK", 4) == 0)
	//{

	//	//JUNKチャンクとはデータの開始位置をきりよく配置するためのダミーデータらしい

	//	//読み取り位置をJUNKチャンクの終わりまで進める
	//	file.seekg(data.size_, std::ios_base::cur);

	//	//再読み込み
	//	file.read((char*)&data, sizeof(data));

	//}

	//if (strncmp(data.id_, "LIST", 4) == 0)
	//{
	//	//再読み込み
	//	file.read((char*)&data, sizeof(data));
	//}
	//if (strncmp(data.id_, "INFO", 4) == 0)
	//{
	//	//再読み込み
	//	file.read((char*)&data, sizeof(data));
	//}


	////dataかどうか判断
	//if (strncmp(data.id_, "data", 4) != 0)
	//{
	//	assert(0);
	//}

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

	SoundData soundData = {};

	soundData.wfex_ = format.fmt_;
	soundData.pBuffer_ = reinterpret_cast<BYTE*>(pBuffer);
	soundData.BufferSize_ = data.size_;

	return soundData;

}

void XAudio::deleteSound(SoundData* soundData)
{
	delete[] soundData->pBuffer_;

	soundData->BufferSize_ = uint32_t(0);
	soundData->pBuffer_ = 0;
	soundData->wfex_ = {};

}

void XAudio::PlaySoundData(const SoundData& soundData)
{
	HRESULT result;

	//波形フォーマットをもとにsourceVoiceのせいせい
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex_);
	assert(SUCCEEDED(result));

	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer_;
	buf.AudioBytes = soundData.BufferSize_;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	//波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
	result = pSourceVoice->Stop();


}

void XAudio::StapSoundData(const SoundData& soundData)
{
	soundData;
}