#include "Engine.h"
#include "Primitive.h"
#include "AGEMeshImporter.h"
#include "Timer.h"
#include "InputEngine.h"
#include "PhysicsEngine.h"
#include "AudioEngine.h"
#include "RtInfomation.h"
#include "LevelManager.h"
using namespace AGE;

Engine::Engine() :mLastTimeUpdate(0)
{
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

	//mGameLogic->StartUp();

	return 0;
}

void Engine::ShutDown()
{
	//mGameLogic->ShutDown();
	//mScene->ShutDown();

	//Timer::GetInstance()->ShutDown();
	AudioEngine::GetInstance()->ShutDown();
	GetLevelManager()->UnloadLevel();
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
	ShutDown();
}

int Engine::Update()
{

	float now = Timer::GetInstance()->GetTotalMilliSeconds();
	float delta = now - mLastTimeUpdate;
	//printf("fps: %f\r", 1000.0 / delta);
	if (delta < 10)
		return 1;

	RtInfomation::GetInstance()->FrameStart();

	InputEngine::GetInstance()->Update();
	AudioEngine::GetInstance()->Update();
	PhysicsEngine::GetInstance()->Update(delta);
	if (GetLevel())
		GetLevel()->Update(delta);
	RtInfomation::GetInstance()->FrameLogicEnd();
	if (GetScene())
		GetScene()->UpdateAndCulling();
	RtInfomation::GetInstance()->FrameCullingEnd();
	RenderEngine::GetInstance()->Render();
	RtInfomation::GetInstance()->FrameEnd();


	mLastTimeUpdate = now;
	return 1;
}
