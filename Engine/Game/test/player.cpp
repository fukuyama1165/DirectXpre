#include "player.h"
#include <imgui.h>
#include "Easing.h"

player::player()
{

}
player::~player()
{

}



void player::Init(const std::string& directoryPath, const char filename[])
{
	input_ = input_->GetInstance();
	playerObj_.objDrawInit(directoryPath, filename,true);
	reticle3DObj_.objDrawInit(directoryPath, filename,true);
	reticle_.initialize(SpriteCommon::GetInstance(), 1);

	bulletModel_ = std::make_unique<AnimationModel>();

	bulletModel_->Load("testGLTFBall", "gltf", "white1x1");

	
	playerObj_.SetPos({ -20,0,0 });
	playerObj_.SetScale({ 0.05f,0.05f,0.05f });

	playerCamera_.pos_ = { 0,0,-200 };
	hogeca.eye_ = { 0,0,-3 };

	reticle_.scale_ = { 0.5f,0.5f };
}

void player::Update(const Camera& camera)
{
	moveVec_ = { 0,0,0 };

	//デスフラグの立った弾を削除(remove_if->条件一致を全て削除)
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)//ifの中で簡易的な関数を生成してる->[](引数)
	{
		return bullet->IsDead();
	});
	

	if (input_->PushKey(DIK_SPACE) && time_<=20)
	{
		attackFlag_ = true;
		
	}

	if (input_->ReleaseKey(DIK_SPACE))
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

	hogeca.upDate();
	playerCamera_.SetCamera(hogeca);

	

	//移動

	if (moveVec_.x != 0 || moveVec_.z != 0)
	{

		playerObj_.Trans_ += moveVec_ * moveSpeed_;
		//playerCamera_.pos_ += moveVec_ * moveSpeed_;
	}
	
	
	playerObj_.Update(camera);

	if (playerObj_.GetWorldPos().x > 50 || playerObj_.GetWorldPos().x < -50)
	{
		moveSpeed_ = -moveSpeed_;
	}

	Attack();

	/*reticle_.pos_ = input_->GetMousePos();
	reticle_.Update();*/

	Reticle2DMouseAttack(camera);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update(camera);
	}
	
}

void player::Draw()
{
	playerObj_.Draw();
	reticle3DObj_.Draw();
	reticle_.Draw();
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(bulletModel_.get());
	}
}

void player::Attack()
{
	
	if (input_->TriggerKey(DIK_N) and bulletCT_ <= 0)
	{
		//発射地点の為に自キャラの座標をコピー
		Vector3 position = playerObj_.GetWorldPos();
		position.z += 2;

		//移動量を追加
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, 0);
		velocity = reticle3DObj_.GetWorldPos() - playerObj_.GetWorldPos();
		velocity = velocity.normalize() * kBulletSpeed;

		//速度ベクトルを自機の向きに合わせて回転する
		velocity = VectorMat(velocity, playerObj_.GetWorldMat());

		//弾の生成と初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initlize(position, velocity);

		//弾を登録
		bullets_.emplace_back(std::move(newBullet));

		bulletCT_ = 5;
	}

	if (bulletCT_ > 0)
	{
		bulletCT_--;
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
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, 0);
		velocity = reticle3DObj_.GetWorldPos() - playerObj_.GetWorldPos();
		velocity = velocity.normalize() * kBulletSpeed;

		//速度ベクトルを自機の向きに合わせて回転する
		velocity = VectorMat(velocity, playerObj_.GetWorldMat());

		//弾の生成と初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initlize(position, velocity);

		//弾を登録
		bullets_.emplace_back(std::move(newBullet));

		bulletCT_ = 5;
	}



}

void player::HideRightWall()
{
	if (attackFlag_)
	{
		if (time_ < maxTime_/15)
		{
			time_++;
		}
		hogeca.eye_ = easeOutQuint(Vector3{playerCamera_.pos_.x, playerCamera_.pos_.y, playerCamera_.pos_.z}, Vector3{ playerCamera_.pos_.x+5, playerCamera_.pos_.y, playerCamera_.pos_.z }, time_ / maxMoveTime_);
		hogeca.target_ = easeOutQuint(Vector3{ hogeca.eye_.x,hogeca.eye_.y,hogeca.eye_.z+1 }, Vector3{ hogeca.eye_.x-100,hogeca.eye_.y,hogeca.eye_.z}, time_ / maxTime_);
		
	}
	else
	{
		if (time_ > 0)
		{
			time_--;
		}
		/*hogeca.eye_ = easeOutQuint(Vector3{ playerCamera_.pos_.x + 5, playerCamera_.pos_.y, playerCamera_.pos_.z }, Vector3{ playerCamera_.pos_.x, playerCamera_.pos_.y, playerCamera_.pos_.z }, 1.0f - (time_ / maxTime_));
		hogeca.target_ = easeOutQuint(Vector3{ hogeca.eye_.x - 100,hogeca.eye_.y,hogeca.eye_.z }, Vector3{ hogeca.eye_.x,hogeca.eye_.y,hogeca.eye_.z + 1 }, 1.0f - (time_ / maxTime_));*/
		
		hogeca.eye_ = easeOutQuint(Vector3{ playerCamera_.pos_.x, playerCamera_.pos_.y, playerCamera_.pos_.z }, Vector3{ playerCamera_.pos_.x + 5, playerCamera_.pos_.y, playerCamera_.pos_.z }, time_ / maxMoveTime_);
		hogeca.target_ = easeOutQuint(Vector3{ hogeca.eye_.x,hogeca.eye_.y,hogeca.eye_.z + 1 }, Vector3{ hogeca.eye_.x - 100,hogeca.eye_.y,hogeca.eye_.z }, time_ / maxTime_);

	}
}

void player::HideDownWall()
{
	if (attackFlag_)
	{
		if (time_ < maxTime_)
		{
			time_++;
		}
		
	}
	else
	{
		time_ = 0;
	}

	//hogeca.target_ = easeOutQuint(Vector3{ 0,0,1 }, Vector3{ 0,0,1 }, time_ / maxTime_);
	hogeca.eye_ = easeOutQuint(Vector3{ playerCamera_.pos_.x, playerCamera_.pos_.y, playerCamera_.pos_.z }, Vector3{ playerCamera_.pos_.x, playerCamera_.pos_.y-10, playerCamera_.pos_.z }, time_ / maxMoveTime_);

	playerCamera_.cameobj_.matWorldGeneration();

	Vector3 forward = { 0.0f, 0.0f, 1.0f };

	forward = VectorMat(forward, playerObj_.GetWorldMat());

	hogeca.target_ = hogeca.eye_ + forward;
}

void player::Reticle2DMouseAttack(Camera camera)
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
		spritePosition.x += (float)Input::GetInstance()->GetGamePadLStick().x / SHRT_MAX * 7.0f;
		spritePosition.y -= (float)Input::GetInstance()->GetGamePadLStick().y / SHRT_MAX * 7.0f;

		//reticle_.pos_ = spritePosition;
	}



	Matrix4x4 matViewport = {
			640,0,0,0,
			0,-360,0,0,
			0,0,1,0,
			640 + 0,360 + 0,0,1
	};

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matVPV;
	matVPV = camera.matView_;
	matVPV *= camera.matProjection_;
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
	reticle3DObj_.Update(camera);


}