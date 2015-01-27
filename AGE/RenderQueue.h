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
		/*
		class const_iterator
		{
			friend class RenderQueue;
		public:
			const_iterator() : mRenderQueue(nullptr){}
			const_iterator(const RenderQueue* renderQueue) : mRenderQueue(renderQueue) {}
			const_iterator(const_iterator&& it) 
			{ 
				*this = it;
				it.mRenderQueue = nullptr;
			}

			const_iterator& operator = (const_iterator& it) 
			{
				mRenderQueue = it.mRenderQueue;
				mMapIt = it.mMapIt;
				mVectorIt = it.mVectorIt;
			}

			//It is not a standard behavior of iterators, but it can simple our code.
			const RenderObject* operator ->() const
			{
				return *mVectorIt;
			}

			//See above
			const RenderObject& operator *()
			{
				return **mVectorIt;
			}

			const_iterator& operator ++()
			{
				return const_cast<const_iterator&>(static_cast<const const_iterator *>(this)->operator++());
			}

			const const_iterator& operator ++() const
			{
				if (mMapIt == mRenderQueue->mRenderQueue.end())
					return;
				if (mVectorIt != mMapIt->second.end()) {
					mVectorIt++;
				} else {
					mMapIt++;
					mVectorIt;
				}
				return *this;
			}

			const_iterator operator ++(int)
			{
				return static_cast<const const_iterator *>(this)->operator++(0);
			}

			const_iterator operator ++(int) const
			{
				const_iterator tmp = *this;
				this->operator++();
				return tmp;
			}

		private:

			const RenderQueue* mRenderQueue;
			mutable RenderQueueMap::const_iterator mMapIt;
			mutable RenderObjectVector::const_iterator mVectorIt;
		};
		*/

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