#ifndef __AGE_MATH_HEADER__
#define __AGE_MATH_HEADER__

namespace AGE
{
	typedef unsigned int uint;
	typedef unsigned long ulong;

	class Vector3f
	{
	public:
		float& operator[](int index){ return mVector[index]; }
		void Set(float _0, float _1, float _2){ mVector[0] = _0; mVector[1] = _1; mVector[2] = _2; }

	private:
		float mVector[3];
	};

	class Matrix4x4f
	{
	public:
		float* operator[](int index){ return mMatrix[index]; }

	private:
		float mMatrix[4][4];
	};

	class Transform
	{
	private:
		Matrix4x4f mTransformMatrix;
	};
}

#endif