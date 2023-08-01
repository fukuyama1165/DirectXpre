#pragma once
#include <xaudio2.h>

//ファイル読み込みに必要らしい
#include <fstream>

//ComPtr用インクルード
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
	//同じやつ読み込み防ぎ用
	std::string filepath = "";
	AudioType type= AudioType::Wave;
};


//音声データ
struct SoundData:public AudioData
{

	//波形フォーマット
	WAVEFORMATEX wfex_{};

	//バッファの先頭アドレス
	std::vector<BYTE> buffer_;

	//バッファのサイズ
	uint32_t BufferSize_ = 0;

};

class XAudio
{

	//構造体
public:



private:

	//.wav ファイルを読み込むとき
	//バイナリで読み込むらしい
	//のでチャンク(データ単位?)ごとにデータが分けられるので
	//中身を分けられるらしい
	//

	//チャンクヘッター
	struct ChunkHeader
	{

		//チャンク毎のID
		char id_[4];

		//チャンクサイズ
		int32_t size_;

	};

	//RIFFヘッタチャンク
	struct RiffHeader
	{

		//RIFF
		ChunkHeader chunk_;

		//↓拡張子の確認してる?
		//WAVE
		char type_[4];

	};

	//FMTチャンク
	struct FormatChunk
	{

		//"fmt"
		ChunkHeader chunk_;

		//波形フォーマット
		WAVEFORMATEX fmt_;

	};




	//変数
public:

	

private:

	static Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;

	//サウンドデータの連想配列
	std::map<std::string, std::shared_ptr<SoundData>> soundDatas_;

	//shared_ptrについて
	//unique_ptrと違い所有者が一人ではないやつ
	// 同じリソースをみんなで使ってくれる
	// リソースを使っているやつが誰もいなくなるとdeleteしてくれる
	//

	IXAudio2MasteringVoice* masterVoice_ = nullptr;

	HRESULT result_=S_OK;

	//人のコードを参考にした
	struct PlayingInfo {
		std::string handle;
		IXAudio2SourceVoice* pSource;
	};
	std::vector<PlayingInfo> playingList;

	//関数
public:

	static XAudio* GetInstance() {
		static XAudio instance;
		return &instance;
	}

	void Init();

	static std::string SoundLoadWave(const char* filename,std::string handle="");

	//指定したハンドルの音をだす
	static void PlaySoundData(const std::string handle, const float& volume = 1.0f, const bool loop = false);

	//指定したハンドルの音を止める
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