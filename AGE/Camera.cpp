#include <cmath>
#include <stack>
#include "Camera.h"
#include "Scene.h"
#include "Engine.h"
#include "PhysicsEngine.h"
using namespace AGE;

const float PI = 3.1415926;

void Camera::SetParameters(float fov, float aspectRatio, float nearClip, float farClip)
{
	/*
	0 -- 0  1  2  3
	1 -- 4  5  6  7
	2 -- 8  9  10 11
	3 -- 12 13 14 15
	*/

	float t2i = 1 / float(tan(fov * PI / 360.0));
	mProjectMatrix[0][0] = -t2i / aspectRatio ;
	mProjectMatrix[1][1] = t2i;
	mProjectMatrix[2][2] = farClip / (farClip - nearClip);
	mProjectMatrix[2][3] = 1.0f;
	mProjectMatrix[3][2] = -nearClip * farClip / (farClip - nearClip);

}

const Matrix4x4f& Camera::CalcViewMatrix()
{
	mViewMatrix.MakeIdentity();

	std::stack<Matrix4x4f> s;
	s.push(this->mTransform.GetTransformMatrix());
	SceneNode* parent = mParent;
	while (parent != NULL) {
		s.push(parent->GetTransform()->GetTransformMatrix());
		parent = parent->GetParent();
	}

	while (!s.empty()) {
		mViewMatrix = mViewMatrix * s.top();
		s.pop();
	}

	mViewMatrix = mViewMatrix.Inverse();

	return mViewMatrix;
}

SceneNode* Camera::PickAt(int x, int y)
{
	uint winWidth = Engine::GetInstance()->GetMainWindow().Width;
	uint winHeight = Engine::GetInstance()->GetMainWindow().Height;

	float vx = (+2.0f*x / winWidth - 1.0f) / mProjectMatrix[0][0];
	float vy = (-2.0f*y / winHeight + 1.0f) / mProjectMatrix[1][1];

	Vector3 rayDirection = Vector3(vx, vy, 1) * 1000;
	Matrix4 viewInv = mWorldTransform.GetTransformMatrix();

	Vector3 rayDirInWorld = rayDirection * (Matrix3)viewInv;
	Vector3 from = GetWorldTransform().GetPosition();

	//printf("x:%d y:%d vx:%f vy: %f\n", x, y, vx, vy);
	//printf("from: %f %f %f, to: %f %f %f\n", from[0], from[1], from[2], rayDirInWorld[0], rayDirInWorld[1], rayDirInWorld[2]);

	return GetPhysicsEngine()->GetRaycastClosetObject(from, from + rayDirInWorld, nullptr);
}
