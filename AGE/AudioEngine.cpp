#include "AudioEngine.h"

using namespace AGE;

void AudioEngine::StartUp()
{
	YSE::System().init();
}

void AudioEngine::ShutDown()
{
	YSE::System().close();
}

Sound3D::Sound3D() : mAttachable(0) {}

void Sound3D::SetParent(SceneNode* parent)
{
	mAttachable = parent;
}

void Sound3D::Load(const char* filename, bool loop)
{
	mSound.create(filename, NULL, loop);
}

//Sound3D::Sound3D(const char* filename, bool loop, Attachable* parent) 
//	: mAttachable(parent)
//{
//	mSound.create(filename, NULL, loop);
//}

void Sound3D::Update()
{
	YSE::Vec vec;
	Vector3f v = mAttachable->GetWorldTransform().GetPosition();
	vec.set(v[0], v[1], v[2]);
	mSound.setPosition(vec);
}

void AudioEngine::SetListenerPosition(const Vector3f &position)
{
	YSE::Vec vec;
	vec.set(position[0], position[1], position[2]);
	YSE::Listener().setPosition(vec);
}

void AudioEngine::Update()
{
	YSE::System().update();
}

//Sound3D* AudioEngine::CreateSound3D(Attachable* att)
//{
//	return new Sound3D(att);
//}
