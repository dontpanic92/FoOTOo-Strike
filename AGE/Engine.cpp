#include "Engine.h"
using namespace AGE;

Engine::Engine(){
	mScene = new Scene();
}

int Engine::StartUp(){
	mRenderer = new OpenGLRenderer();
	mApp.StartUp();

	mScene->GetRoot()->Attach(mScene->LoadMesh());

	mRenderer->StartUp(mApp.GetMainWindow());
	return 0;
}

int Engine::Run(){
	return mApp.MainLoop();
}

Engine::~Engine(){
	delete mScene;
}

int Engine::Update(){
	//static Mesh mesh;

	//mRenderer->TestRender();
	//mRenderer->RenderMesh(&mesh);
	mRenderer->RenderScene(mScene);
	return 1;
}