#pragma once
#include "IScene.h"
#include "Object3D.h"
#include "cameraObj.h"
#include "Sprite.h"

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
	Object3D objobj3_;

	cameraObj cameobj_;

	std::string textureNum_ = "";

	Sprite title_;

};
