#pragma once

#include<Windows.h>

//WindouwsAPI
class WinApp
{

public:

	WinApp();
	~WinApp();

	//������
	void initialize();



	//���b�Z�[�W�̏���
	bool processMassage();

	//�E�C���h�E�N���X�̐ݒ��Ԃ��֐�
	//�\���̂�getter�͉��̂悤�ɂ���(����������s�ɂ������ɂ���\���̂����ɗ����悤�ɂ��Ă���)
	const WNDCLASSEX& getW() const;

	//��ʃT�C�Y�������Ă���ϐ���Ԃ��֐�
	const RECT& getWrc() const;

	//HWND��Ԃ��֐�
	HWND getHwnd() const;

	//�E�C���h�E�̉�����Ԃ��֐�
	int getWindowSizeWidth();

	//�E�C���h�E�̏c����Ԃ��֐�
	int getWindowSizeHeight();

	//�ÓI�����o�֐�
public:
	//�E�C���h�E�v���V�[�W��
	static LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:

	const int windowWidth = 1280;
	const int windowHeight = 720;

	WNDCLASSEX w = {};//�E�B���h�E�N���X�̐ݒ�

	RECT wrc = {};

	HWND hwnd;

	//����������Ȃ��ƃE�B���h�E����u�ŏ����邽�ߋL�q
	MSG msg{};

};

