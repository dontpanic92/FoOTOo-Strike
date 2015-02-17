#include "Engine.h"
#include "Primitive.h"
#include "AGEMeshImporter.h"
#include "Timer.h"
#include "PhysicsEngine.h"
#include "RtInfomation.h"
using namespace AGE;

Engine::Engine() :mLastTimeUpdate(0), mScene(0)
{
	mGameLogic = new GameLogicImp();
	mScene = new Scene();
}

int Engine::StartUp()
{
	mApp.StartUp();
	RenderEngine::SetEngineType(RenderEngine::Type::OpenGL);
	RenderEngine::GetInstance()->StartUp(mApp.GetMainWindow());
	InputEngine::GetInstance()->StartUp(mApp.GetMainWindow());
	PhysicsEngine::GetInstance()->StartUp();
	Timer::GetInstance()->StartUp();

	mScene->StartUp();

	mGameLogic->StartUp();

	return 0;
}

int Engine::Run()
{
	return mApp.MainLoop();
}

Engine::~Engine()
{
	delete mScene;
	delete mGameLogic;
}

int Engine::Update()
{

	float now = Timer::GetInstance()->GetTotalMilliSeconds();
	float delta = now - mLastTimeUpdate;
	//if (delta < 10)
	//return 1;

	RtInfomation::GetInstance()->FrameStart();

	InputEngine::GetInstance()->Update();
	PhysicsEngine::GetInstance()->Update(delta);
	mGameLogic->Update(delta);
	RtInfomation::GetInstance()->FrameLogicEnd();
	//mScene->Render();
	mScene->UpdateAndCulling();
	RtInfomation::GetInstance()->FrameCullingEnd();
	RenderEngine::GetInstance()->Render();
	RtInfomation::GetInstance()->FrameEnd();


	mLastTimeUpdate = now;
	return 1;
}
