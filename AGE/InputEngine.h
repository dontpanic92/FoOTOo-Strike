#ifndef __AGE_INPUTENGINE_HEADER__
#define __AGE_INPUTENGINE_HEADER__

#include "Singleton.h"
#include "Window.h"
#include <ois/OIS.h>

namespace AGE
{
	class InputEngine : public Singleton < InputEngine >
	{
	public:
		InputEngine() {}
		~InputEngine() { OIS::InputManager::destroyInputSystem(mInputManager); }

		void StartUp(Window window);

		void GetKeyStates(char keys[256])
		{
			mKeyboard->copyKeyStates(keys);
		}

		const OIS::MouseState& GetMouseState()
		{
			return mMouse->getMouseState();
		}

		//OIS::Keyboard * GetKeyboard(){return mKeyboard;}
		void Update()
		{
			mKeyboard->capture(); mMouse->capture();
		}

		void RegisterKeyListener(OIS::KeyListener* keyListener)
		{
			mKeyboard->setEventCallback(keyListener);
		}

		void RegisterMouseListener(OIS::MouseListener* mouseListener)
		{
			mMouse->setEventCallback(mouseListener);
		}

	private:
		OIS::InputManager	*mInputManager;
		OIS::Keyboard		*mKeyboard;
		OIS::Mouse			*mMouse;
		//OIS::JoyStick* g_joys[4] = {0,0,0,0};
	};

	//inline InputEngine* GetInputEngine() { return InputEngine::GetInstance(); }
}

#endif