#include "Math.h"
#include <cmath>
#include <cstdio>
using namespace AGE;

const float Matrix4x4f::Identity[4][4] = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};

const Vector3f Vector3f::Zero(0.0f, 0.0f, 0.0f);

Matrix4x4f::operator float*(){
	/*static float outputMatrix[4][4];
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			outputMatrix[j][i] = mMatrix[i][j];
		}
	}

	return (float*)outputMatrix;*/
	return (float*)mMatrix;
}

bool Matrix4x4f::operator != (const Matrix4x4f& mat){
	return !memcmp(mMatrix, mat.mMatrix, sizeof(mMatrix));;
}

Matrix4x4f Matrix4x4f::operator *(const Matrix4x4f& mul){
	Matrix4x4f tmp;
	//float tmp[4][4];
	//memcpy(mMatrix, tmp, sizeof(mMatrix));

	for(int i = 0; i < 4; i++){
		tmp.mMatrix[i][0] = mMatrix[i][0]*mul.mMatrix[0][0]+mMatrix[i][1]*mul.mMatrix[1][0]+mMatrix[i][2]*mul.mMatrix[2][0]+mMatrix[i][3]*mul.mMatrix[3][0];
		tmp.mMatrix[i][1] = mMatrix[i][0]*mul.mMatrix[0][1]+mMatrix[i][1]*mul.mMatrix[1][1]+mMatrix[i][2]*mul.mMatrix[2][1]+mMatrix[i][3]*mul.mMatrix[3][1];
		tmp.mMatrix[i][2] = mMatrix[i][0]*mul.mMatrix[0][2]+mMatrix[i][1]*mul.mMatrix[1][2]+mMatrix[i][2]*mul.mMatrix[2][2]+mMatrix[i][3]*mul.mMatrix[3][2];
		tmp.mMatrix[i][3] = mMatrix[i][0]*mul.mMatrix[0][3]+mMatrix[i][1]*mul.mMatrix[1][3]+mMatrix[i][2]*mul.mMatrix[2][3]+mMatrix[i][3]*mul.mMatrix[3][3];
	}

	return tmp;
}

void Transform::Translate(const Vector3f& translation){
	mTransformMatrix[3][0] += translation[0];
	mTransformMatrix[3][1] += translation[1];
	mTransformMatrix[3][2] += translation[2];
}

void Transform::RotateByRadian(float radian, float x, float y, float z, CoordSystem coordSystem){
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

	float norm = sqrt(x*x+y*y+z*z);
	if(norm == 0){
		return;
	}

	x /= norm, y /= norm, z /= norm;

	float c = cos(radian);
	float s = sin(radian);
	float mc = 1 - c;

	Matrix4x4f rotationMatrix;
	rotationMatrix[0][0] = c + mc*x*x;
	rotationMatrix[1][0] = mc*x*y-s*z;
	rotationMatrix[2][0] = mc*x*z+s*y;
	rotationMatrix[0][1] = mc*x*y+s*z;
	rotationMatrix[1][1] = c + mc*y*y;
	rotationMatrix[2][1] = mc*y*z-s*x;
	rotationMatrix[0][2] = mc*x*z-s*y;
	rotationMatrix[1][2] = mc*y*z+s*x;
	rotationMatrix[2][2] = c + mc*z*z;

	if(coordSystem == Local)
		mTransformMatrix = rotationMatrix * mTransformMatrix;
	else
		mTransformMatrix = mTransformMatrix * rotationMatrix;
}
