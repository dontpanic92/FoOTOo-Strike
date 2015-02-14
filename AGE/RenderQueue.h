#ifndef __AGE_RENDERQUEUE_HEADER__
#define __AGE_RENDERQUEUE_HEADER__

#include <map>
#include <vector>
#include "Renderable.h"
#include "Singleton.h"

namespace AGE 
{
	class RenderQueue : public Singleton<RenderQueue>
	{
	public:

		typedef std::vector<const RenderObject*> RenderObjectVector;
		typedef std::map<const Material*, RenderObjectVector> RenderQueueMap;

		void PushRenderObject(const RenderObject* object);
		void PushRenderable(const Renderable* renderable);
		void ClearQueue() { mRenderQueue.clear(); }

		const RenderQueueMap& GetQueue() const { return mRenderQueue; }

	private:
		RenderQueueMap mRenderQueue;
	};
}

#endif