#pragma once
#include "Object3D.h"
#include"Input.h"
#include "cameraObj.h"
#include "Sprite.h"
#include "PlayerBullet.h"
#include <list>
#include "MobCollision.h"
#include "CubeCollider.h"

class Player
{
public:
	Player();
	~Player();

	void Init(const std::string& directoryPath, const char filename[]);
	void Update();
	void Draw();
	void Attack();
	void Damage();

	//マウスの位置を取得してその位置をレティクル用のオブジェクト等に入れる関数
	void Reticle2DMouse();

	Object3D playerObj_;

	Object3D reticle3DObj_;

	//オブジェクトとしてのカメラ
	cameraObj playerCamera_;

	//かめら本体
	Camera playCamera_;

	Sprite reticle_;

	float time_ = 0;

	bool cameraCheng_ = false;

	float bulletMaxCT_ = 10;

	float bulletSpeed_ = 1.0f;

	bool isDebugShot_ = false;

	float muzzleFlashTime_ = 2.0f;

	float muzzleFlashMaxTime_ = 2.0f;

	float hp_ = 3;

	//タイトル用の処理にするか
	bool isTitle_ = false;

	//隠れるか
	bool attackFlag_ = false;

private:

	void HideRightWall();

	void HideDownWall();

	//マズルフラッシュして
	void MuzzleFlash();

private:

	
	
	float rotate_ = 0;
	

	Input* input_ =nullptr;

	Vector3 pos_ = {};

	Vector3 moveVec_;

	bool moveEventStart_ = false;

	float moveSpeed_ = 0.1f;

	//注視点滑らかな移動のため
	float maxTime_ = 500;
	//カメラの始点の移動するときの時間
	float maxMoveTime_ = maxTime_/15;

	

	float attackTime_ = 3;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;

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

	

};

