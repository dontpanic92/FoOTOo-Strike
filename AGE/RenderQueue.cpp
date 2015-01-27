#include "RenderQueue.h"

using namespace std;
using namespace AGE;

void RenderQueue::PushRenderObject(const RenderObject* object)
{
	RenderQueueMap::iterator it;
	if ((it = mRenderQueue.find(object->Material)) != mRenderQueue.end()) {
		it->second.push_back(object);
	} else {
		vector<const RenderObject*> objects = { object };
		mRenderQueue.insert(make_pair(object->Material, move(objects)));
	}
}

void RenderQueue::PushRenderable(const Renderable* renderable)
{
	for each(auto& object in *renderable)
	{
		PushRenderObject(&object);
	}
}