#ifndef __AGE_CAMERA_HEADER__
#define __AGE_CAMERA_HEADER__

#include "Math.h"
#include "Attachable.h"

namespace AGE
{
	class Camera// : public Attachable
	{
	public:
		Camera(){}
		Camera(float fov, float aspectRatio, float nearClip, float farClip){
			SetParameters(fov, aspectRatio, nearClip, farClip);
		}

		void SetParameters(float fov, float aspectRatio, float nearClip, float farClip);

		Matrix4x4f GetProjectMatrix(){ return mProjectMatrix; }
		Transform* GetTransform(){ return &mTransform; }

	private:

		Matrix4x4f mProjectMatrix;
		Transform mTransform;
	};
}

#endif