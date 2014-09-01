#include "GameLogic.h"
#include "Engine.h"
using namespace AGE;
using namespace OIS;

void GameLogicImp::StartUp(){
	InputEngine::GetInstance()->RegisterMouseListener(this);
	InputEngine::GetInstance()->RegisterKeyListener(this);
}

bool GameLogicImp::Update(float time){
	//Renderable* renderable = dynamic_cast<Renderable*>(Engine::GetInstance()->GetScene()->GetAttachable());
	//Transform* translate = renderable->GetTramsform();

	//float yRot = time * 60.0f / 1000.0f;
	//translate->Translate(Vector3f(0, 0, -time / 1000), Transform::Local);
	//translate->RotateByRadian(Deg2Rad(yRot), 0, 1, 0);

	char keys[256];
	InputEngine::GetInstance()->GetKeyStates(keys);

	Transform* cameraTransform = Engine::GetInstance()->GetScene()->GetCurrentCamera()->GetTransform();
	float speed = time / 4;// / 100;

	if(keys[KC_W]){
		cameraTransform->Translate(Vector3f(0, 0, -speed));
	}

	if(keys[KC_S]){
		cameraTransform->Translate(Vector3f(0, 0, speed));
	}

	if(keys[KC_A]){
		cameraTransform->Translate(Vector3f(-speed, 0, 0));
	}

	if(keys[KC_D]){
		cameraTransform->Translate(Vector3f(speed, 0, 0));
	}

	if(keys[KC_LCONTROL] || keys[KC_RCONTROL]){
		cameraTransform->Translate(Vector3f(0, -speed, 0), Transform::World);
	}

	if(keys[KC_SPACE]){
		cameraTransform->Translate(Vector3f(0, speed, 0), Transform::World);
	}

	return true;
}

bool GameLogicImp::mouseMoved( const MouseEvent &arg ){
	//printf("abs: %d, rel: %d\n", arg.state.X.abs, arg.state.X.rel);
	Transform* cameraTransform = Engine::GetInstance()->GetScene()->GetCurrentCamera()->GetTransform();
	static float pitchDegree = 0.0f;
	static float yawDegree = 0.0f;

	float speed = - arg.state.X.rel / 5.0f;
	float yspeed = - arg.state.Y.rel / 5.0f;

	pitchDegree += yspeed;
	yawDegree += speed;
	if(pitchDegree >= 90)
		pitchDegree = 89.9999f;
	if(pitchDegree <= -90)
		pitchDegree = -89.9999f;

	while(yawDegree > 360)
		yawDegree -= 360;

	while(yawDegree < -360)
		yawDegree += 360;

	cameraTransform->ClearRotation();
	cameraTransform->RotateByRadian(Deg2Rad(yawDegree), 0, 1, 0);
	cameraTransform->RotateByRadian(Deg2Rad(pitchDegree), 1, 0, 0);

	return true;
}
