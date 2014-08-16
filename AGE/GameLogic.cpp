#include "GameLogic.h"
#include "Engine.h"
using namespace AGE;

bool GameLogicImp::Update(float time){
	Renderable* renderable = dynamic_cast<Renderable*>(Engine::GetInstance()->GetScene()->GetAttachable());
	Transform* translate = renderable->GetTramsform();

	float yRot = time * 60.0f;
	translate->RotateByRadian(Deg2Rad(yRot), 0, 1, 0, Transform::World);
	translate->RotateByRadian(-Deg2Rad(yRot), 0, 1, 0);

	return true;
}
