#include "Actor.h"

using namespace AGE;

void BasicPlayerController::PostUpdate(dFloat timestep, int threadIndex)
{
	CustomPlayerController::PostUpdate(timestep, threadIndex);
	float veloc[3];
	NewtonBodyGetVelocity(m_body, veloc);
	Actor* player = (Actor*)NewtonBodyGetUserData(m_body);
	Vector3f v = player->GetVelocity();

	veloc[0] = v[0];
	veloc[2] = v[2];

	NewtonBodySetVelocity(m_body, &veloc[0]);
}

BasicPlayerControllerManager* Actor::mPlayerManager = nullptr;

void BasicPlayerControllerManager::ApplyPlayerMove(CustomPlayerController* const controller, dFloat timestep)
{
	NewtonBody* const body = controller->GetBody();
	Actor* player = (Actor*)NewtonBodyGetUserData(body);

	float gravity[] = { 0.0f, -10.0, 0.0f, 0.0f };
	Vector3f v = player->GetVelocity();

	controller->SetPlayerVelocity(v[0], v[2], v[1], player->GetHeadingAngle(), gravity, timestep);
}

BasicPlayerController* BasicPlayerControllerManager::CreatePlayer(dFloat mass, dFloat outerRadius, dFloat innerRadius, dFloat height, dFloat stairStep, const dMatrix& localAxis)
{
	BasicPlayerController* const controller = CreateController();
	controller->Init(mass, outerRadius, innerRadius, height, stairStep, localAxis);
	return controller;
}

void Actor::InitPhysics()
{
	if (mPlayerManager == nullptr) {
		auto p = GetPhysicsWorld();
		mPlayerManager = new BasicPlayerControllerManager(p);
	}

	Matrix4x4f playerMatrix = Matrix4x4f::MakeZero();
	playerMatrix[0][1] = 1.0f;
	playerMatrix[1][0] = 1.0f;
	playerMatrix[2][2] = 1.0f;
	playerMatrix[3][3] = 1.0f;

	dMatrix playerAxis;
	playerAxis[0] = dVector(0.0f, 1.0f, 0.0f, 0.0f); // the y axis is the character up vector
	playerAxis[1] = dVector(1.0f, 0.0f, 0.0f, 0.0f); // the x axis is the character front direction
	playerAxis[2] = playerAxis[0] * playerAxis[1];
	playerAxis[3] = dVector(0.0f, 0.0f, 0.0f, 1.0f);

	mController = mPlayerManager->CreatePlayer(200, mRadius, mRadius, mHeight, mHeight * 0.333, playerAxis);
	mController->SetRestrainingDistance(0.1f);
	NewtonBodySetUserData(mController->GetBody(), this);
}

void Actor::SetPosition(Vector3f position)
{
	float m[16];
	NewtonBodyGetMatrix(mController->GetBody(), m);
	m[12] = position[0];
	m[13] = position[1];
	m[14] = position[2];
	NewtonBodySetMatrix(mController->GetBody(), m);
}

Actor::~Actor()
{
}
