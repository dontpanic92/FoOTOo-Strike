#ifndef __AGE_RTINFOMATION_HEADER__
#define __AGE_RTINFOMATION_HEADER__

#include "Singleton.h"
#include "Timer.h"

namespace AGE{

	class RtInfomation : public Singleton<RtInfomation>{
	public:
		RtInfomation();
		~RtInfomation() {}
		void FrameStart()
		{
			float now = Timer::GetInstance()->GetTotalMilliSeconds();
			if (mRenderStartTime == 0)
				mFps = 0;
			else
				mFps = 1000 / (now - mRenderStartTime);
			mRenderStartTime = now;
			mTmpNumberOfTriangles = 0;
			mTmpNumberOfObject = 0;
		}

		void FrameLogicEnd()
		{
			mCpuLogicTime = Timer::GetInstance()->GetTotalMilliSeconds() - mRenderStartTime;
		}

		void FrameCullingEnd()
		{
			mCpuCullingTime = Timer::GetInstance()->GetTotalMilliSeconds() - mRenderStartTime - -mCpuLogicTime;
		}

		void FrameEnd()
		{
			mCpuRenderingTime = Timer::GetInstance()->GetTotalMilliSeconds() - mRenderStartTime - mCpuLogicTime;
			mNumberOfTriangles = mTmpNumberOfTriangles;
			mNumberOfObjects = mTmpNumberOfObject;

			//printf("%f %f %f %f %lu %lu   \n", mCpuLogicTime, mCpuCullingTime, mCpuRenderingTime, mFps, mNumberOfTriangles, mNumberOfObjects);
		}

		void MoreTriangles(unsigned long n)
		{
			mTmpNumberOfTriangles += n;
		}

		void MoreObjects(unsigned long n)
		{
			mTmpNumberOfObject += n;
		}



	private:

		//Temp vars

		float mRenderStartTime;
		unsigned long mTmpNumberOfTriangles;
		unsigned long mTmpNumberOfObject;

		//Values

		float mCpuLogicTime;
		float mCpuCullingTime;
		float mCpuRenderingTime;
		float mFps;
		unsigned long mNumberOfTriangles;
		unsigned long mNumberOfObjects;

	public:
		float GetCpuLogicTime() const { return mCpuLogicTime; }
		float GetCpuRenderingTime() const  { return mCpuRenderingTime; }
		float GetFps() const { return mFps; }
		unsigned long GetNumberOfTriangles() const { return mNumberOfTriangles; }
		unsigned long GetNumberOfObjects() const { return mNumberOfObjects; }
	};


	inline RtInfomation::RtInfomation()
		:mCpuLogicTime(0), mCpuRenderingTime(0), mFps(0)
	{
	}

	static inline RtInfomation* GetRtInfomation() { return RtInfomation::GetInstance(); }
}

#endif
