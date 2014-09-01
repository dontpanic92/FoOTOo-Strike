#include "Engine.h"
#include "Primitive.h"
#include "3DSMeshImporter.h"
#include "Timer.h"
using namespace AGE;

Engine::Engine():mLastTimeUpdate(0), mScene(0){
	mGameLogic = new GameLogicImp();
}

int Engine::StartUp(){
	mApp.StartUp();
	RenderEngine::GetInstance()->StartUp(mApp.GetMainWindow());
	InputEngine::GetInstance()->StartUp(mApp.GetMainWindow());
	Timer::GetInstance()->StartUp();

	//mScene->GetRoot()->Attach(mScene->LoadMesh());
	//if (mScene)
	delete mScene;

	AGE3DSMeshImporter importer;
	mScene = importer.LoadSceneFromFile();
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
	if (delta < 10)
		return 1;
	InputEngine::GetInstance()->Update();
	mGameLogic->Update(delta);
	mScene->Render();

	mLastTimeUpdate = now;
	return 1;
}
