#pragma once
#include "bullets_example.hpp"

class BulletFly
{
public:
    BulletFly()
	{
		mType = 0;
		mVecCount = 0;
		mLinking = false;
		mPrev = nullptr;
		mNext = nullptr;
	}
    ~BulletFly() {}

public:
	sint32 mType;
    Point mPos;
	Point mVec;
    sint32 mVecCount;
	bool mLinking;
	BulletFly* mPrev;
	BulletFly* mNext;
};

class Airplane
{
    BOSS_DECLARE_NONCOPYABLE_CLASS(Airplane)
public:
    Airplane()
    {
		mSizeR = 0;
		mDegree = -1;
		mDegreeCancelOnce = false;
    }
    ~Airplane() {}
    Airplane(Airplane&& rhs) {operator=(ToReference(rhs));}
    Airplane& operator=(Airplane&& rhs)
    {
		mSizeR = rhs.mSizeR;
        mPos = rhs.mPos;
		mVec = rhs.mVec;
		mDegree = rhs.mDegree;
		mDegreeCancelOnce = rhs.mDegreeCancelOnce;
		mKeyPressMap = rhs.mKeyPressMap;
		mTail.mNext = rhs.mTail.mNext;
		if(mTail.mNext) mTail.mNext->mPrev = &mTail;
		rhs.mTail.mNext = nullptr;
        return *this;
    }

public:
	void AnimationOnce(sint32 width, sint32 height)
	{
		const sint32 Speed = 7;
		const float XAdd = (mDegree == -1)? 0 : Speed * Math::Cos(Math::ToRadian(mDegree + 225));
		const float YAdd = (mDegree == -1)? 0 : Speed * Math::Sin(Math::ToRadian(mDegree + 225));

		mVec.x = mVec.x * 0.95f + XAdd * 0.05f;
		mVec.y = mVec.y * 0.95f + YAdd * 0.05f;
		mPos += mVec;
		while(mPos.x > width / 2) mPos.x -= width;
		while(mPos.y > height / 2) mPos.y -= height;
		while(mPos.x < -width / 2) mPos.x += width;
		while(mPos.y < -height / 2) mPos.y += height;

		if(mDegreeCancelOnce)
		{
			mDegree = -1;
			mDegreeCancelOnce = false;
		}
	}

public:
	sint32 mSizeR;
    Point mPos;
	Point mVec;
	sint32 mDegree;
	bool mDegreeCancelOnce;
	Map<sint32> mKeyPressMap;
	BulletFly mTail;
};

class bulletsData : public ZayObject, public SyncuClient
{
public:
    bulletsData();
    ~bulletsData();

public:
	void OnVisit(chars id) override;
	void OnLeave(chars id) override;
	void OnEvent(chars id, const Context& json) override;

public:
	Map<Airplane> mAirplanes;
	Map<BulletFly> mFlies;
	sint32 mLastFlyID;
	sint32 mAddWait;
};
