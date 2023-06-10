#include<Windows.h>

#include "GameScene.h"


int WINAPI WinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE, _In_ LPSTR,_In_ int)
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::unique_ptr<IScene> game = std::make_unique<GameScene>();

	game->Ran();
	

	return 0;


}

