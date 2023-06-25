#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{

public:
	PostEffect();

	void Initialize(uint32_t ChangeTexure = 0);

	void Draw(uint32_t ChangeTexure = 0, uint16_t PipeLineRuleFlag = 0);

};

