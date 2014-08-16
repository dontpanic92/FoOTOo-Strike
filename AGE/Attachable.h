#ifndef __AGE_ATTACHABLE_HEADER__
#define __AGE_ATTACHABLE_HEADER__

#include "Math.h"

namespace AGE
{
	class Attachable
	{
	public:
		virtual ~Attachable(){};
		Transform* GetTramsform(){ return &mTransform; }
	private:
		Transform mTransform;
	};

}

#endif