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
	float xmin, xmax, ymin, ymax;

	ymax = nearClip * float(tan(fov * PI / 360.0));
	ymin = -ymax;
	xmin = ymin * aspectRatio;
	xmax = -xmin;

	mProjectMatrix[0][0] = (2.0f * nearClip) / (xmax - xmin);
	mProjectMatrix[1][1]/*[5]*/ = (2.0f * nearClip) / (ymax - ymin);
	mProjectMatrix[2][0]/*[8]*/ = (xmax + xmin) / (xmax - xmin);
	mProjectMatrix[2][1]/*[9]*/ = (ymax + ymin) / (ymax - ymin);
	mProjectMatrix[2][2]/*[10]*/ = -((farClip + nearClip) / (farClip - nearClip));
	mProjectMatrix[2][3]/*[11]*/ = -1.0f;
	mProjectMatrix[3][2]/*[14]*/ = -((2.0f * farClip * nearClip) / (farClip - nearClip));
	mProjectMatrix[3][3]/*[15]*/ = 0.0f;
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