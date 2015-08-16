#ifndef __AGE_CAMERA_HEADER__
#define __AGE_CAMERA_HEADER__

#include "LinearMath.h"
#include "SceneObject.h"

namespace AGE
{
	class Camera : public SceneObject
	{
	public:
		Camera() {}
		Camera(float fov, float aspectRatio, float nearClip, float farClip)
		{
			SetParameters(fov, aspectRatio, nearClip, farClip);
		}

		void SetParameters(float fov, float aspectRatio, float nearClip, float farClip);

		const Matrix4x4f& GetProjectMatrix()
		{
			return mProjectMatrix;
		}
		Transform* GetTransform() { return &mTransform; }

		const Matrix4x4f& CalcViewMatrix();

	private:

		Matrix4x4f mProjectMatrix;
		Matrix4x4f mViewMatrix;
		//Transform mTransform;
	};
}

#endif