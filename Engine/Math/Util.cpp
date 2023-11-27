#include "Util.h"
#include <random>

//ラジアンから角度
float Util::RadianToAngle(float Radian)
{
	return Radian * (180 / PI);
}

//角度からラジアン
float Util::AngleToRadian(float Angle)
{
	return Angle * (PI / 180);
}

uint32_t Util::Rand(uint32_t min, uint32_t max)
{
	std::random_device seed;
	std::default_random_engine engine(seed());

	std::uniform_int_distribution<uint32_t> dest(min, max);

	return dest(engine);
}
uint16_t Util::Rand(uint16_t min, uint16_t max)
{
	std::random_device seed;
	std::default_random_engine engine(seed());

	std::uniform_int_distribution<uint16_t> dest(min, max);

	return dest(engine);
}

float Util::Rand(float min, float max)
{
	std::random_device seed;
	std::default_random_engine engine(seed());

	std::uniform_real_distribution<float> dest(min, max);

	return dest(engine);
}
double Util::Rand(double min, double max)
{
	std::random_device seed;
	std::default_random_engine engine(seed());

	std::uniform_real_distribution<double> dest(min, max);

	return dest(engine);
}

std::string Util::WStringToString(const std::wstring& wstr)
{
	// wstring → SJIS
	int32_t iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, wstr.c_str()
		, -1, (char*)NULL, 0, NULL, NULL);

	//変換用の入れ物を用意する
	std::vector<char> buff;
	//これにより1以下を無くすぜ
	buff.resize(max(1, iBufferSize));

	// wstring → SJIS
	WideCharToMultiByte(CP_OEMCP, 0, wstr.c_str(), -1, &buff[0]
		, iBufferSize, NULL, NULL);

	// stringの生成
	std::string oRet(&buff[0], &buff.back());

	

	// 変換結果を返す
	return oRet;
}

std::wstring Util::ConvertMultByteStringToWideString(const std::string& mstring)
{

	//文字列のサイズをワイド文字て取得
	int32_t wideStringSize = MultiByteToWideChar(CP_ACP, 0, mstring.c_str(), -1, nullptr, 0);

	//ワイド文字を作って大きさを変更
	std::wstring wstring;
	wstring.resize(wideStringSize);

	//ワイド文字に変換
	MultiByteToWideChar(CP_ACP, 0, mstring.c_str(), -1, &wstring[0], wideStringSize);

	return wstring;

}

std::string Util::SeparateFilePath(const std::string& filePath)
{
	size_t pos1;
	std::string exceptExt;
	std::string directoryPath;
	std::string fileName;

	//区切り文字'.'が出てくる一番最後の部分を検索
	pos1 = filePath.rfind('.');

	//検索がヒットしたら
	if (pos1 != std::wstring::npos)
	{
		//区切り文字の前までを抜き出す
		exceptExt = filePath.substr(0, pos1);

	}
	else
	{

		exceptExt = filePath;

	}

	//区切り文字'\\'が出てくる最後の部分を検索
	pos1 = exceptExt.rfind('\\');

	if (pos1 != std::wstring::npos)
	{

		//区切り文字の後ろをディレクトリパスとして保存
		directoryPath = exceptExt.substr(0, pos1 + 1);

		//区切り文字の後ろをファイル名として保存
		fileName = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return fileName;

	}


	//区切り文字'/'が出てくる最後の部分を検索
	pos1 = exceptExt.rfind('/');

	if (pos1 != std::wstring::npos)
	{

		//区切り文字の後ろをディレクトリパスとして保存
		directoryPath = exceptExt.substr(0, pos1 + 1);

		//区切り文字の後ろをファイル名として保存
		fileName = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return fileName;

	}


	//区切り文字が無いのでファイル名のみとして扱うよ
	directoryPath = "";
	fileName = exceptExt;
	return fileName;
}