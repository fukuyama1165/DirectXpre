#include "CountTimer.h"
#include <imgui.h>
#include "Input.h"
#include "Easing.h"

CountTimer::CountTimer()
{
}

CountTimer::~CountTimer()
{
}

void CountTimer::Initialize()
{
	timer_ = MaxTime_;
	
	Texture::GetInstance()->loadTexture("Resources/numbers.png", "numbers");
	for (uint32_t i = 0; i < 4; i++)
	{
		num[i].initialize("numbers");
	}
}

void CountTimer::Initialize(float maxTime)
{
	timer_ = maxTime;
	Texture::GetInstance()->loadTexture("Resources/numbers.png", "numbers");
	for (uint32_t i = 0; i < 4; i++)
	{
		num[i].initialize("numbers");
	}

	damageSprote_.initialize("white1x1");

	damageSprote_.pos_ = { WinApp::SWindowWidthF_ / 2,WinApp::SWindowHeightF_ / 2 };
	damageSprote_.scale_ = { WinApp::SWindowWidthF_,WinApp::SWindowHeightF_ };
	//透明に
	damageSprote_.setColor({ 1.0f,1.0f, 1.0f, 0.0f });
}

void CountTimer::Finalize()
{

}

void CountTimer::Update()
{
	//引く
	if (timer_ > 0)
	{
		timer_--;
	}

	if (timer_ == 0)
	{
		isZero_ = true;
		timeUpEffectTime_ = 0;
	}

	//百秒を超えないように(見た目を用意してないから)
	if (timer_ > 6000)
	{
		timer_ = 6000;
	}

	//加算
	if (addTime_ > 0)
	{
		if (addTime_ > 100)
		{
			timer_+=100;
			addTime_-=100;
		}
		else if (addTime_ > 10)
		{
			timer_ += 10;
			addTime_ -= 10;
		}
		else if (addTime_ > 0)
		{
			timer_ += 1;
			addTime_ -= 1;
		}
	}

	if (addTime_ > 0 && sizeAddTimer_ < sizeAddMaxTime_)
	{
		sizeAddTimer_++;
	}
	else if(addTime_ == 0 && sizeAddTimer_ > 0)
	{
		sizeAddTimer_--;
	}

	int32_t now_ = 0;
	int32_t count_ = 0;

	int32_t ans = Numreturn(timer_/60, now_, count_);

#ifdef _DEBUG


	if (Input::GetInstance()->TriggerKey(DIK_6))
	{
		AddTimer(600);
	}

#endif
	//時間系の演出
	if (addTime_ > 0 || sizeAddTimer_ > 0)
	{
		//時間が増えた時
		for (int32_t i = 0; i < 4; i++)
		{
			num[i].scale_ = easeInSine(Vector2{ 1,1 }, Vector2{ 3,3 }, sizeAddTimer_ / sizeAddMaxTime_);
		}
	}
	else if(timer_< timeUpEffectStartTime_)
	{
		//時間が切れそうな時の演出
		for (int32_t i = 0; i < 4; i++)
		{
			num[i].scale_ = { 1.0f + (sinf(timeUpEffectTime_) / 2), 1.0f + (sinf(timeUpEffectTime_)/2) };

		}
		timeUpEffectTime_+=0.1f;

		if (timeUpScreenEffectTime_ >= timeUpScreenEffectMaxTime_)
		{
			isadd_ = false;
		}
		else if (timeUpScreenEffectTime_ <= 0)
		{
			isadd_ = true;
		}

		if (isadd_)
		{
			timeUpScreenEffectTime_++;
		}
		else
		{
			timeUpScreenEffectTime_--;
		}

		Vector4 color = { 1.0f,0.0f, 0.0f, 0.0f };
		color.w = lerp(0.0f, 0.03f, timeUpScreenEffectTime_ / timeUpScreenEffectMaxTime_);
		damageSprote_.setColor(color);
		damageSprote_.Update();

	}
	else if (timer_ >= timeUpEffectStartTime_)
	{
		timeUpEffectTime_ = 0;
		damageSprote_.setColor({ 1.0f,1.0f, 1.0f, 0.0f });
	}

	//数の画像を読み込んでいるので横のサイズは1/10になるので割っている
	GraphNumberDisplayScore({100,100}, ans, 4,num[0].GetTextureSize().x/10, num[0].GetTextureSize().y);

	for (uint32_t i = 0; i < 4; i++)
	{
		num[i].Update();
	}

	

	//たぶん描画するだけ



}

void CountTimer::ImguiUpdate()
{
#ifdef _DEBUG

	ImGui::Begin("CountTimerTest");

	ImGui::DragFloat("timer", &timer_, 1.0f, 0.0f, 6000.0f);

	ImGui::End();


#endif
}

void CountTimer::Draw()
{
	
	for (Sprite aa : num)
	{
		aa.Draw();
	}

	damageSprote_.Draw();
}

//10の累乗
int32_t exponentiation10(int32_t a)
{
	int ans = 0;
	if (a == 0)
	{
		return 1;
	}

	for (int i = 0; i < a; i++)
	{
		if (ans == 0)
		{
			ans = 10;
		}
		else
		{
			ans = ans * 10;
		}
	}

	return ans;
}

void CountTimer::GraphNumberDisplayScore(Vector2 pos, int32_t number, int32_t rank, float w, float h)
{
	if (rank > 4)
	{
		rank = 4;
	}
	int32_t outputNumber = 0;
	for (int32_t i = rank; i > 0; i--)
	{
		outputNumber = number / exponentiation10(i-1);
		if (outputNumber > 9)
		{
			outputNumber = outputNumber % 10;
		}

		if (i < 3)
		{
			num[i - 1].SetTextureRect(w * (float)outputNumber, 0.0f, w, h);
			num[i - 1].pos_ = { pos.x + w * (rank - (i - 1)) ,pos.y };
		}
		else
		{
			num[i - 1].SetTextureRect(w * (float)outputNumber, 0.0f, w, h);
			num[i - 1].pos_ = { pos.x + w * (rank - ((i + 1) - 1)) ,pos.y };
		}

		
	}

}

int32_t CountTimer::Numreturn(float number, int32_t& now, int32_t& count)
{

	
	int32_t buff = (int32_t)number;

	if (count == 0)
	{
		now = buff;
	}
	else if(count<3)
	{
		now *= 10;
		now += buff;
	}

	if (number - buff != 0 && count < 3)
	{
		count++;
		Numreturn((number - buff)*10, now, count);
	}
	else
	{
		return now;
	}

	return now;

}

