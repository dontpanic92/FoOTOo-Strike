// FoOTOo-Strike.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <AGE.h>
#include "SimpleLevel.h"

int main(int argc, TCHAR* argv[])
{
	AGE::Engine::GetInstance()->StartUp();
	GetLevelManager()->LoadLevel<SimpleLevel>();
	AGE::Engine::GetInstance()->Run();
	return 0;
}

