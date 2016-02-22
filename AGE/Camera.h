#ifndef __AGE_CAMERA_HEADER__
#define __AGE_CAMERA_HEADER__

#include "Def.h"
#include "LinearMath.h"
#include "SceneNode.h"

namespace AGE
{
	class AGE_EXPORT Camera : public SceneNode
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

		//Left-top origin
		SceneNode* PickAt(int x, int y);

	private:

		Matrix4x4f mProjectMatrix;
		Matrix4x4f mViewMatrix;
		//Transform mTransform;
	};
}

#endif