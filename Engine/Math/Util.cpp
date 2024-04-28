#include "Util.h"
#include <random>
#include <imgui.h>

//ラジアンから角度
float DirectXpre::Util::RadianToAngle(float Radian)
{
	return Radian * (180 / PI);
}

//角度からラジアン
float DirectXpre::Util::AngleToRadian(float Angle)
{
	return Angle * (PI / 180);
}

uint32_t DirectXpre::Util::Rand(uint32_t min, uint32_t max)
{
	std::random_device seed;
	std::default_random_engine engine(seed());

	std::uniform_int_distribution<uint32_t> dest(min, max);

	return dest(engine);
}
uint16_t DirectXpre::Util::Rand(uint16_t min, uint16_t max)
{
	std::random_device seed;
	std::default_random_engine engine(seed());

	std::uniform_int_distribution<uint16_t> dest(min, max);

	return dest(engine);
}

float DirectXpre::Util::Rand(float min, float max)
{
	std::random_device seed;
	std::default_random_engine engine(seed());

	std::uniform_real_distribution<float> dest(min, max);

	return dest(engine);
}
double DirectXpre::Util::Rand(double min, double max)
{
	std::random_device seed;
	std::default_random_engine engine(seed());

	std::uniform_real_distribution<double> dest(min, max);

	return dest(engine);
}

float DirectXpre::Util::Clamp(float num, float min, float max)
{

	if (num > max)
	{
		return max;
	}
	else if (num < min)
	{
		return min;
	}

	return num;

}

std::string DirectXpre::Util::WStringToString(const std::wstring& wstr)
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

std::wstring DirectXpre::Util::ConvertMultByteStringToWideString(const std::string& mstring)
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

std::string DirectXpre::Util::SeparateFilePath(const std::string& filePath)
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

	//ファイル名を返すよ
	return fileName;
}

std::vector<std::string> DirectXpre::Util::FindFileNames(const std::string& dir, const std::string& extension, bool isExtension)
{
	//人のコードを参考にしました
	//結果を入れる
	std::vector<std::string> result;

	//状態を保持
	HANDLE hFind;
	//結果のデータを入れるところ
	WIN32_FIND_DATA win32fd;

	//探すファイル名指定　ワイルドカードを使用
	std::string search_name = dir + "*" + extension;
	//ワイド文字にする
	std::wstring wsearch_name = ConvertMultByteStringToWideString(search_name);
	//ファイル検索
	hFind = FindFirstFile(wsearch_name.c_str(), &win32fd);

	//見つからなかった場合
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return result;
	}

	//次のファイルがある限り読み込み続ける
	do
	{
		//ディレクトリなら無視
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {}
		else
		{
			//ファイル名をパス付で取得
			std::wstring wfileName = win32fd.cFileName;
			std::string fileName = WStringToString(wfileName);		
			result.push_back(fileName);
		}
	} while (FindNextFile(hFind, &win32fd));
	//閉じる
	FindClose(hFind);

	//拡張子を消す
	if (isExtension == false) {
		for (auto& fileName : result) {
			size_t num = fileName.find(".");
			fileName = fileName.substr(0, num);
		}
	}

	return result;
}


bool DirectXpre::Util::Combo(const char* label, int32_t& current_item, const std::vector<std::string>& items)
{
	
	//demoから直引っ張ってきた
	//操作したかを取る
	bool result = false;

	//選ばれているやつの描画ように選択されているのを保持
	const char* combo_preview_value = items[current_item].c_str();

	//カスタム性の高いcombo
	if (ImGui::BeginCombo(label, combo_preview_value))
	{
		//個数分回す
		for (int n = 0; n < items.size(); n++)
		{
			//選択されているか取得
			const bool is_selected = (current_item == n);
			//選ばれたら数値を変更
			if (ImGui::Selectable(items[n].c_str(), is_selected))
			{
				current_item = n;
				result = true;
			}

			//初期設定?(選択肢を開くときの?)
			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	//ここまでエラー出てないなら正常に終わったはず
	return result;
}