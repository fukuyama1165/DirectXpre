#include "Player.h"
#include <imgui.h>
#include "Easing.h"
#include "EventPointManager.h"
#include "Quaternion.h"
#include "ModelManager.h"
#include "WinApp.h"
#include "EmitterManager.h"
#include "XAudio.h"

Player::Player()
{

}
Player::~Player()
{

}

void Player::Init()
{
	input_ = input_->GetInstance();
	playerObj_.FBXInit();
	reticle3DObj_.FBXInit();
	

	//画像の読み込み
	Texture::GetInstance()->loadTexture("Resources/life.png", "Life");
	Texture::GetInstance()->loadTexture("Resources/ammunition.png", "Ammo");
	Texture::GetInstance()->loadTexture("Resources/Reticle.png", "Reticle");
	Texture::GetInstance()->loadTexture("Resources/particle.png", "Particle");

	Texture::GetInstance()->loadTexture("Resources/mouseimage.png", "Mouse");
	Texture::GetInstance()->loadTexture("Resources/mouseRightClickimage.png", "MouseRightClick");
	Texture::GetInstance()->loadTexture("Resources/SPACEimage.png", "SPACEimage");

	Texture::GetInstance()->loadTexture("Resources/padButtonAimage.png", "padButtonA");
	Texture::GetInstance()->loadTexture("Resources/RT.png", "padRT");	
	Texture::GetInstance()->loadTexture("Resources/stickL.png", "stickL");

	Texture::GetInstance()->loadTexture("Resources/standUpIcon.png", "stateStandUp");
	Texture::GetInstance()->loadTexture("Resources/squatIcon.png", "stateSquat");

	Texture::GetInstance()->loadTexture("Resources/ReticleMove.png", "reticleMove");
	Texture::GetInstance()->loadTexture("Resources/shoticon.png", "shotIcon");
	Texture::GetInstance()->loadTexture("Resources/obj/ammo/ammoTex.png", "ammoTex");
	
	//モデルの読み込み
	ModelManager::GetInstance()->Load("testGLTFBall", "gltf", "whiteBall", "white1x1");
	ModelManager::GetInstance()->Load("Gun", "gltf", "Gun", "gun");
	ModelManager::GetInstance()->Load("ammo", "gltf", "Ammo", "ammoTex");

	//モデルのセット
	bulletModel_ = ModelManager::GetInstance()->SearchModelData("whiteBall");
	gunModel_ = ModelManager::GetInstance()->SearchModelData("Gun");
	
	//モデルとかの位置
	playerObj_.SetPos({ 0,5,0 });
	playerObj_.SetScale({ 0.13f,0.21f,0.17f });
	playerObj_.useQuaternion = true;

	playerCamera_.pos_ = { 0,0,-200 };
	playerCamera_.rotate_ = { 0,0,0 };
	playCamera_.eye_ = { 0,0,-3 };

	reticle3DObj_.SetPos({ 0,0,-100 });
	//reticle3DObj_.SetScale({ 0.05f,0.05f,0.05f });
	reticle3DObj_.SetScale({ 5,5,5 });

	//当たり判定
	collision = MobCollision("player");

	Collider.SetObject(&collision);

	Collider.Initialize();

	CollisionManager::GetInstance()->AddCollider(&Collider);

	//スプライト生成
	hp1Sprote_.initialize("Life");
	hp2Sprote_.initialize("Life");
	hp3Sprote_.initialize("Life");
	damageSprote_.initialize("white1x1");

	hp1Sprote_.pos_ = { hp1Sprote_.GetTextureSize().x/2,32 };
	hp2Sprote_.pos_ = { hp1Sprote_.GetTextureSize().x*1.5f,32};
	hp3Sprote_.pos_ = { hp1Sprote_.GetTextureSize().x*2.5f,32 };

	damageSprote_.pos_ = { WinApp::SWindowWidthF_/2,WinApp::SWindowHeightF_/2 };
	damageSprote_.scale_ = { WinApp::SWindowWidthF_,WinApp::SWindowHeightF_ };

	reticle_.initialize("Reticle");
	reticle_.scale_ = { 2,2 };

	mouseSprite_.initialize("Mouse");
	rightClickSprite_.initialize("MouseRightClick");
	spaceButtonSprite_.initialize("SPACEimage");

	gamePadButtonASprite_.initialize("padButtonA");
	gamePadButtonRTSprite_.initialize("padRT");
	gamePadStickLSprite_.initialize("stickL");

	reticleMoveSprite_.initialize("reticleMove");
	shotIconSprite_.initialize("shotIcon");
	stateiconSprite_.initialize("stateSquat");

	//サイズ気に入らないので直しとく
	mouseSprite_.pos_ = { mouseSprite_.GetTextureSize().x / 2,WinApp::SWindowHeightF_ - (mouseSprite_.GetTextureSize().y + rightClickSprite_.GetTextureSize().y + spaceButtonSprite_.GetTextureSize().y) };
	rightClickSprite_.pos_ = { rightClickSprite_.GetTextureSize().x / 2,WinApp::SWindowHeightF_ - (rightClickSprite_.GetTextureSize().y + spaceButtonSprite_.GetTextureSize().y) };
	spaceButtonSprite_.pos_ = { spaceButtonSprite_.GetTextureSize().x / 2,WinApp::SWindowHeightF_ - (spaceButtonSprite_.GetTextureSize().y) };

	reticleMoveSprite_.pos_ = { (mouseSprite_.GetTextureSize().x / 2) + reticleMoveSprite_.GetTextureSize().x/2, WinApp::SWindowHeightF_ - (mouseSprite_.GetTextureSize().y + rightClickSprite_.GetTextureSize().y + spaceButtonSprite_.GetTextureSize().y)};
	shotIconSprite_.pos_ = { (rightClickSprite_.GetTextureSize().x / 2)+shotIconSprite_.GetTextureSize().x/2,WinApp::SWindowHeightF_ - (rightClickSprite_.GetTextureSize().y + spaceButtonSprite_.GetTextureSize().y) };
	stateiconSprite_.pos_ = { (spaceButtonSprite_.GetTextureSize().x / 2)+stateiconSprite_.GetTextureSize().x,WinApp::SWindowHeightF_ - (spaceButtonSprite_.GetTextureSize().y) };

	stateiconSprite_.setColor({ 0.5f,0.5f ,0.5f ,1.0f });

	//位置が気になるので手が開いたら直す(おもにマジックナンバー)
	gamePadButtonASprite_.pos_ = { gamePadButtonASprite_.GetTextureSize().x / 2,WinApp::SWindowHeightF_ - (gamePadButtonASprite_.GetTextureSize().y + gamePadButtonRTSprite_.GetTextureSize().y + gamePadStickLSprite_.GetTextureSize().y) };
	gamePadButtonRTSprite_.pos_ = { gamePadButtonRTSprite_.GetTextureSize().x-48,WinApp::SWindowHeightF_ - (gamePadButtonRTSprite_.GetTextureSize().y + gamePadStickLSprite_.GetTextureSize().y/2) };
	gamePadStickLSprite_.pos_ = { gamePadStickLSprite_.GetTextureSize().x / 2,WinApp::SWindowHeightF_ - (gamePadStickLSprite_.GetTextureSize().y/2) };

	stateSprite_.initialize("stateStandUp");
	stateSprite_.pos_ = { WinApp::SWindowWidthF_ / 4*3,WinApp::SWindowHeightF_ - (stateSprite_.GetTextureSize().y/3*2)};
	stateSprite_.scale_ = { 2.0f,2.0f };
	stateSprite_.setColor({ 0.5f,0.5f ,0.5f ,1.0f });

	//弾の個数分生成
	for (uint16_t i = 0; i < bulletMaxNum_; i++)
	{
		Sprite newBulletSprite;
		newBulletSprite.initialize("Ammo");
		newBulletSprite.pos_ = { WinApp::SWindowWidth_/2 + newBulletSprite.GetTextureSize().x/2 * i,WinApp::SWindowHeight_- newBulletSprite.GetTextureSize().y/4 };
		//newBulletSprite.scale_ = { 0.5f,0.5f };
		bulletSprite_.push_back(newBulletSprite);
	}

	//音の読み込み
	gunShotSount_ = XAudio::GetInstance()->SoundLoadWave("Resources/sound/GunShot.wav");
	gunReloadSount_ = XAudio::GetInstance()->SoundLoadWave("Resources/sound/GunReload.wav");
	damageSound_ = XAudio::GetInstance()->SoundLoadWave("Resources/sound/enemydown.wav");

	//板ポリの設定
	flashObj_.boarPolygonInit();

	//timer.Initialize();

}

void Player::Update()
{

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
	{
		return bullet->IsDead();
	});
	
	//ゲームシーンなら
	if (!isTitle_)
	{

		//隠れる動作用のフラグ立てる場所(リロードも)
		if (((input_->PushKey(DIK_SPACE) && input_->GetIsUseKeybord()) || (input_->GetGamePadButton(XINPUT_GAMEPAD_A) && !input_->GetIsUseKeybord())) && time_ <= 20 && EventPointManager::GetInstance()->GetPEventPoint()->GetEventType() != moveEvent)
		{
			attackFlag_ = true;
			Reload();


		}

		//隠れた時一回だけ音を鳴らしたい
		if ((input_->TriggerKey(DIK_SPACE) || input_->GetGamePadButtonDown(XINPUT_GAMEPAD_A)) && EventPointManager::GetInstance()->GetPEventPoint()->GetEventType() != moveEvent)
		{
			XAudio::PlaySoundData(gunReloadSount_, 1.0f);
			Collider.isActive_ = false;
		}

		if ((input_->ReleaseKey(DIK_SPACE) && input_->GetIsUseKeybord()) || (input_->GetGamePadButtonUp(XINPUT_GAMEPAD_A) && !input_->GetIsUseKeybord()))
		{
			attackFlag_ = false;
			Collider.isActive_ = true;
		}
	}
	
	EventUpdate();
	
	SpriteUpdate();
	
	playCamera_.upDate();

	playerCamera_.upDate();

	/*test += {0.1f, 0, 0};

	hp3Sprote_.pos_.y = sinf(test.x)*10;*/

	

	//移動(基本的にカメラを基準に)	
	Vector3 playerPos = { playerCamera_.pos_.x,playerCamera_.pos_.y - 1 ,playerCamera_.pos_.z };

	Vector3 pos = Vector3::normalize(reticle3DObj_.GetWorldPos() - playerPos);

	playerObj_.SetPos(playerPos + (playerCamera_.GetCameraP()->forward_ * 5));
	flashObj_.SetPos(playerPos + (playerCamera_.GetCameraP()->forward_ * 6));


	Vector3 normalVec = Vector3::normalize(reticle3DObj_.GetWorldPos() - playerObj_.GetPos());
	//レティクルの方向へ向ける
	
	//馬鹿だったので補足
	//dirTodirは(無回転の時の向き,回転させたい方向)なので(始点,終点)ではない
	//
	//Quaternion rot2 = rot;

	//rot = Quaternion::DirectionToDirection(Vector3::normalize(playerCamera_.forward_), normalVec);
	///*Vector3 reticlePosX;
	//Vector3 reticlePosY;
	//Vector3 reticlePosZ;

	//reticlePosX = Quaternion::RotateVector({ 1,0,0 }, rot);
	//reticlePosY = Quaternion::RotateVector({ 0,1,0 }, rot);
	//reticlePosZ = Quaternion::RotateVector({ 0,0,1 }, rot);*/

	////playerObj_.Rotate_ = { reticlePosY.z,reticlePosZ.x ,0 };
	////playerObj_.Rotate_ = { reticlePosY.z,reticlePosZ.x ,reticlePosX.y };

	//playerObj_.Rotate_ = Quaternion::RotateVector(playerCamera_.forward_, rot);
	//if (rot2.v.x != rot.v.x|| rot2.v.y != rot.v.y|| rot2.v.z != rot.v.z|| rot2.w != rot.w)
	//{
	//	playerObj_.quaternionRot_ *= rot;
	//}

	Vector3 nowvec = Quaternion::RotateVector({0,0,1}, playerObj_.quaternionRot_).normalize();

	nowvec.normalize();

	rot = Quaternion::DirectionToDirection({ 0,0,1 }, Vector3::normalize(reticle3DObj_.GetPos() - playerObj_.GetPos()));


	if (nowvec != Vector3::normalize(reticle3DObj_.GetPos() - playerObj_.GetPos()))
	{
		playerObj_.quaternionRot_ = Quaternion::Normalize(rot);


	}



	if (!isTitle_)
	{
		Reticle2DMouse();
	}
	
	
	playerObj_.Update();
	Collider.Update(playerCamera_.pos_);


	//reticle3DObj_.Update();

	if (!isTitle_)
	{
		if ((!attackFlag_ && EventPointManager::GetInstance()->GetPEventPoint()->GetEventType() != moveEvent && bulletNum_ > 0) || isDebugShot_)
		{
			Attack();
		}
	}

	if (muzzleFlashTime_ > 0)
	{
		playerObj_.SLightGroup_->SetPointLightActive(0, true);	

	}
	else
	{
		playerObj_.SLightGroup_->SetPointLightActive(0, false);
	}

	if (muzzleFlashTime_ > 0)
	{
		muzzleFlashTime_--;
	}
	
	
	Damage();

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	if (!isTitle_)
	{
		if (collision.isHit)
		{
			hp_--;
			collision.isHit = false;
			XAudio::GetInstance()->PlaySoundData(damageSound_);
			isDamage_ = true;
			damageEffectTimer_ = 0;
		}
	}

	hp1Sprote_.Update();
	hp2Sprote_.Update();
	hp3Sprote_.Update();
	damageSprote_.Update();

	if (isDebugShot_ && bulletNum_ <= 0)
	{
		Reload();
	}

	for (uint32_t i = 0; i < bulletMaxNum_; i++)
	{

		bulletSprite_[i].pos_ = lerp({ WinApp::SWindowWidth_ / 2 + bulletSprite_[i].GetTextureSize().x / 2 * (float)i,WinApp::SWindowHeightF_ + bulletSprite_[i].GetTextureSize().y / 4 }, { WinApp::SWindowWidth_ / 2 + bulletSprite_[i].GetTextureSize().x / 2 * (float)i, WinApp::SWindowHeight_ - bulletSprite_[i].GetTextureSize().y / 4 }, reloadTimer / reloadMaxTime);

	}

	if (reloadTimer < reloadMaxTime)
	{
		reloadTimer++;
	}
	
	for (uint16_t i = 0; i < bulletMaxNum_; i++)
	{
		bulletSprite_[i].Update();
	}

	

	MuzzleFlash();

	flashObj_.Update();

	stateSprite_.Update();

	mouseSprite_.Update();
	rightClickSprite_.Update();
	spaceButtonSprite_.Update();

	gamePadButtonASprite_.Update();
	gamePadButtonRTSprite_.Update();
	gamePadStickLSprite_.Update();

	reticleMoveSprite_.Update();
	shotIconSprite_.Update();
	stateiconSprite_.Update();



	//timer.Update();

}

void Player::ImguiUpdate()
{
#ifdef _DEBUG
	if (!isTitle_)
	{
		ImGui::Begin("player");

		ImGui::Text("playerObjPos:%0.2ff,%0.2ff,%0.2ff", playerObj_.GetWorldPos().x, playerObj_.GetWorldPos().y, playerObj_.GetWorldPos().z);
		ImGui::Text("cameraFront:%0.2ff,%0.2ff,%0.2ff", playerCamera_.forward_.x, playerCamera_.forward_.y, playerCamera_.forward_.z);
		ImGui::Text("camerapos:%0.2ff,%0.2ff,%0.2ff", playerCamera_.pos_.x, playerCamera_.pos_.y, playerCamera_.pos_.z);
		ImGui::Text("quaternionRot:%0.2ff,%0.2ff,%0.2ff,%0.2ff", playerObj_.quaternionRot_.v.x, playerObj_.quaternionRot_.v.y, playerObj_.quaternionRot_.v.z, playerObj_.quaternionRot_.w);
		ImGui::Text("rot:%0.2ff,%0.2ff,%0.2ff,%0.2ff", rot.v.x, rot.v.y, rot.v.z, rot.w);
		ImGui::Text("hppos:%0.2ff", hp3Sprote_.pos_.y);

		ImGui::End();
	}
#endif
}

void Player::Draw()
{
	playerObj_.FBXDraw(*gunModel_);
	//reticle3DObj_.Draw();
	
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(bulletModel_);
	}

	if (!isTitle_)
	{
		reticle_.Draw();
		if (hp_ > 0)
		{
			hp1Sprote_.Draw();
		}
		if (hp_ > 1)
		{
			hp2Sprote_.Draw();
		}
		if (hp_ > 2)
		{
			hp3Sprote_.Draw();
		}

		for (uint16_t i = 0; i < bulletMaxNum_; i++)
		{
			if (bulletNum_ > i)
			{
				bulletSprite_[i].Draw();
			}
		}

		stateSprite_.Draw();
	}
	
	//発射の時のエフェクト
	if (flashObj_.GetColor().w > 0)
	{
		flashObj_.Draw("Particle");
	}

	
	
	damageSprote_.Draw();

	if (!isTitle_)
	{
		if (input_->GetIsUseKeybord())
		{
			mouseSprite_.Draw();
			rightClickSprite_.Draw();
			spaceButtonSprite_.Draw();
		}
		else
		{
			gamePadButtonASprite_.Draw();
			gamePadButtonRTSprite_.Draw();
			gamePadStickLSprite_.Draw();
		}

		reticleMoveSprite_.Draw();
		shotIconSprite_.Draw();
		stateiconSprite_.Draw();

	}

	//reticle3DObj_.FBXDraw(*bulletModel_);

	//timer.Draw();

}

void Player::Attack()
{
	
	if (isTitle_)
	{
		reticle3DObj_.Update();
	}

	if (bulletCT_ > 0)
	{
		bulletCT_--;
	}

	

	if ((input_->GetMouseButtonDown(0) && bulletCT_ <= 0 && input_->GetIsUseKeybord()) || (isTitle_ && input_->GetIsUseKeybord()))
	{
		BulletAdd();
		
	}

	

	//ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetIsUseGamePad())
	{
		return;
	}

	//Rトリガーを押していたら
	if ((Input::GetInstance()->GetGamePadRTrigger() && bulletCT_ <= 0 && !input_->GetIsUseKeybord()) || (isTitle_ && !input_->GetIsUseKeybord()))
	{
		BulletAdd();
	}



}

void Player::HideRightWall()
{
	if (attackFlag_)
	{
		if (time_ < maxMoveTime_)
		{
			time_++;
		}
		
	}
	else
	{
		if (time_ > 0)
		{
			time_--;
		}

	}


	Vector3 camerapos = {};

	camerapos = DirectXpre::easeOutQuint(Vector3{ originalPos_.x, originalPos_.y, originalPos_.z }, originalPos_+ (Vector3::normalize(playerCamera_.GetCamera().rightDirection)*hideDistanceX_), time_ / maxMoveTime_);

	playerCamera_.pos_ = camerapos;

}

void Player::HideDownWall()
{
	if (attackFlag_)
	{
		if (time_ < maxMoveTime_)
		{
			time_++;
		}
		
	}
	else
	{
		time_ = 0;
	}

	Vector3 camerapos = {};
	
	camerapos = DirectXpre::easeOutQuint(Vector3{ originalPos_.x, originalPos_.y, originalPos_.z }, Vector3{ originalPos_.x, originalPos_.y-hideDistanceY_, originalPos_.z }, time_ / maxMoveTime_);

	playerCamera_.pos_ = camerapos;
}

void Player::HideLeftWall()
{
	if (attackFlag_)
	{
		if (time_ < maxMoveTime_)
		{
			time_++;
		}

	}
	else
	{
		if (time_ > 0)
		{
			time_--;
		}

	}


	Vector3 camerapos = {};

	camerapos = DirectXpre::easeOutQuint(Vector3{ originalPos_.x, originalPos_.y, originalPos_.z }, originalPos_ + (-Vector3::normalize(playerCamera_.GetCamera().rightDirection) * hideDistanceX_), time_ / maxMoveTime_);

	playerCamera_.pos_ = camerapos;

}

void Player::EventUpdate()
{
	//移動中ではないなら
	if (EventPointManager::GetInstance()->GetPEventPoint()->GetEventType() != moveEvent)
	{
		if (EventPointManager::GetInstance()->GetPEventPoint()->GetPlayerHideVector() == (float)playerHideVectorType::Right)
		{
			HideRightWall();
		}
		else if(EventPointManager::GetInstance()->GetPEventPoint()->GetPlayerHideVector() == (float)playerHideVectorType::Down)
		{
			HideDownWall();
		}
		else
		{
			HideLeftWall();
		}
	}

	//移動イベント処理
	if (EventPointManager::GetInstance()->GetPEventPoint()->GetEventType() == EventType::moveEvent && !EventPointManager::GetInstance()->GetPEventPoint()->GetIsFinished())
	{
		//移動を開始していないなら
		if (moveEventStart_ == false)
		{
			//移動中は隠れているのをやめて元の位置に戻す
			if (EventPointManager::GetInstance()->GetEventCount() != 0)
			{
				playerCamera_.pos_ = originalPos_;
				attackFlag_ = false;
				time_ = 0;
			}
			//開始位置を取得して進む大きさを決定
			pos_ = EventPointManager::GetInstance()->GetPEventPoint()->GetMoveStartPoint();
			moveVec_ = nainavec3(EventPointManager::GetInstance()->GetPEventPoint()->GetMovePoint(), pos_).normalize();
			playerCamera_.pos_ = EventPointManager::GetInstance()->GetPEventPoint()->GetMoveStartPoint();
			moveEventStart_ = true;
		}

		//角度を動かす時間があるなら動かす
		if (EventPointManager::GetInstance()->GetPEventPoint()->GetEventSeting().moveRotTime != 0)
		{
			playerCamera_.rotate_ = lerp(rotVec_, EventPointManager::GetInstance()->GetPEventPoint()->GetMovePointRot(), rotTimer_ / EventPointManager::GetInstance()->GetPEventPoint()->GetEventSeting().moveRotTime);
		}

		if (rotTimer_ < EventPointManager::GetInstance()->GetPEventPoint()->GetEventSeting().moveRotTime)
		{
			rotTimer_++;
		}

		//ちょっとずれてもいいように
		if (((playerCamera_.pos_.x <= EventPointManager::GetInstance()->GetPEventPoint()->GetMovePoint().x + EventPointManager::GetInstance()->GetPEventPoint()->GetMoveSpeed()) &&
			(playerCamera_.pos_.x >= EventPointManager::GetInstance()->GetPEventPoint()->GetMovePoint().x - EventPointManager::GetInstance()->GetPEventPoint()->GetMoveSpeed())) &&
			((playerCamera_.pos_.y <= EventPointManager::GetInstance()->GetPEventPoint()->GetMovePoint().y + EventPointManager::GetInstance()->GetPEventPoint()->GetMoveSpeed()) &&
				(playerCamera_.pos_.y >= EventPointManager::GetInstance()->GetPEventPoint()->GetMovePoint().y - EventPointManager::GetInstance()->GetPEventPoint()->GetMoveSpeed())) &&
			((playerCamera_.pos_.z <= EventPointManager::GetInstance()->GetPEventPoint()->GetMovePoint().z + EventPointManager::GetInstance()->GetPEventPoint()->GetMoveSpeed()) &&
				(playerCamera_.pos_.z >= EventPointManager::GetInstance()->GetPEventPoint()->GetMovePoint().z - EventPointManager::GetInstance()->GetPEventPoint()->GetMoveSpeed())))
		{
			//回転が終わったことも確認
			if (rotTimer_ >= EventPointManager::GetInstance()->GetPEventPoint()->GetEventSeting().moveRotTime)
			{
				moveVec_ = { 0,0,0 };
				originalPos_ = EventPointManager::GetInstance()->GetPEventPoint()->GetMovePoint();
				playerCamera_.pos_ = EventPointManager::GetInstance()->GetPEventPoint()->GetMovePoint();
				EventPointManager::GetInstance()->GetPEventPoint()->SetIsFinished(true);
				rotVec_ = EventPointManager::GetInstance()->GetPEventPoint()->GetMovePointRot();
				moveEventStart_ = false;
				rotTimer_ = 0;
			}
		}
		else
		{
			playerCamera_.pos_ += moveVec_ * EventPointManager::GetInstance()->GetPEventPoint()->GetMoveSpeed();
		}

	}

	if (EventPointManager::GetInstance()->GetPEventPoint()->GetEventType() == EventType::BattleEvent && !EventPointManager::GetInstance()->GetPEventPoint()->GetIsFinished())
	{
		if (battleEventStart_ == false)
		{
			//開始位置を取得
			playerCamera_.pos_ = EventPointManager::GetInstance()->GetPEventPoint()->GetEventSeting().playerPos;
			originalPos_ = EventPointManager::GetInstance()->GetPEventPoint()->GetEventSeting().playerPos;
			battleEventStart_ = true;
		}
	}
	//バトルイベント終了時の処理
	else if (EventPointManager::GetInstance()->GetPEventPoint()->GetEventType() == EventType::BattleEvent)
	{

		battleEventStart_ = false;

	}
}

void Player::SpriteUpdate()
{
	if (attackFlag_)
	{
		stateSprite_.ChangeTexture("stateSquat");
	}
	else
	{
		stateSprite_.ChangeTexture("stateStandUp");
	}

	//操作の描画の位置変更
	if (input_->GetIsUseKeybord())
	{
		//キーボードの時
		reticleMoveSprite_.pos_ = { (mouseSprite_.GetTextureSize().x / 2) + reticleMoveSprite_.GetTextureSize().x / 2, WinApp::SWindowHeightF_ - (mouseSprite_.GetTextureSize().y + rightClickSprite_.GetTextureSize().y + spaceButtonSprite_.GetTextureSize().y) };
		shotIconSprite_.pos_ = { (rightClickSprite_.GetTextureSize().x / 2) + +shotIconSprite_.GetTextureSize().x / 2,WinApp::SWindowHeightF_ - (rightClickSprite_.GetTextureSize().y + spaceButtonSprite_.GetTextureSize().y) };
		stateiconSprite_.pos_ = { (spaceButtonSprite_.GetTextureSize().x / 2) + +stateiconSprite_.GetTextureSize().x,WinApp::SWindowHeightF_ - (spaceButtonSprite_.GetTextureSize().y) };

		reticleMoveSprite_.scale_ = { 0.5f,0.5f };
		shotIconSprite_.scale_ = { 0.5f,0.5f };
		stateiconSprite_.scale_ = { 0.5f,0.5f };
	}
	else
	{
		//コントローラーの時(手が開いたら修正する)
		reticleMoveSprite_.pos_ = { (gamePadStickLSprite_.GetTextureSize().x / 2) + reticleMoveSprite_.GetTextureSize().x / 2,WinApp::SWindowHeightF_ - (gamePadStickLSprite_.GetTextureSize().y / 2) };
		shotIconSprite_.pos_ = { (gamePadButtonRTSprite_.GetTextureSize().x - 48) + +shotIconSprite_.GetTextureSize().x / 2,WinApp::SWindowHeightF_ - (gamePadButtonRTSprite_.GetTextureSize().y + gamePadStickLSprite_.GetTextureSize().y / 2) };
		stateiconSprite_.pos_ = { (gamePadButtonASprite_.GetTextureSize().x / 2) + +stateiconSprite_.GetTextureSize().x / 2,WinApp::SWindowHeightF_ - (gamePadButtonASprite_.GetTextureSize().y + gamePadButtonRTSprite_.GetTextureSize().y + gamePadStickLSprite_.GetTextureSize().y) };

		reticleMoveSprite_.scale_ = { 1.0f,1.0f };
		shotIconSprite_.scale_ = { 1.0f,1.0f };
		stateiconSprite_.scale_ = { 1.0f,1.0f };

	}
}

void Player::Reticle2DMouse()
{
	

	POINT mousePosition = {};
	Vector2 spritePosition = reticle_.pos_;

	if (input_->GetIsUseKeybord())
	{
		//スクリーン座標を取得
		GetCursorPos(&mousePosition);

		//クライアントエリア座標に変換する
		HWND hwnd = WinApp::GetInstance()->getHwnd();
		ScreenToClient(hwnd, &mousePosition);

		reticle_.pos_=(Vector2((float)mousePosition.x, (float)mousePosition.y));
		//reticle_.pos_=Input::GetInstance()->GetMousePos();
	}
	else if(!input_->GetIsUseKeybord())
	{
		spritePosition.x += (float)Input::GetInstance()->GetGamePadLStick().x*10;
		spritePosition.y -= (float)Input::GetInstance()->GetGamePadLStick().y * 10;

		reticle_.pos_ = spritePosition;
	}

	/*ImGui::Begin("check");

	ImGui::Text("reticlePos:%0.3f,%0.3f", reticle_.pos_.x, reticle_.pos_.y);

	ImGui::End();*/

	Matrix4x4 matViewport = {
			WinApp::SWindowWidthF_/2,0,0,0,
			0,-WinApp::SWindowHeightF_ / 2,0,0,
			0,0,1,0,
			WinApp::SWindowWidthF_ / 2 + 0,WinApp::SWindowHeightF_ / 2 + 0,0,1
	};

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matVPV;
	matVPV = Camera::nowCamera->matView_;
	matVPV *= Camera::nowCamera->matProjection_;
	matVPV *= matViewport;
	//合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV =matVPV.InverseMatrix();

	Matrix4x4 testmat;

	testmat = matInverseVPV * matVPV;

	//スクリーン座標
	Vector3 posNear = Vector3(reticle_.pos_.x, reticle_.pos_.y, 0);
	Vector3 posFar = Vector3(reticle_.pos_.x, reticle_.pos_.y, 1);

	//スクリーン座標系からワールド座標系へ
	posNear = Matrix4x4::VectorMatDivW(matInverseVPV, posNear);
	posFar = Matrix4x4::VectorMatDivW(matInverseVPV, posFar);

	//マウスレイの方向
	Vector3 mouseDirection = nainavec3(posFar, posNear);
	mouseDirection = mouseDirection.normalize();

	//カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 80;

	Vector3 A = posNear;
	A += Vector3(mouseDirection.x * kDistanceTestObject, mouseDirection.y * kDistanceTestObject, mouseDirection.z * kDistanceTestObject);
	reticle3DObj_.pos_ = A;
	//reticle3DObj_.Scale_ = { 0.05f,0.05f,0.05f };


	reticle_.Update();
	reticle3DObj_.Update();


}

void Player::Damage()
{
	if (isDamage_)
	{

		Vector3 shakevec = lerp(Vector3{ 0,0,0 }, shakeVecSize_, damageTimer_ / damageMaxTimer_);

		playerCamera_.CameraShake(shakevec, shakePow_);


		if (damageTimer_ < damageMaxTimer_)
		{
			damageTimer_++;
		}
		else
		{
			damageTimer_ = 0;
			isDamage_ = false;
			playerCamera_.CameraShake({0,0,0});
		}
	}

	float effectT = DirectXpre::lerp(0.6f, 0.0f, damageEffectTimer_ / damageEffectMaxTime_);

	if (damageEffectTimer_ < damageEffectMaxTime_)
	{
		damageEffectTimer_++;
	}

	damageSprote_.setColor({ 1,0,0,effectT });

}

void Player::MuzzleFlash()
{
	if (flashTimer_ < flashChengTime_)
	{
		flashAlpha_ = DirectXpre::easeOutQuad(0.0f, 1.0f, flashTimer_ / flashChengTime_);
	}
	else
	{
		//光ったので消えるときの動きflashTimerは全体で動いているので統合するために引いている
		flashAlpha_ = DirectXpre::easeInSine(1.0f, 0.0f, (flashTimer_ - flashChengTime_) / flashEndTime_);
	}

	if (flashTimer_ < flashMaxTime_)
	{
		flashTimer_++;
	}

	flashObj_.SetColor({ 1,1,1,flashAlpha_ });

}

void Player::Reload()
{

	reloadTimer = 0;

	bulletNum_ = bulletMaxNum_;
}

void Player::BulletAdd()
{
	//発射地点の為に自キャラの座標をコピー
	Vector3 position = playerObj_.GetWorldPos();
	position += Vector3::normalize(playerObj_.forward_) * 2;

	//移動量を追加
	Vector3 velocity(0, 0, 0);
	velocity = reticle3DObj_.GetWorldPos() - playerObj_.GetWorldPos();
	velocity = velocity.normalize() * bulletSpeed_;

	Matrix4x4 playermat = matScaleGeneration({ 1,1,1 }) * QuaternionMatRotateGeneration({ 0,0,0 }) * matMoveGeneration(playerObj_.GetPos());

	//速度ベクトルを自機の向きの方向に合わせて回転する
	velocity = VectorMat(velocity, playermat);

	//弾の生成と初期化
	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	newBullet->Initlize(position, velocity, { 0.5f,0.5f,0.5f });


	//弾を登録
	bullets_.emplace_back(std::move(newBullet));

	bulletCT_ = bulletMaxCT_;
	muzzleFlashTime_ = muzzleFlashMaxTime_;
	playerObj_.SLightGroup_->SetPointLightPos(0, position);
	playerObj_.SLightGroup_->SetPointLightAtten(0, { 0.1f,0.1f,0.1f });
	playerObj_.SLightGroup_->SetPointLightColor(0, { 1.0f,1.0f,1.0f });

	bulletNum_--;

	if (!isTitle_)
	{
		EmitterManager::GetInstance()->AddSpriteEmitter(bulletSprite_[(uint32_t)bulletNum_].pos_, "BASIC", "Fall", 50.0f, 50.0f, 1.0f, 1.0f, { -10.0f,10.0f }, { -20.0f,-10.0f }, { 1,1 }, { 1,1 }, "Ammo");
	}
	EmitterManager::GetInstance()->AddObjEmitter(playerObj_.GetWorldPos(), "BASIC", "Cartridge", 50.0f, 20.0f, 1.0f, 1.0f, { -0.1f,0.1f }, { 0.1f,0.2f }, { -0.1f,0.1f }, { 0.1f,0.1f,0.1f }, { 0.1f,0.1f,0.1f }, "Ammo");

	XAudio::PlaySoundData(gunShotSount_, 1.0f);

	//マズルフラッシュ用のタイマーをリセット
	flashTimer_ = 0;
}