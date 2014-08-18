#include <Windows.h>
#include "Application.h"
#include "Engine.h"

using namespace AGE;

int WindowsApplication::MainLoop(){

	bool bRunning = true;
	while(bRunning)
	{
		MSG msg;
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			if(msg.message == WM_QUIT){
				bRunning = false;
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(!Engine::GetInstance()->Update()){
			bRunning = false;
			break;
		}
	}

	return 0;
}

int WindowsApplication::StartUp(){
	CreateApplicationWindow();
	return true;
}

Window WindowsApplication::GetMainWindow(){
	return mWindow;
}

LRESULT CALLBACK WindowEventHandler (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch(message){
	case WM_ERASEBKGND:
		return 1;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

bool WindowsApplication::CreateApplicationWindow(){
	HWND hWnd;
	WNDCLASSEX ageWindow;
	ageWindow.cbSize             =      sizeof(ageWindow);
	ageWindow.style              =      CS_HREDRAW|CS_VREDRAW;
	ageWindow.lpfnWndProc        =      WindowEventHandler;
	ageWindow.cbClsExtra         =      0;
	ageWindow.cbWndExtra         =      0;
	ageWindow.hInstance          =      0;
	ageWindow.hIcon              =      ::LoadIcon(NULL,IDI_APPLICATION);
	ageWindow.hCursor            =      ::LoadCursor(NULL,IDC_ARROW);
	ageWindow.hbrBackground      =      (HBRUSH)::GetStockObject(BLACK_BRUSH);
	ageWindow.lpszMenuName       =      NULL;
	ageWindow.lpszClassName      =      "AGE Application";
	ageWindow.hIconSm            =      NULL;

	RegisterClassEx(&ageWindow);

	hWnd = ::CreateWindow("AGE Application", "AGE Application", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, NULL, NULL);

	if (!hWnd)
	{
		return false;
	}

	mWindow.hWnd = hWnd;
	mWindow.hDC = ::GetWindowDC(hWnd);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	return true;
}
