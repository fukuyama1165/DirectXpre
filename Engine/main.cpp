#include<Windows.h>

#include "GameScene.h"


int32_t WINAPI WinMain(_In_ HINSTANCE,_In_opt_ HINSTANCE, _In_ LPSTR,_In_ int32_t)
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::unique_ptr<IScene> game = std::make_unique<GameScene>();

	game->Ran();
	

	return 0;


}

