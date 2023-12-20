#include "CountTimer.h"
#include <imgui.h>
#include "Input.h"

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

	if (timer_ > 6000)
	{
		timer_ = 6000;
	}

	int32_t now_ = 0;
	int32_t count_ = 0;

	int32_t ans = Numreturn(timer_/60, now_, count_);
	ans;

	if (Input::GetInstance()->TriggerKey(DIK_6))
	{
		AddTimer(600);
	}

	//数の画像を読み込んでいるので横のサイズは1/10になるので割っている
	GraphNumberDisplayScore({100,100}, ans, 4,num[0].GetTextureSize().x/10, num[0].GetTextureSize().y);
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

void CountTimer::GraphNumberDisplayScore(Vector2 pos, int32_t number, int32_t rank, float w, float h)
{
	h;
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
			num[i - 1].SetTextureRect(w * (float)outputNumber, 0.0f, 32, 32);
			num[i - 1].pos_ = { pos.x + w * (rank - (i - 1)) ,pos.y };
		}
		else
		{
			num[i - 1].SetTextureRect(w * (float)outputNumber, 0.0f, 32, 32);
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

