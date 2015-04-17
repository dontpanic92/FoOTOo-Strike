// FoOTOo-Strike.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <AGE.h>
#include "GameLogicImp.h"

int main(int argc, TCHAR* argv[])
{
	GameLogicImp gameLogicImp;
	AGE::Engine::GetInstance()->StartUp();
	AGE::Engine::GetInstance()->SetGameLogic(&gameLogicImp);
	AGE::Engine::GetInstance()->Run();

	return 0;
}

