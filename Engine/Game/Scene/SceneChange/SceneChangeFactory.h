#pragma once

#include "ISceneChangeFactory.h"

//�p�����Ă��闝�R
//����Ă�����̂��ς�������ɃV�[�������������ɐV���ɏ����������邵
//�ς���̂��ȒP�ɂȂ邽��

class SceneChangeFactory :public ISceneChangeFactory
{
public:
	SceneChangeFactory();
	~SceneChangeFactory();

	/// <summary>
	/// �V�[������
	/// </summary>
	/// <param name="sceneName">�V�[����</param>
	/// <returns>���������V�[��</returns>
	std::unique_ptr<ISceneChange> CreateSceneChange(const std::string& sceneChangeName)override;

private:

};