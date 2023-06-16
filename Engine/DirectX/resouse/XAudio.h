#pragma once
#include <xaudio2.h>

#pragma comment(lib,"xaudio2.lib")

//ファイル読み込みに必要らしい
#include <fstream>

//ComPtr用インクルード
#include <wrl.h>

#include <vector>
#include <memory>

#include <cstdint>

//音声データ
struct SoundData
{

	//波形フォーマット
	WAVEFORMATEX wfex_;

	//バッファの先頭アドレス
	BYTE* pBuffer_;

	//バッファのサイズ
	uint32_t BufferSize_;


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

	static Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;

private:

	

	IXAudio2MasteringVoice* masterVoice_;

	HRESULT result_;

	//関数
public:

	void Init();

	static SoundData SoundLoadWave(const char* filename);

	//渡したサウンドデータの中身を無くす
	static void deleteSound(SoundData* soundData);

	//音をだす
	static void PlaySoundData(const SoundData& soundData);

	static void StapSoundData(const SoundData& soundData);

private:

};