#include "Player.h"
#include <imgui.h>
#include "Easing.h"
#include "EventPointManager.h"
#include "Quaternion.h"
#include "ModelManager.h"
#include "WinApp.h"

Player::Player()
{

}
Player::~Player()
{

}



void Player::Init(const std::string& directoryPath, const char filename[])
{
	input_ = input_->GetInstance();
	playerObj_.FBXInit();
	reticle3DObj_.objDrawInit(directoryPath, filename,true);
	reticle_.initialize("basketballMan");

	/*bulletModel_ = std::make_unique<AnimationModel>();
	gunModel_ = std::make_unique<AnimationModel>();

	bulletModel_->Load("testGLTFBall", "gltf", "white1x1");
	gunModel_->Load("Gun", "gltf", "gray2x2");*/

	ModelManager::GetInstance()->Load("testGLTFBall", "gltf", "whiteBall", "white1x1");
	ModelManager::GetInstance()->Load("Gun", "gltf", "Gun", "gray2x2");

	bulletModel_ = ModelManager::GetInstance()->SearchModelData("whiteBall");
	gunModel_ = ModelManager::GetInstance()->SearchModelData("Gun");
	
	playerObj_.SetPos({ 0,5,0 });
	playerObj_.SetScale({ 0.13f,0.21f,0.17f });

	playerCamera_.pos_ = { 0,0,-200 };
	playCamera_.eye_ = { 0,0,-3 };

	reticle_.scale_ = { 0.5f,0.5f };

	reticle3DObj_.SetPos({ 0,0,-100 });
	reticle3DObj_.SetScale({ 0.05f,0.05f,0.05f });

	collision = MobCollision("player");

	Collider.SetObject(&collision);

	Collider.Initialize();

	CollisionManager::GetInstance()->AddCollider(&Collider);

	hp1Sprote_.initialize("basketballMan");
	hp2Sprote_.initialize("basketballMan");
	hp3Sprote_.initialize("basketballMan");

	hp1Sprote_.pos_ = { 8,64 };
	hp2Sprote_.pos_ = { 15*2,64 };
	hp3Sprote_.pos_ = { 17*3,64 };

	for (uint16_t i = 0; i < bulletMaxNum_; i++)
	{
		Sprite newBulletSprite;
		newBulletSprite.initialize("basketballMan");
		newBulletSprite.pos_ = { WinApp::SWindowWidth_/2 + newBulletSprite.GetTextureSize().x/2 * i,WinApp::SWindowHeight_- newBulletSprite.GetTextureSize().y/4 };
		newBulletSprite.scale_ = { 0.5f,0.5f };
		bulletSprite_.push_back(newBulletSprite);
	}



}

void Player::Update()
{


	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
	{
		return bullet->IsDead();
	});
	

	if ((input_->PushKey(DIK_SPACE) || input_->GetGamePadButton(XINPUT_GAMEPAD_A)) && time_<=20 and EventPointManager::GetInstance()->GetPEventPoint()->GetEventType() != moveEvent)
	{
		attackFlag_ = true;
		bulletNum_ = bulletMaxNum_;
		
	}

	if (input_->ReleaseKey(DIK_SPACE) || input_->GetGamePadButtonUp(XINPUT_GAMEPAD_A))
	{
		attackFlag_ = false;
		

		
	}
	if (cameraCheng_)
	{
		HideRightWall();
	}
	else
	{
		HideDownWall();
	}

	if (EventPointManager::GetInstance()->GetPEventPoint()->GetEventType()==moveEvent and !EventPointManager::GetInstance()->GetPEventPoint()->GetIsFinished())
	{
		if (moveEventStart_ == false)
		{
			pos_ = playerCamera_.pos_;
			moveVec_ = nainavec3(EventPointManager::GetInstance()->GetPEventPoint()->GetMovePoint(), pos_).normalize();
			moveEventStart_ = true;
		}
		playerCamera_.pos_ += moveVec_;

		if (EventPointManager::GetInstance()->GetPEventPoint()->GetMovePoint() == playerCamera_.pos_)
		{
			moveVec_ = { 0,0,0 };
			EventPointManager::GetInstance()->GetPEventPoint()->SetIsFinished(true);
			moveEventStart_ = false;
		}

	}

	playCamera_.upDate();
	playerCamera_.SetCamera(playCamera_);

	

	//移動(基本的にカメラを基準に)	
	Vector3 playerPos = { playCamera_.eye_.x,playCamera_.eye_.y - 1 ,playCamera_.eye_.z };

	playerObj_.SetPos(playerPos + (playCamera_.forward_ * 5));
	
	playerObj_.Update(/*camera*/);
	Collider.Update(playerObj_.GetWorldPos());

	if (playerObj_.GetWorldPos().x > 50 || playerObj_.GetWorldPos().x < -50)
	{
		moveSpeed_ = -moveSpeed_;
	}

	reticle3DObj_.Update(/*camera*/);

	if (!attackFlag_ and EventPointManager::GetInstance()->GetPEventPoint()->GetEventType() != moveEvent and bulletNum_>0)
	{
		Attack();
	}

	if (muzzleFlashTime_ > 0)
	{
		playerObj_.SLightGroup_->SetPointLightActive(1, true);
		

	}
	else
	{
		playerObj_.SLightGroup_->SetPointLightActive(1, false);
	}

	/*reticle_.pos_ = input_->GetMousePos();
	reticle_.Update();*/

	Reticle2DMouse();
	

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	if (collision.isHit)
	{
		hp_--;
		collision.isHit = false;
	}

	hp1Sprote_.Update();
	hp2Sprote_.Update();
	hp3Sprote_.Update();
	
	for (uint16_t i = 0; i < bulletMaxNum_; i++)
	{
		bulletSprite_[i].Update();
	}



}

void Player::Draw()
{
	playerObj_.FBXDraw(*gunModel_);
	//reticle3DObj_.Draw();
	reticle_.Draw();
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(bulletModel_);
	}

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

}

void Player::Attack()
{
	
	if ((input_->GetMouseButtonDown(0) and bulletCT_ <= 0) or (isDebugShot_ and bulletCT_ <= 0))
	{
		//発射地点の為に自キャラの座標をコピー
		Vector3 position = playerObj_.GetWorldPos();
		position.z += 2;

		//移動量を追加
		Vector3 velocity(0, 0, 0);
		velocity = reticle3DObj_.GetWorldPos() - playerObj_.GetWorldPos();
		velocity = velocity.normalize() * bulletSpeed_;

		Matrix4x4 playermat = matScaleGeneration({ 1,1,1 }) * QuaternionMatRotateGeneration(playerObj_.GetRotate()) * matMoveGeneration(playerObj_.GetPos());

		//速度ベクトルを自機の向きの方向に合わせて回転する
		velocity = VectorMat(velocity, playermat);

		/*ImGui::Begin("player");

		ImGui::Text("velocity:%0.5f,%0.5f,%0.5f", velocity.x, velocity.y, velocity.z);

		ImGui::End();*/

		//弾の生成と初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initlize(position, velocity);

		//弾を登録
		bullets_.emplace_back(std::move(newBullet));

		bulletCT_ = bulletMaxCT_;
		muzzleFlashTime_ = muzzleFlashMaxTime_;
		playerObj_.SLightGroup_->SetPointLightPos(1, position);
		playerObj_.SLightGroup_->SetPointLightAtten(1, {0.1f,0.1f,0.1f});

		bulletNum_--;
		
	}

	if (bulletCT_ > 0)
	{
		bulletCT_--;
	}

	if (muzzleFlashTime_ > 0)
	{
		muzzleFlashTime_--;
	}

	//ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetIsUseGamePad())
	{
		return;
	}

	//Rトリガーを押していたら
	if (Input::GetInstance()->GetGamePadRTrigger() and bulletCT_ <= 0)
	{
		//発射地点の為に自キャラの座標をコピー
		Vector3 position = playerObj_.GetWorldPos();
		position.z += 2;

		//移動量を追加		
		Vector3 velocity(0, 0, 0);
		velocity = reticle3DObj_.GetWorldPos() - playerObj_.GetWorldPos();
		velocity = velocity.normalize() * bulletSpeed_;

		Matrix4x4 playermat = matScaleGeneration({ 1,1,1 }) * QuaternionMatRotateGeneration(playerObj_.GetRotate()) * matMoveGeneration(playerObj_.GetPos());

		//速度ベクトルを自機の向きに合わせて回転する
		velocity = VectorMat(velocity, playermat);

		//弾の生成と初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initlize(position, velocity);

		//弾を登録
		bullets_.emplace_back(std::move(newBullet));

		bulletCT_ = bulletMaxCT_;
		muzzleFlashTime_ = muzzleFlashMaxTime_;

		bulletNum_--;
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
		playCamera_.eye_ = easeOutQuint(Vector3{playerCamera_.pos_.x, playerCamera_.pos_.y, playerCamera_.pos_.z}, Vector3{ playerCamera_.pos_.x+5, playerCamera_.pos_.y, playerCamera_.pos_.z }, time_ / maxMoveTime_);
		playCamera_.target_ = easeOutQuint(Vector3{ playCamera_.eye_.x,playCamera_.eye_.y,playCamera_.eye_.z+1 }, Vector3{ playCamera_.eye_.x-100,playCamera_.eye_.y,playCamera_.eye_.z}, time_ / maxTime_);
		
	}
	else
	{
		if (time_ > 0)
		{
			time_--;
		}
		
		playCamera_.eye_ = easeOutQuint(Vector3{ playerCamera_.pos_.x, playerCamera_.pos_.y, playerCamera_.pos_.z }, Vector3{ playerCamera_.pos_.x + 5, playerCamera_.pos_.y, playerCamera_.pos_.z }, time_ / maxMoveTime_);
		playCamera_.target_ = easeOutQuint(Vector3{ playCamera_.eye_.x,playCamera_.eye_.y,playCamera_.eye_.z + 1 }, Vector3{ playCamera_.eye_.x - 100,playCamera_.eye_.y,playCamera_.eye_.z }, time_ / maxTime_);

	}
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

	
	playCamera_.eye_ = easeOutQuint(Vector3{ playerCamera_.pos_.x, playerCamera_.pos_.y, playerCamera_.pos_.z }, Vector3{ playerCamera_.pos_.x, playerCamera_.pos_.y-2, playerCamera_.pos_.z }, time_ / maxMoveTime_);

	playerCamera_.cameobj_.matWorldGeneration();

	Vector3 forward = { 0.0f, 0.0f, 1.0f };

	forward = VectorMat(forward, playerObj_.GetWorldMat());

	playCamera_.target_ = playCamera_.eye_ + forward;
}

void Player::Reticle2DMouse()
{
	

	POINT mousePosition = {};
	Vector2 spritePosition = reticle_.pos_;

	if (!Input::GetInstance()->GetIsUseGamePad())
	{
		//スクリーン座標を取得
		GetCursorPos(&mousePosition);

		//クライアントエリア座標に変換する
		HWND hwnd = WinApp::GetInstance()->getHwnd();
		ScreenToClient(hwnd, &mousePosition);

		reticle_.pos_=(Vector2((float)mousePosition.x, (float)mousePosition.y));
		//reticle_.pos_=Input::GetInstance()->GetMousePos();
	}
	else
	{
		spritePosition.x += (float)Input::GetInstance()->GetGamePadLStick().x*10;
		spritePosition.y -= (float)Input::GetInstance()->GetGamePadLStick().y * 10;

		reticle_.pos_ = spritePosition;
	}

	/*ImGui::Begin("check");

	ImGui::Text("reticlePos:%0.3f,%0.3f", reticle_.pos_.x, reticle_.pos_.y);

	ImGui::End();*/

	Matrix4x4 matViewport = {
			640,0,0,0,
			0,-360,0,0,
			0,0,1,0,
			640 + 0,360 + 0,0,1
	};

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matVPV;
	matVPV = Camera::nowCamera->matView_;
	matVPV *= Camera::nowCamera->matProjection_;
	matVPV *= matViewport;
	//合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV =matVPV.InverseMatrix();

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
	const float kDistanceTestObject = 100;

	Vector3 A = posNear;
	A += Vector3(mouseDirection.x * kDistanceTestObject, mouseDirection.y * kDistanceTestObject, mouseDirection.z * kDistanceTestObject);
	reticle3DObj_.Trans_ = A;
	reticle3DObj_.Scale_ = { 0.05f,0.05f,0.05f };


	reticle_.Update();
	reticle3DObj_.Update(/*camera*/);


}