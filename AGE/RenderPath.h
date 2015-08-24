#ifndef __AGE_RENDERPATH_HEADER__
#define __AGE_RENDERPATH_HEADER__

namespace AGE
{
	class RenderPath
	{
	public:
		virtual int ExecuteRendering() = 0;
		virtual ~RenderPath() {}
	};
}

#endif
