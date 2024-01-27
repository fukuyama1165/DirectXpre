#pragma once
/**
 * @file ExplosionObjManager.h
 * @brief ゲーム内の爆発するオブジェクトの管理を行うクラス
 * @author フクヤマ
 * @date 2023_01/10
 */

#include "ExplosionObj.h"
#include <list>
class ExplosionObjManager
{

public:

	static ExplosionObjManager* GetInstance();

	/// <summary>
	/// 爆発オブジェクト追加
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="explosioneventNum">爆発イベントを起こすイベント位置(何個目のイベントか)</param>
	/// <param name="size">当たり判定の大きさ</param>
	/// <param name="explosionSize">爆発範囲</param>
	/// <param name="explosionTime">爆発時間</param>
	void PopExplosionObj(Vector3 pos,int32_t explosioneventNum, Vector3 size = { 1,1,1 }, Vector3 explosionSize = { 0,0,0 }, float explosionTime = 1.0f);

	//初期化
	void Init();

	//更新
	void UpDate(int32_t EventNum);

	//描画
	void Draw();

	//まとめたやつを中身を全部消す
	void Reset() { objs_.clear();};

	//爆発するオブジェクトが何個あるかを返す
	size_t GetExplosionObjNum() { return objs_.size(); };

private:

	ExplosionObjManager() = default;
	~ExplosionObjManager();

	ExplosionObjManager(const ExplosionObjManager&) = delete;
	ExplosionObjManager& operator=(const ExplosionObjManager&) = delete;
public:
	//爆発するオブジェクトのまとめ
	std::list<std::unique_ptr<ExplosionObj>> objs_;

	//モデル
	AnimationModel* model_ = nullptr;

	//爆発音のサウンドのハンドル
	std::string explosionSound_;

};