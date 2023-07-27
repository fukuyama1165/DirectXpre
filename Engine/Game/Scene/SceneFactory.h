#pragma once

#include "ISceneFactory.h"

//�p�����Ă��闝�R
//����Ă�����̂��ς�������ɃV�[�������������ɐV���ɏ����������邵
//�ς���̂��ȒP�ɂȂ邽��

class SceneFactory:public ISceneFactory
{
public:
	SceneFactory();
	~SceneFactory();

	/// <summary>
	/// �V�[������
	/// </summary>
	/// <param name="sceneName">�V�[����</param>
	/// <returns>���������V�[��</returns>
	std::unique_ptr<IScene> CreateScene(const std::string& sceneName)override;

private:

};

