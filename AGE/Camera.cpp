#include "Camera.h"
#include <cmath>
#include <stack>
#include "Scene.h"
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