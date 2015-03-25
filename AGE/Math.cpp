#include "Math.h"
#include <cmath>
#include <cstdio>
using namespace AGE;


inline void swap(float* a, float* b)
{
	float t = *a;
	*a = *b;
	*b = t;
}


const float Matrix4x4f::IdentityFloat[4][4] = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};
const Matrix4x4f Matrix4x4f::Identity;

const float Matrix3x3f::Identity[3][3] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

const Vector3f Vector3f::Zero(0.0f, 0.0f, 0.0f);

Vector3f Vector3f::operator* (const Matrix3x3f& mul)
{
	Vector3f tmp;

	for (int i = 0; i < 3; i++) {
		tmp.mVector[i] = mul[0][i] * mVector[0] + mul[1][i] * mVector[1] + mul[2][i] * mVector[2];
	}

	return tmp;
}
Vector3f Vector3f::operator*(const Matrix4x4f& mul)
{
	Vector3f tmp;
	float scale = mul[0][3] + mul[1][3] + mul[2][3] + mul[3][3];
	for (int i = 0; i < 3; i++) {
		tmp.mVector[i] = (mul[0][i] * mVector[0] + mul[1][i] * mVector[1] + mul[2][i] * mVector[2] + mul[3][i]);// / scale;
	}

	//if (scale > 1.1)
	//printf("%f\n", scale);
	return tmp;
}

void Vector3f::Normalize()
{
	float n = sqrt(mVector[0] * mVector[0] + mVector[1] * mVector[1] + mVector[2] * mVector[2]);
	if (n == 0)
		return;

	mVector[0] /= n;
	mVector[1] /= n;
	mVector[2] /= n;
}

Vector3f Vector3f::operator *(float mul)
{
	Vector3f tmp(*this);
	tmp.mVector[0] *= mul;
	tmp.mVector[1] *= mul;
	tmp.mVector[2] *= mul;
	return tmp;
}

float Vector3f::GetLength()
{
	return sqrt(mVector[0] * mVector[0] + mVector[1] * mVector[1] + mVector[2] * mVector[2]);
}

Matrix3x3f::operator float*()
{
	return (float*)mMatrix;
}

Matrix3x3f& Matrix3x3f::Transpose()
{
	swap(&mMatrix[0][1], &mMatrix[1][0]);
	swap(&mMatrix[0][2], &mMatrix[2][0]);
	swap(&mMatrix[1][2], &mMatrix[2][1]);
	return *this;
}

bool Matrix3x3f::operator != (const Matrix3x3f& mat)
{
	return !memcmp(mMatrix, mat.mMatrix, sizeof(mMatrix));
}

Matrix3x3f::Matrix3x3f(const Matrix4x4f& copy)
{
	memcpy(mMatrix[0], copy[0], sizeof(float) * 3);
	memcpy(mMatrix[1], copy[1], sizeof(float) * 3);
	memcpy(mMatrix[2], copy[2], sizeof(float) * 3);
}


Matrix4x4f::Matrix4x4f(const Matrix3x3f& matrix, const Vector3f& vector)
{
	memcpy(mMatrix[0], matrix[0], sizeof(float) * 3);
	memcpy(mMatrix[1], matrix[1], sizeof(float) * 3);
	memcpy(mMatrix[2], matrix[2], sizeof(float) * 3);
	memcpy(mMatrix[3], (const float*)vector, sizeof(float) * 3);
	mMatrix[0][3] = mMatrix[1][3] = mMatrix[2][3] = 0.0f;
	mMatrix[3][3] = 1.0f;
}

Matrix4x4f::operator float*()const
{
	return (float*)mMatrix;
}

bool Matrix4x4f::operator != (const Matrix4x4f& mat) const
{
	return !memcmp(mMatrix, mat.mMatrix, sizeof(mMatrix));;
}

Matrix4x4f Matrix4x4f::Transpose() const
{
	Matrix4x4f transposeMatrix(*this);
	swap(&transposeMatrix.mMatrix[0][1], &transposeMatrix.mMatrix[1][0]);
	swap(&transposeMatrix.mMatrix[0][2], &transposeMatrix.mMatrix[2][0]);
	swap(&transposeMatrix.mMatrix[0][3], &transposeMatrix.mMatrix[3][0]);
	swap(&transposeMatrix.mMatrix[1][2], &transposeMatrix.mMatrix[2][1]);
	swap(&transposeMatrix.mMatrix[1][3], &transposeMatrix.mMatrix[3][1]);
	swap(&transposeMatrix.mMatrix[2][3], &transposeMatrix.mMatrix[3][2]);
	return *this;
}

Matrix4x4f Matrix4x4f::Inverse() const
{
	/*
	* http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche53.html
	*
	*/
	Matrix3x3f inverseMatrix(*this);
	swap(&inverseMatrix[0][1], &inverseMatrix[1][0]);
	swap(&inverseMatrix[0][2], &inverseMatrix[2][0]);
	swap(&inverseMatrix[1][2], &inverseMatrix[2][1]);

	Vector3f vec((*this)[3][0], (*this)[3][1], (*this)[3][2]);

	vec = vec * inverseMatrix;
	vec.Set(-vec[0], -vec[1], -vec[2]);

	Matrix4x4f newMatrix(inverseMatrix, vec);

	return newMatrix;
}

Matrix4x4f Matrix4x4f::operator *(const Matrix4x4f& mul) const
{
	Matrix4x4f tmp;

	for (int i = 0; i < 4; i++) {
		tmp.mMatrix[i][0] = mMatrix[i][0] * mul.mMatrix[0][0] + mMatrix[i][1] * mul.mMatrix[1][0] + mMatrix[i][2] * mul.mMatrix[2][0] + mMatrix[i][3] * mul.mMatrix[3][0];
		tmp.mMatrix[i][1] = mMatrix[i][0] * mul.mMatrix[0][1] + mMatrix[i][1] * mul.mMatrix[1][1] + mMatrix[i][2] * mul.mMatrix[2][1] + mMatrix[i][3] * mul.mMatrix[3][1];
		tmp.mMatrix[i][2] = mMatrix[i][0] * mul.mMatrix[0][2] + mMatrix[i][1] * mul.mMatrix[1][2] + mMatrix[i][2] * mul.mMatrix[2][2] + mMatrix[i][3] * mul.mMatrix[3][2];
		tmp.mMatrix[i][3] = mMatrix[i][0] * mul.mMatrix[0][3] + mMatrix[i][1] * mul.mMatrix[1][3] + mMatrix[i][2] * mul.mMatrix[2][3] + mMatrix[i][3] * mul.mMatrix[3][3];
	}

	return tmp;
}

void Transform::Translate(const Vector3f& translation, CoordSystem coordSystem)
{

	Matrix4x4f translateMatrix;
	translateMatrix[3][0] = translation[0];
	translateMatrix[3][1] = translation[1];
	translateMatrix[3][2] = translation[2];

	/*if(coordSystem == Local){
		mTransformMatrix = translateMatrix * mTransformMatrix;
		}else{
		mTransformMatrix = mTransformMatrix * translateMatrix;
		}*/

	Multiply(translateMatrix, coordSystem);
}

void Transform::SetPosition(const Vector3f& position)
{
	mTransformMatrix[3][0] = position[0];
	mTransformMatrix[3][1] = position[1];
	mTransformMatrix[3][2] = position[2];
}

Vector3f Transform::GetPosition()
{
	Vector3f position;
	position[0] = mTransformMatrix[3][0];
	position[1] = mTransformMatrix[3][1];
	position[2] = mTransformMatrix[3][2];
	return position;
}

void Transform::RotateByRadian(float radian, float x, float y, float z, CoordSystem coordSystem)
{
	/*
	*	http://en.wikipedia.org/wiki/Rotation_matrix
	*
	*	c = cos(theta), s = sin(theta), v = (x, y, z), where x^2+y^2+z^2=0
	*	M(v, theta) = [c+(1-c)x^2	(1-c)xy-sz	(1-c)xz+sy]
	*				  [(1-c)xy+sz	c+(1-c)y^2	(1-c)yz-sx]
	*				  [(1-c)zx-sy	(1-c)yz+sx	c+(1-c)z^2]
	*		Note: this matrix is column-major and we need a Transpose.
	*
	*/

	float norm = sqrt(x*x + y*y + z*z);
	if (norm == 0) {
		return;
	}

	x /= norm, y /= norm, z /= norm;

	float c = cos(radian);
	float s = sin(radian);
	float mc = 1 - c;

	Matrix4x4f rotationMatrix;
	rotationMatrix[0][0] = c + mc*x*x;
	rotationMatrix[1][0] = mc*x*y - s*z;
	rotationMatrix[2][0] = mc*x*z + s*y;
	rotationMatrix[0][1] = mc*x*y + s*z;
	rotationMatrix[1][1] = c + mc*y*y;
	rotationMatrix[2][1] = mc*y*z - s*x;
	rotationMatrix[0][2] = mc*x*z - s*y;
	rotationMatrix[1][2] = mc*y*z + s*x;
	rotationMatrix[2][2] = c + mc*z*z;

	/*if(coordSystem == Local)
		mTransformMatrix = rotationMatrix * mTransformMatrix;
		else
		mTransformMatrix = mTransformMatrix * rotationMatrix;
		*/
	Multiply(rotationMatrix, coordSystem);
}

void Transform::ClearRotation()
{
	mTransformMatrix[0][0] = mTransformMatrix[1][1] = mTransformMatrix[2][2] = 1.0f;
	mTransformMatrix[0][1] = mTransformMatrix[0][2] = mTransformMatrix[1][0]
		= mTransformMatrix[1][2] = mTransformMatrix[2][0] = mTransformMatrix[2][1] = 0.0f;
}

void Transform::Multiply(const Matrix4x4f& matrix, CoordSystem coordSystem)
{
	if (coordSystem == Local)
		mTransformMatrix = matrix * mTransformMatrix;
	else
		mTransformMatrix = mTransformMatrix * matrix;
}

Matrix4x4f Transform::GetInverseTransformMatrix()
{
	/*
	 * http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche53.html
	 * NOTE: NO SCALE!
	 */
	Matrix3x3f inverseMatrix(mTransformMatrix);
	swap(&inverseMatrix[0][1], &inverseMatrix[1][0]);
	swap(&inverseMatrix[0][2], &inverseMatrix[2][0]);
	swap(&inverseMatrix[1][2], &inverseMatrix[2][1]);

	Vector3f vec(mTransformMatrix[3][0], mTransformMatrix[3][1], mTransformMatrix[3][2]);

	vec = vec * inverseMatrix;
	vec.Set(-vec[0], -vec[1], -vec[2]);

	return Matrix4x4f(inverseMatrix, vec);
}

Matrix4x4f Quaternion::ToRotationMatrix()
{
	float x2 = mQuaternion[0] * mQuaternion[0];
	float y2 = mQuaternion[1] * mQuaternion[1];
	float z2 = mQuaternion[2] * mQuaternion[2];
	float xy = mQuaternion[0] * mQuaternion[1];
	float xz = mQuaternion[0] * mQuaternion[2];
	float yz = mQuaternion[1] * mQuaternion[2];
	float wx = mQuaternion[3] * mQuaternion[0];
	float wy = mQuaternion[3] * mQuaternion[1];
	float wz = mQuaternion[3] * mQuaternion[2];

	Matrix4x4f matrix;

	matrix[0][0] = 1 - 2 * (y2 - z2);
	matrix[0][1] = 2 * (xy + wz);
	matrix[0][2] = 2 * (xz - wy);
	matrix[1][0] = 2 * (xy - wz);
	matrix[1][1] = 1 - 2 * (x2 - z2);
	matrix[1][2] = 2 * (yz + wz);
	matrix[2][0] = 2 * (xz - wy);
	matrix[2][1] = 2 * (yz + wz);
	matrix[2][2] = 1 - 2 * (x2 - y2);

	return matrix;
}


Matrix4x4f AGE::MakeOrthoProjectionMatrix(float width, float height, float near, float far)
{
	Matrix4x4f proj;
	proj[0][0] = 2 / width;
	proj[1][1] = 2 / height;
	proj[2][2] = 1 / (far - near);
	proj[3][2] = near / (far - near);
	return proj;
}
