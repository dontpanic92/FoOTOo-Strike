#ifndef __AGE_MATH_HEADER__
#define __AGE_MATH_HEADER__

#include <cstring>
#include <vector>
#include <functional>
#include "Def.h"

namespace AGE
{
	const float PI = 3.14159265f; //3.141592653589793238;
	const float DOUBLE_PI = 2 * PI;
	const float _PI_DIV_180 = 0.0174532925f; //0.017453292519943296;
	const float _180_DIV_PI = 57.2957795f; //57.295779513082320876798154814105;

	inline float Deg2Rad(float Deg) { return Deg * _PI_DIV_180; }
	inline float Rad2Deg(float Rad) { return Rad * _180_DIV_PI; }

	class Matrix4x4f;
	class AGE_EXPORT Matrix3x3f
	{
	public:
		Matrix3x3f() { SetAsIdentity(); }
		Matrix3x3f(const Matrix4x4f& copy);
		Matrix3x3f(float* m);

		Matrix3x3f& Transpose();

		float* operator[](int index) { return mMatrix[index]; }
		const float* operator[](int index) const { return mMatrix[index]; }

		bool operator != (const Matrix3x3f& mat);

		operator float*();

		void SetAsIdentity() { memcpy(mMatrix, IdentityFloat, sizeof(IdentityFloat)); }
		void SetAsZero() { memcpy(mMatrix, ZeroFloat, sizeof(ZeroFloat)); }

		static Matrix3x3f  MakeIdentity() { return (float*)IdentityFloat; }
		static Matrix3x3f  MakeZero() { return (float*)ZeroFloat; }

	private:
		float mMatrix[3][3];

		static const float IdentityFloat[3][3];
		static const float ZeroFloat[3][3];
	};

	class Matrix4x4f;
	class AGE_EXPORT Vector3f
	{
	public:
		Vector3f() { Set(Zero[0], Zero[1], Zero[2]); }
		Vector3f(float _0, float _1, float _2) { Set(_0, _1, _2); }
		Vector3f(const float v[3]) { memcpy(mVector, v, sizeof(mVector)); }

		float& operator[](int index) { return mVector[index]; }
		float operator[](int index) const { return mVector[index]; }
		Vector3f operator +(const Vector3f& v);

		//As Vector
		Vector3f operator *(const Matrix3x3f& mul);

		//As Point.
		Vector3f operator *(const Matrix4x4f& mul);

		Vector3f operator *(float mul);
		operator float*() { return mVector; }
		operator const float*() const { return mVector; }

		void Set(float _0, float _1, float _2) { mVector[0] = _0; mVector[1] = _1; mVector[2] = _2; }
		void Normalize();
		float GetLength();

		static const Vector3f Zero;
	private:
		float mVector[3];
	};

	class AGE_EXPORT Matrix4x4f
	{
	public:
		Matrix4x4f() { SetAsIdentity(); }
		Matrix4x4f(const float const* m) { memcpy(mMatrix, m, sizeof(mMatrix)); }
		Matrix4x4f(const Matrix3x3f& matrix, const Vector3f& vector);

		float* operator[](int index) { return mMatrix[index]; }
		const float* operator[](int index) const { return mMatrix[index]; }

		operator float*() const;
		//operator const float*() const{ return (const float*)mMatrix; }

		Matrix4x4f Transpose() const;
		Matrix4x4f Inverse() const;

		Matrix4x4f operator *(const Matrix4x4f& mul) const;

		Matrix4x4f operator = (float a[16]) { memcpy(mMatrix, a, sizeof(mMatrix)); return *this; }

		bool operator != (const Matrix4x4f& mat) const;

		void SetAsIdentity() { memcpy(mMatrix, IdentityFloat, sizeof(IdentityFloat)); }

		static Matrix4x4f MakeIdentity() { return (float*)IdentityFloat; }
		static Matrix4x4f MakeZero() { return (float*)ZeroFloat; }

	private:
		float mMatrix[4][4];

		static const float IdentityFloat[4][4];
		static const float ZeroFloat[4][4];
	};

	class AGE_EXPORT Transform
	{
	public:
		enum CoordSpace { World, Relative };

		typedef void ValueChangedCallback(Transform* newValue);

		Transform() {}
		Transform(const Matrix4x4f& matrix) :mTransformMatrix(matrix) {}

		void AddValueChangedCallback(std::function<ValueChangedCallback> f);

		void Translate(const Vector3f& translation, CoordSpace coordSpace = Relative);
		void SetPosition(const Vector3f& position);
		Vector3f GetPosition() const;
		void RotateByRadian(float radian, float x, float y, float z, CoordSpace coordSpace = Relative);
		void ClearRotation();

		void Multiply(const Matrix4x4f& matrix, CoordSpace coordSpace = Relative);

		bool operator != (const Transform& tran) const { return mTransformMatrix != tran.mTransformMatrix; }

		Matrix4x4f GetTransformMatrix() const { return mTransformMatrix; }
		void SetTransformMatrix(const Matrix4x4f& matrix) { mTransformMatrix = matrix; TriggerCallbacks(); }
		void SetTransformMatrixNoCallback(const Matrix4x4f& matrix) { mTransformMatrix = matrix; }

		Matrix4x4f GetInverseTransformMatrix() const;
	private:
		void TriggerCallbacks();

		Matrix4x4f mTransformMatrix;

		std::vector<std::function<ValueChangedCallback> > mCallbacks;
	};


	class AGE_EXPORT Quaternion
	{
	public:
		Quaternion() {}

		Quaternion(float x, float y, float z, float w)
		{
			mQuaternion[0] = x;
			mQuaternion[1] = y;
			mQuaternion[2] = z;
			mQuaternion[3] = w;
		}

		Quaternion(float q[4]) { memcpy(mQuaternion, q, sizeof(mQuaternion)); }

		float& operator[] (int index) { return mQuaternion[index]; }

		Matrix4x4f ToRotationMatrix();

	private:
		float mQuaternion[4];
	};


	Matrix4x4f MakeOrthoProjectionMatrix(float left, float right, float bottom, float top, float near, float far);

	typedef Matrix4x4f Matrix4;
	typedef Matrix3x3f Matrix3;
	typedef Vector3f Vector3;
}

#endif