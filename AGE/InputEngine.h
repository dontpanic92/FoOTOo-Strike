#ifndef __AGE_INPUTENGINE_HEADER__
#define __AGE_INPUTENGINE_HEADER__

#include "Singleton.h"
#include "Window.h"
#include <ois/OIS.h>
#include "Def.h"
#include <vector>

namespace AGE
{
	class InputEngine;
	class UnbufferedInputBase
	{
	public:
		void ActiveInput() { mActive = true; }
		void DeactiveInput() { mActive = false; }

		virtual void ProcessInput() = 0;

	protected:
		void ProcessInputProtected()
		{
			if (mActive) {
				ProcessInput();
			}
		}

		bool	mActive = true;

		friend class InputEngine;
	};


	class AGE_EXPORT InputEngine : public Singleton < InputEngine >
	{
	public:
		InputEngine() {}
		~InputEngine() { OIS::InputManager::destroyInputSystem(mInputManager); }

		void StartUp();

		void GetKeyStates(char keys[256])
		{
			if (mKeyboard)
				mKeyboard->copyKeyStates(keys);
		}

		const OIS::MouseState& GetMouseState()
		{
			static OIS::MouseState d = OIS::MouseState();
			if (mMouse)
				return mMouse->getMouseState();
			return d;
		}

		void Update()
		{
			if (mKeyboard) {
				mKeyboard->capture();
			}

			if (mMouse) {
				mMouse->capture();
			}

			if (1) {
				NotifyUnbufferedUpdate();
			}
		}

		void RegisterUnbufferedInput(UnbufferedInputBase* input)
		{
			mUnbufferedInputInstances.push_back(input);
		}

		void RegisterKeyListener(OIS::KeyListener* keyListener)
		{
			if (mKeyboard)
				mKeyboard->setEventCallback(keyListener);
		}

		void RegisterMouseListener(OIS::MouseListener* mouseListener)
		{
			if (mMouse)
				mMouse->setEventCallback(mouseListener);
		}

	private:
		void NotifyUnbufferedUpdate()
		{
			for (auto p : mUnbufferedInputInstances) {
				p->ProcessInputProtected();
			}
		}

		OIS::InputManager	*mInputManager = nullptr;
		OIS::Keyboard		*mKeyboard = nullptr;
		OIS::Mouse			*mMouse = nullptr;

		std::vector<UnbufferedInputBase*> mUnbufferedInputInstances;

		//OIS::JoyStick* g_joys[4] = {0,0,0,0};
	};

	inline InputEngine* GetInputEngine() { return InputEngine::GetInstance(); }
}

#endif