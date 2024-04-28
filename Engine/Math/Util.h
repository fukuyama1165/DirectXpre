#pragma once


/**
 * @file Util.h
 * @brief 便利機能や固定の値を置くクラス
 * @author フクヤマ
 * @date 2023_12/29
 */

#include <cstdint>
#include <Windows.h>
const float PI = 3.141592653589f;
#include <string>
#include <vector>

namespace DirectXpre
{

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

		/// <summary>
		/// 指定したフォルダの中にある指定した拡張子のファイルの名前を取ってくる関数
		/// </summary>
		/// <param name="dir">フォルダへのパス</param>
		/// <param name="extension">探す拡張子</param>
		/// <param name="isExtension">返す文字から拡張子をつけるか</param>
		/// <returns>見つかったファイル名</returns>
		static std::vector<std::string> FindFileNames(const std::string& dir, const std::string& extension, bool isExtension);

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

		/// <summary>
		/// imguiのcomboのラッピング関数
		/// </summary>
		/// <param name="label">つける名前</param>
		/// <param name="current_item">選択した場所</param>
		/// <param name="items">表示するための文字列</param>
		/// <returns></returns>
		static bool Combo(const char* label, int32_t& current_item, const std::vector<std::string>& items);

	};
}