#pragma once
#include "IScene.h"

class TitleScene : public IScene
{
public:
	TitleScene();
	~TitleScene();

	void Initialize()override;

	void Finalize()override;

	void Update()override;

	void Draw()override;

private:

};
