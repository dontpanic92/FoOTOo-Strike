#ifndef __AGE_MATH_HEADER__
#define __AGE_MATH_HEADER__

#include <cstring>

namespace AGE
{
	typedef unsigned int uint;
	typedef unsigned long ulong;

	const float PI = 3.14159265f; //3.141592653589793238;
	const float DOUBLE_PI = 2 * PI;
	const float _PI_DIV_180 = 0.0174532925f; //0.017453292519943296;
	const float _180_DIV_PI = 57.2957795f; //57.295779513082320876798154814105;

	inline float Deg2Rad(float Deg){ return Deg * _PI_DIV_180; }
	inline float Rad2Deg(float Rad){ return Rad * _180_DIV_PI; }

	class Matrix4x4f;
	class Matrix3x3f
	{
	public:
		Matrix3x3f(){ MakeIdentity(); }
		Matrix3x3f(const Matrix4x4f& copy);

		Matrix3x3f& Transpose();

		float* operator[](int index) { return mMatrix[index]; }
		const float* operator[](int index) const { return mMatrix[index]; }

		bool operator != (const Matrix3x3f& mat);
		
		operator float*();

		void MakeIdentity() { memcpy(mMatrix, Identity, sizeof(Identity)); }

		static const float Identity[3][3];
	private:
		float mMatrix[3][3];
	};

	class Vector3f
	{
	public:
		Vector3f(){Set(Zero[0], Zero[1], Zero[2]);}
		Vector3f(float _0, float _1, float _2){ Set(_0, _1, _2); }

		float& operator[](int index){ return mVector[index]; }
		float operator[](int index) const{ return mVector[index]; }
		Vector3f operator*(const Matrix3x3f& mul);
		operator float*() { return mVector; }
		operator const float*() const {return mVector;}

		void Set(float _0, float _1, float _2){ mVector[0] = _0; mVector[1] = _1; mVector[2] = _2; }

		static const Vector3f Zero;
	private:
		float mVector[3];
	};

	class Matrix4x4f
	{
	public:
		Matrix4x4f() { MakeIdentity(); }
		Matrix4x4f(const Matrix3x3f& matrix, const Vector3f& vector);

		float* operator[](int index) { return mMatrix[index]; }
		const float* operator[](int index) const { return mMatrix[index]; }

		operator float*();
		//operator const float*() const{ return (const float*)mMatrix; }

		Matrix4x4f& Transpose();

		Matrix4x4f operator *(const Matrix4x4f& mul);

		bool operator != (const Matrix4x4f& mat);

		void MakeIdentity() { memcpy(mMatrix, Identity, sizeof(Identity)); }

		static const float Identity[4][4];

	private:
		float mMatrix[4][4];
	};

	class Transform
	{
	public:
		enum CoordSystem { World, Local};

		void Translate(const Vector3f& translation, CoordSystem coordSystem = Local);
		void RotateByRadian(float radian, float x, float y, float z, CoordSystem coordSystem = Local);

		bool operator != (const Transform& tran) { return mTransformMatrix != tran.mTransformMatrix; }

		Matrix4x4f GetTransformMatrix(){ return mTransformMatrix; }

		Matrix4x4f GetInverseTransformMatrix();
	private:
		Matrix4x4f mTransformMatrix;
	};

}

#endif