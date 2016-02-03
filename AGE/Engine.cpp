#include "Engine.h"
#include "Primitive.h"
#include "AGEMeshImporter.h"
#include "Timer.h"
#include "InputEngine.h"
#include "PhysicsEngine.h"
#include "AudioEngine.h"
#include "RtInformation.h"
#include "LevelManager.h"
using namespace AGE;

Engine::Engine() :mLastTimeUpdate(0), mInitialized(false)
{
}

int Engine::StartUp(HWND hWnd)
{
	mApp.StartUp(hWnd);
	RenderEngine::SetEngineType(RenderEngine::Type::Direct3D11);
	RenderEngine::GetInstance()->StartUp();
	InputEngine::GetInstance()->StartUp();
	PhysicsEngine::GetInstance()->StartUp();
	AudioEngine::GetInstance()->StartUp(); 
	Timer::GetInstance()->StartUp();

	//mGameLogic->StartUp();

	mInitialized = true;
	return 0;
}

void Engine::ShutDown()
{
	//mGameLogic->ShutDown();
	//mScene->ShutDown();

	//Timer::GetInstance()->ShutDown();
	AudioEngine::GetInstance()->ShutDown();
	GetLevelManager()->UnloadLevel();
	PhysicsEngine::GetInstance()->ShutDown();
	//InputEngine::GetInstance()->ShutDown();
	//RenderEngine::GetInstance()->ShutDown();
	mApp.ShutDown();

	mInitialized = false;
}

int Engine::Run()
{
	return mApp.MainLoop();
}

Engine::~Engine()
{
	ShutDown();
}

int Engine::Update()
{
	if (!mInitialized || !GetLevel()) {
		return 1;
	}

	float now = Timer::GetInstance()->GetTotalMilliSeconds();
	float delta = now - mLastTimeUpdate;
	//printf("fps: %f\r", 1000.0 / delta);
	//if (delta < 10)
	//	return 1;
	

	RtInformation::GetInstance()->FrameStart();

	InputEngine::GetInstance()->Update();
	if (GetLevel())
		GetLevel()->Update(delta);

	AudioEngine::GetInstance()->Update();
	PhysicsEngine::GetInstance()->Update(delta);
	RtInformation::GetInstance()->FrameLogicEnd();
	if (GetScene())
		GetScene()->UpdateAndCulling();
	RtInformation::GetInstance()->FrameCullingEnd();
	RenderEngine::GetInstance()->Render();
	RtInformation::GetInstance()->FrameEnd();


	mLastTimeUpdate = now;
	return 1;
}
