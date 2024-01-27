#pragma once

/**
 * @file Player.h
 * @brief ゲームの自機のクラス
 * @author フクヤマ
 * @date 2023_11/10
 */

#include "Object3D.h"
#include"Input.h"
#include "cameraObj.h"
#include "Sprite.h"
#include "PlayerBullet.h"
#include <list>
#include "MobCollision.h"
#include "CubeCollider.h"
#include "CountTimer.h"

class Player
{
public:
	Player();
	~Player();

	//初期化
	void Init();

	//更新
	void Update();

	//imguiの更新
	void ImguiUpdate();

	//描画
	void Draw();

	//攻撃処理
	void Attack();

	//ダメージを受けた時の処理
	void Damage();

	//弾を補充するときの処理
	void Reload();

	//マウスの位置を取得してその位置をレティクル用のオブジェクト等に入れる関数
	void Reticle2DMouse();

	//位置関係をリセット
	void Reset(){
		pos_ = {0,0,0};

		moveVec_ = { 0,0,0 };

		rotTimer_ = 0;

		rotVec_ = { 0,0,0 };

		playerCamera_.pos_ = { 0,0,0 };
		playerCamera_.rotate_ = { 0,0,0 };
		playCamera_.upDate();

		moveEventStart_ = false;
		battleEventStart_ = false;


		playerCamera_.pos_ = originalPos_;
		attackFlag_ = false;

		bulletNum_ = bulletMaxNum_;

		hp_ = 3;
	}

	//本体
	Object3D playerObj_;

	//レティクルと合わせるオブジェクト
	Object3D reticle3DObj_;

	//オブジェクトとしてのカメラ
	cameraObj playerCamera_;

	//かめら本体
	Camera playCamera_;

	//スプライトのレティクル
	Sprite reticle_;

	//隠れるときの時間計測用の変数
	float time_ = 0;
	
	//弾が再度撃てるようになる時間
	float bulletMaxCT_ = 10;

	//弾の速度
	float bulletSpeed_ = 5.0f;

	//リロードとか再発射の時間を無視して撃てるようになるフラグ
	bool isDebugShot_ = false;

	//マズルフラッシュが描画されている時間
	float muzzleFlashTime_ = 2.0f;

	//マズルフラッシュが描画されている最大時間
	float muzzleFlashMaxTime_ = 2.0f;

	//体力
	float hp_ = 3;

	//タイトル用の処理にするか
	bool isTitle_ = false;

	//隠れるか
	bool attackFlag_ = false;

	//回転保存用のやつ
	Vector3 rotVec_ = { 0,0,0 };

private:

	//右に隠れる処理
	void HideRightWall();

	//下に隠れる処理
	void HideDownWall();

	//左に隠れる処理
	void HideLeftWall();

	//イベントの更新処理
	void EventUpdate();

	//スプライトの更新処理
	void SpriteUpdate();

	//マズルフラッシュして
	void MuzzleFlash();

	//弾の追加処理部分
	void BulletAdd();

private:
	

	Input* input_ =nullptr;

	Vector3 pos_ = {};

	Vector3 moveVec_ = {};

	float rotTimer_ = 0;

	//移動開始のための事前処理用フラグ
	bool moveEventStart_ = false;

	//戦闘開始のための事前処理用フラグ
	bool battleEventStart_ = false;

	//滑らかな移動のため
	float maxTime_ = 500;
	//カメラの始点の移動するときの時間
	float maxMoveTime_ = maxTime_/15;

	float attackTime_ = 3;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//隠れるときの動く大きさ
	float hideDistanceX_ = 2;
	float hideDistanceY_ = 2;

	//モデル
	AnimationModel* bulletModel_=nullptr;
	AnimationModel* gunModel_=nullptr;

	float bulletCT_ = 0;

	const float bulletMaxNum_ = 6;
	float bulletNum_ = 6;

	//当たり判定の処理
	MobCollision collision;

	//当たり判定の本体
	CubeCollider Collider;

	//体力とか
	Sprite hp1Sprote_;
	Sprite hp2Sprote_;
	Sprite hp3Sprote_;
	Sprite damageSprote_;

	//弾の画像群
	std::vector<Sprite>bulletSprite_;

	//操作表示用画像
	//キーボードマウス用
	Sprite mouseSprite_;
	Sprite rightClickSprite_;
	Sprite spaceButtonSprite_;

	//ゲームパッド
	Sprite gamePadButtonASprite_;
	Sprite gamePadButtonRTSprite_;
	Sprite gamePadStickLSprite_;

	//状態
	Sprite stateSprite_;

	//操作描画
	Sprite reticleMoveSprite_;
	Sprite shotIconSprite_;
	Sprite stateiconSprite_;


	//音
	std::string gunShotSount_;
	std::string gunReloadSount_;
	std::string damageSound_;
	
	//キーボード使っているかい？
	bool isUseKeybord_ = true;

	//壁に隠れたりする用の基準位置
	Vector3 originalPos_ = {};

	Vector3 test = {};

	//ダメージを受けたか
	bool isDamage_ = false;

	//ダメージを受けた時の揺れる範囲
	Vector3 shakeVecSize_ = {90,0,0};

	//揺れる強さ
	Vector3 shakePow_ = { 0.5f,1.0f,1.0f };

	//ダメージを受けた時のシェイク用タイマー
	float damageTimer_ = 0;
	float damageMaxTimer_ = 10;
	
	//ダメージを受けた時の演出用のタイマー
	float damageEffectMaxTime_ = 50;
	float damageEffectTimer_ = damageEffectMaxTime_;


	//マズルフラッシュ関連変数
	Object3D flashObj_;

	float flashAlpha_ = 0;

	//動きを切り替えるときの時間
	float flashChengTime_ = 3;

	//後引き
	float flashEndTime_ = 7;

	//全体の動きのフレーム数なので切り替えるときの値を足している
	float flashMaxTime_ = flashEndTime_ + flashChengTime_;

	//現在進捗具合
	float flashTimer_ = flashMaxTime_;

	//リロード用のタイマー
	float reloadTimer = 0;

	float reloadMaxTime = 5;
	

	Quaternion rot;

	CountTimer timer;

};

