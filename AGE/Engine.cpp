#include "Engine.h"
#include "Primitive.h"
#include "AGEMeshImporter.h"
#include "Timer.h"
#include "PhysicsEngine.h"
#include "AudioEngine.h"
#include "RtInfomation.h"
using namespace AGE;

Engine::Engine() :mLastTimeUpdate(0), mScene(0)
{
	mScene = new Scene();
	mGameLogic = 0;
}

int Engine::StartUp()
{
	mApp.StartUp();
	RenderEngine::SetEngineType(RenderEngine::Type::Direct3D11);
	RenderEngine::GetInstance()->StartUp(mApp.GetMainWindow());
	InputEngine::GetInstance()->StartUp(mApp.GetMainWindow());
	PhysicsEngine::GetInstance()->StartUp();
	AudioEngine::GetInstance()->StartUp(); 
	Timer::GetInstance()->StartUp();

	mScene->StartUp();

	//mGameLogic->StartUp();

	return 0;
}

void Engine::ShutDown()
{
	//mGameLogic->ShutDown();
	//mScene->ShutDown();

	//Timer::GetInstance()->ShutDown();
	AudioEngine::GetInstance()->ShutDown();
	//PhysicsEngine::GetInstance()->ShutDown();
	//InputEngine::GetInstance()->ShutDown();
	//RenderEngine::GetInstance()->ShutDown();
	//mApp.ShutDown();
}

int Engine::Run()
{
	return mApp.MainLoop();
}

Engine::~Engine()
{
	delete mScene;
}

void Engine::SetGameLogic(GameLogic* logic)
{
	mGameLogic = logic;
	logic->StartUp();
}

int Engine::Update()
{

	float now = Timer::GetInstance()->GetTotalMilliSeconds();
	float delta = now - mLastTimeUpdate;
	//if (delta < 10)
	//	return 1;

	RtInfomation::GetInstance()->FrameStart();

	InputEngine::GetInstance()->Update();
	AudioEngine::GetInstance()->Update();
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
