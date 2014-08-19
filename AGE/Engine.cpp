#include "Engine.h"
#include "Primitive.h"
//#include "InputEngine.h"
#include "Timer.h"
using namespace AGE;

Engine::Engine():mLastTimeUpdate(0){
	mScene = new Scene();
	mGameLogic = new GameLogicImp();
}

int Engine::StartUp(){
	mApp.StartUp();
	RenderEngine::GetInstance()->StartUp(mApp.GetMainWindow());
	InputEngine::GetInstance()->StartUp(mApp.GetMainWindow());
	Timer::GetInstance()->StartUp();

	mScene->GetRoot()->Attach(mScene->LoadMesh());


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
	/*Vector3f points[4];

	points[0].Set(0.5, 0.5, 0);
	points[1].Set(0.5, -0.5, 0);
	points[2].Set(-0.5, 0.5, 0);
	points[3].Set(-0.5, -0.5, 0);
	static Renderable* mesh = Primitive::GetInstance()->CreatePlaneUnmanage(points);

	//mRenderer->TestRender();
	mRenderer->RenderMesh(mesh->GetMesh());*/
	//RenderEngine::GetInstance()->RenderScene(mScene);
	InputEngine::GetInstance()->Update();
	float now = Timer::GetInstance()->GetTotalMilliSeconds();
	mGameLogic->Update(now - mLastTimeUpdate);
	mScene->Render();

	mLastTimeUpdate = now;
	return 1;
}
