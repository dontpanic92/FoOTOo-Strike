#include "InputEngine.h"
#include "Engine.h"
#include <sstream>

using namespace AGE;
using namespace OIS;
using namespace std;

void InputEngine::StartUp(Window window){
	ParamList pl;
	std::ostringstream wnd;
	wnd << (size_t)window.hWnd;
	pl.insert(std::make_pair( std::string("WINDOW"), wnd.str() ));

	mInputManager = InputManager::createInputSystem(pl);
	mInputManager->enableAddOnFactory(InputManager::AddOn_All);

	mKeyboard = (Keyboard*)mInputManager->createInputObject(OISKeyboard, false);
	mMouse = (Mouse*)mInputManager->createInputObject(OISMouse, true);
}
