#include "CountTimer.h"
#include <imgui.h>

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
	}

	ImGui::Begin("debug");

	ImGui::DragInt("drowNum", &testnum, 1, 0, 9999);

	ImGui::End();

	//数の画像を読み込んでいるので横のサイズは1/10になるので割っている
	graphNumberDisplayScore({100,100}, testnum, 4,num[0].GetTextureSize().x/10, num[0].GetTextureSize().y);
	for (uint32_t i = 0; i < 4; i++)
	{
		num[i].Update();
	}

	//たぶん描画するだけ

}

void CountTimer::Draw()
{
	
	for (Sprite aa : num)
	{
		aa.Draw();
	}
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

void CountTimer::graphNumberDisplayScore(Vector2 pos, int32_t number, int32_t rank, float w, float h)
{
	w; h;
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

		num[i-1].SetTextureRect(w * (float)outputNumber, 0.0f, 32, 32);
		num[i-1].pos_ = { pos.x + w * (rank - (i-1)) ,pos.y };

		
	}

}

