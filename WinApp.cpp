#include "WinApp.h"
#pragma comment(lib,"winmm.lib")


WinApp::WinApp()
{

}
WinApp::~WinApp()
{

	// �E�B���h�E�N���X��o�^����
	UnregisterClass(w.lpszClassName, w.hInstance);

}

void WinApp::initialize()
{

#pragma region �E�B���h�E�̏���������
	

	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = static_cast<WNDPROC>(windowProc);//�E�B���h�E�v���V�[�W��
	w.lpszClassName = L"DirectXGame";//�E�B���h�E�N���X��
	w.hInstance = GetModuleHandle(nullptr);//�E�B���h�E�n���h��
	w.hCursor = LoadCursor(NULL, IDC_ARROW);//�J�[�\���w��

	//�E�B���h�E�N���X��OS�ɓo�^
	RegisterClassEx(&w);

	//�E�B���h�E�̃T�C�Y�̍\����{x���W,y���W,����,�c��}
	wrc = { 0,0,windowWidth,windowHeight };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//�����ŃT�C�Y�␳

	//�E�B���h�E�̍\���v�f�H
	hwnd = CreateWindow(w.lpszClassName,//�N���X��
		L"DirectXGame",//�^�C�g���o�[�̖��O
		WS_OVERLAPPEDWINDOW,//�W���I�ȃE�C���h�E�X�^�C��
		CW_USEDEFAULT,//x���W(OS�ɔC����)
		CW_USEDEFAULT,//y���W(OS�ɔC����)
		wrc.right - wrc.left,//�E�B���h�E����
		wrc.bottom - wrc.top,//�E�B���h�E�c��
		nullptr,//�e�E�B���h�E�n���h��
		nullptr,//���j���[�n���h��
		w.hInstance,//�Ăяo���A�v���P�[�V�����n���h��
		nullptr);//�I�v�V����

	//�E�B���h�E�\������
	ShowWindow(hwnd, SW_SHOW);

	//�V�X�e���^�C�}�[�̕���\���グ��
	timeBeginPeriod(1);

}


void WinApp::updare()
{

}

bool WinApp::processMassage()
{

	//���b�Z�[�W������H
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);//�L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg);//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	//�~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
	if (msg.message == WM_QUIT)
	{
		return true;
	}

	return false;

}

WNDCLASSEX WinApp::getW()
{

	return w;

}

RECT WinApp::getWrc()
{

	return wrc;

}

HWND WinApp::getHwnd()
{

	return hwnd;

}


int WinApp::getWindowSizeWidth()
{

	return windowWidth;

}


int WinApp::getWindowSizeHeight()
{

	return windowHeight;

}

#pragma region �E�B���h�E�v���V�[�W��


LRESULT WinApp::windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY://�E�B���h�E���j�����ꂽ
		PostQuitMessage(0);//OS�ɑ΂��āA�I��������Ƃ�`����
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//�W���̏���������
}

#pragma endregion