#pragma once
#include <cstdint>
#include <string>
#include <Windows.h>
const float PI = 3.141592653589f;



class Util
{
public:

	//ラジアンから角度
	static float RadianToAngle(float Radian);

	//角度からラジアン
	static float AngleToRadian(float Angle);

	static uint32_t Rand(uint32_t min, uint32_t max);
	static uint16_t Rand(uint16_t min, uint16_t max);

	static float Rand(float min, float max);
	static double Rand(double min, double max);

	static float Clamp(float num, float min, float max);

	//wstringをstringに変換
	static std::string WStringToString(const std::wstring& wstr);

	/// <summary>
	/// マルチバイト文字列をワイド文字列に変換
	/// </summary>
	/// <param name="mstring">マルチバイト文字</param>
	/// <returns>ワイド文字</returns>
	static std::wstring ConvertMultByteStringToWideString(const std::string& mstring);

	/// <summary>
	/// フォルダパスとファイル名を分離する
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns>ファイル名</returns>
	static std::string SeparateFilePath(const std::string& filePath);
};