#ifndef __AGE_ATTACHABLE_HEADER__
#define __AGE_ATTACHABLE_HEADER__

#include "Math.h"

namespace AGE
{
	class SceneNode;
	class Attachable
	{
	public:
		Attachable() :mParent(0) {}

		virtual ~Attachable(){};
		Transform* GetTramsform(){ return &mTransform; }

		void SetParent(SceneNode* parent) { mParent = parent; }
		SceneNode* GetParent() { return mParent; }

	protected:
		Transform mTransform;
		SceneNode* mParent;

	};

}

#endif