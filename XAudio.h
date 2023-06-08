#pragma once
#include <xaudio2.h>

#pragma comment(lib,"xaudio2.lib")

//ファイル読み込みに必要らしい
#include <fstream>

//ComPtr用インクルード
#include <wrl.h>

//音声データ
struct SoundData
{

	//波形フォーマット
	WAVEFORMATEX wfex_;

	//バッファの先頭アドレス
	BYTE* pBuffer_;

	//バッファのサイズ
	unsigned int BufferSize_;


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

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;

	IXAudio2MasteringVoice* masterVoice_;

	HRESULT result_;

	//関数
public:

	void Init();

	SoundData SoundLoadWave(const char* filename);

private:

};