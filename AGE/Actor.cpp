#include "Actor.h"

using namespace AGE;

void Actor::InitPhysics()
{
	mGhostObject = new btPairCachingGhostObject();

	btScalar characterHeight = 20;
	btScalar characterWidth = 30;
	btConvexShape* capsule = new btCapsuleShape(characterWidth, characterHeight);
	//mGhostObject->setUserIndex(0);
	mGhostObject->setUserPointer(this);
	mGhostObject->setCollisionShape(capsule);
	mGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	btScalar stepHeight = btScalar(2);
	mCharacterController = new btKinematicCharacterController(mGhostObject, capsule, stepHeight);
	mCharacterController->setJumpSpeed(400);
	mCharacterController->setGravity(600);
	mCharacterController->setFallSpeed(800);
	

	GetPhysicsEngine()->GetWorld()->addCollisionObject(mGhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter);
	GetPhysicsEngine()->GetWorld()->addAction(mCharacterController);
	printf("collision obj: %d\n", GetPhysicsWorld()->getNumCollisionObjects());
}

Actor::~Actor()
{
	GetPhysicsWorld()->removeAction(mCharacterController);
	GetPhysicsWorld()->removeCollisionObject(mGhostObject);
	delete mCharacterController;
	delete mGhostObject;
}
