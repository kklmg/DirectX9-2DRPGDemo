#include"Global_data.h"
#include"ChangeFun.h"

void __LoadInGame()
{
	g_pGameMng->CallPlayScene();
}

void __LoadMenu()
{
	g_pGameMng->CallMenuScene();
}

void __QuitGame()
{
	PostQuitMessage(0);
}