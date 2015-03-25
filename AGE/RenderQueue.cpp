#include "RenderQueue.h"

using namespace std;
using namespace AGE;

RenderList::RenderList(RenderListMemoryPool* pool) : mMemoryPool(pool), mHead(0), mTail(0)
{
}

void RenderList::Push(const RenderObject* object)
{
	if (mHead == 0) {
		mHead = mTail = mMemoryPool->AllocNode();
		mHead->Object = object;
		mHead->Next = 0;
	} else {
		mTail->Next = mMemoryPool->AllocNode();
		mTail->Next->Object = object;
		mTail->Next->Next = 0;
		mTail = mTail->Next;
	}
}

RenderListMemoryPool::RenderListMemoryPool()
{
	mBufferSize = 4096;
	mBuffer = new RenderList::ListNode[mBufferSize];
	mCurrent = 0;
}

RenderListMemoryPool::~RenderListMemoryPool()
{
	delete[] mBuffer;
}

void RenderListMemoryPool::Clear()
{
	mCurrent = 0;
}

RenderList::ListNode* RenderListMemoryPool::AllocNode()
{
	mCurrent++;
	if (mCurrent >= mBufferSize) {
		ReAllocMemory();
	}

	return &mBuffer[mCurrent];
}

void RenderListMemoryPool::ReAllocMemory()
{
	ulong newBufferSize = mBufferSize * 2;
	RenderList::ListNode* newBuffer = new RenderList::ListNode[newBufferSize];
	memcpy(newBuffer, mBuffer, sizeof(RenderList::ListNode) * mBufferSize);

	delete[] mBuffer;
	mBuffer = newBuffer;
	mBufferSize = newBufferSize;
}

void RenderQueue::PushRenderObject(const RenderObject* object)
{
	RenderQueueMap::iterator it;
	if ((it = mRenderQueue.find(object->Material)) != mRenderQueue.end()) {
		it->second.Push(object);// push_back(object);
	} else {
		//vector<const RenderObject*> objects = { object };
		RenderList renderList(&mMemoryPool);
		renderList.Push(object);
		mRenderQueue.insert(move(make_pair(object->Material, renderList)));
	}
}

void RenderQueue::PushRenderable(const Renderable* renderable)
{
	for each(auto& object in *renderable)
	{
		PushRenderObject(object);
	}
}