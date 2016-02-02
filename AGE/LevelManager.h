#ifndef __AGE_LEVELMANAGER_HEADER__
#define __AGE_LEVELMANAGER_HEADER__

#include <string>

#include "Def.h"
#include "Singleton.h"
#include "Scene.h"


namespace AGE 
{

	class AGE_EXPORT Level 
	{
	public:
		virtual bool StartUp() = 0;
		virtual void ShutDown() = 0;
		virtual Scene* GetScene() = 0;
		virtual bool Update(float time) = 0;

		virtual ~Level() {};
	};

	class AGE_EXPORT LevelManager : public Singleton < LevelManager >
	{
	public:
		~LevelManager() { UnloadLevel(); }

		template<class LevelType, typename ...Args>
		bool LoadLevel(Args&&... Param)
		{
			UnloadLevel();
			mLevel = new LevelType(std::forward<Args>(Param)...);
			if (!mLevel->StartUp()) {
				mLevel->ShutDown();
				delete mLevel;
				mLevel = 0;
				return false;
			}
			//auto f = [this]() -> LevelType*{ return (LevelType*)mLevel; };

			return true;
		}

		void UnloadLevel()
		{
			if (mLevel) {
				mLevel->ShutDown();
				delete mLevel;
				mLevel = 0;
			}
		}

		Level* GetLevel()
		{
			return mLevel;
		}

		//For convenience
		template<class LevelType>
		LevelType* GetLevel()
		{
			return (LevelType*)mLevel;
		}

	private:
		Level* mLevel = 0;
	};

	static inline LevelManager* GetLevelManager()
	{
		return LevelManager::GetInstance();
	}

	static inline Level* GetLevel()
	{
		return GetLevelManager()->GetLevel();
	}

	template<class LevelType>
	static inline LevelType* GetLevel()
	{
		return (LevelType*)GetLevelManager()->GetLevel();
	}

	static inline Scene* GetScene()
	{
		return GetLevel() ? GetLevel()->GetScene() : 0;
	}
}

#endif
