#include "Engine.h"
#include "Primitive.h"
#include "AGEMeshImporter.h"
#include "Timer.h"
using namespace AGE;

Engine::Engine():mLastTimeUpdate(0), mScene(0){
	mGameLogic = new GameLogicImp();
	mScene = new Scene();
}

int Engine::StartUp(){
	mApp.StartUp();
	RenderEngine::GetInstance()->StartUp(mApp.GetMainWindow());
	InputEngine::GetInstance()->StartUp(mApp.GetMainWindow());
	Timer::GetInstance()->StartUp();

	mScene->StartUp();

	mGameLogic->StartUp();

	return 0;
}

int Engine::Run(){
	return mApp.MainLoop();
}

Engine::~Engine(){
	delete mScene;
	delete mGameLogic;
}

int Engine::Update(){

	float now = Timer::GetInstance()->GetTotalMilliSeconds();
	float delta = now - mLastTimeUpdate;
	//if (delta < 10)
		//return 1;
	InputEngine::GetInstance()->Update();
	mGameLogic->Update(delta);
	mScene->Render();

	mLastTimeUpdate = now;
	return 1;
}
