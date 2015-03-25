#ifndef __AGE_RENDERQUEUE_HEADER__
#define __AGE_RENDERQUEUE_HEADER__

#include <map>
#include "Renderable.h"
#include "Singleton.h"

namespace AGE 
{
	class RenderListMemoryPool;
	class RenderList
	{
	public:

		struct ListNode
		{
			const RenderObject* Object;
			ListNode* Next;
		};

		typedef const RenderObject* value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;

		class const_iterator
		{
		public:
			const_iterator() : mRenderList(0), mCurrent(0) {}
			const_iterator(const RenderList* renderList, RenderList::ListNode* current)
				: mRenderList(renderList), mCurrent(current)
			{
			}

			const_reference operator*() const
			{
				return mCurrent->Object;
			}

			const_iterator& operator++()
			{
				mCurrent = mCurrent->Next;
				return *this;
			}

			const_iterator operator++(int)
			{
				const_iterator i = *this;
				mCurrent = mCurrent->Next;
				return i;
			}

			bool operator !=(const const_iterator& a)
			{
				return (mCurrent != a.mCurrent || mRenderList != mRenderList);
			}

		private:
			const RenderList* mRenderList;
			RenderList::ListNode* mCurrent;
		};

		RenderList(RenderListMemoryPool* pool);

		void Push(const RenderObject* object);

		const_iterator begin() const { return const_iterator(this, mHead); }

		const_iterator end() const { return const_iterator(this, 0); }

	private:
		RenderListMemoryPool* mMemoryPool;
		ListNode* mHead;
		ListNode* mTail;
	};

	class RenderListMemoryPool
	{
	public:
		RenderListMemoryPool();
		~RenderListMemoryPool();

		void Clear();

		RenderList::ListNode* AllocNode();

	private:
		void ReAllocMemory();

		ulong mBufferSize;
		RenderList::ListNode* mBuffer;
		ulong mCurrent;

	};

	class RenderQueue : public Singleton<RenderQueue>
	{
	public:

		//typedef std::vector<const RenderObject*> RenderObjectVector;
		typedef RenderList RenderObjectVector;
		typedef std::map<const Material*, RenderObjectVector> RenderQueueMap;

		void PushRenderObject(const RenderObject* object);
		void PushRenderable(const Renderable* renderable);
		void ClearQueue() { mRenderQueue.clear(); mMemoryPool.Clear(); }

		const RenderQueueMap& GetQueue() const { return mRenderQueue; }

	private:
		RenderQueueMap mRenderQueue;
		RenderListMemoryPool mMemoryPool;
	};
}

#endif