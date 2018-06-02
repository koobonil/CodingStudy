#pragma once
#include "cars_example.hpp"

class Car
{
    BOSS_DECLARE_NONCOPYABLE_CLASS(Car)
public:
    Car()
    {
		mAngle360 = 0;
		mFart = -1;
		mDegree = -1;
		mDegreeCancelOnce = false;
		mButton1Once = false;
		mButton2Once = false;
    }
    ~Car() {}
    Car(Car&& rhs) {operator=(ToReference(rhs));}
    Car& operator=(Car&& rhs)
    {
		mPos = rhs.mPos;
		mVec = rhs.mVec;
		mAngle360 = rhs.mAngle360;
		mFart = rhs.mFart;
		mDegree = rhs.mDegree;
		mDegreeCancelOnce = rhs.mDegreeCancelOnce;
		mButton1Once = rhs.mButton1Once;
		mButton2Once = rhs.mButton2Once;
		mKeyPressMap = rhs.mKeyPressMap;
        return *this;
    }

public:
	void AnimationOnce()
	{
		mPos.x += mVec.x;
		mPos.y += mVec.y;
		if(mDegreeCancelOnce)
		{
			mDegree = -1;
			mDegreeCancelOnce = false;
		}
		mButton1Once = false;
		mButton2Once = false;
	}

public:
	Point mPos;
	Point mVec;
	float mAngle360;
	sint32 mFart;
	sint32 mDegree;
	bool mDegreeCancelOnce;
	bool mButton1Once;
	bool mButton2Once;
	Map<sint32> mKeyPressMap;
};

class carsData : public ZayObject, public SyncuClient
{
public:
    carsData();
    ~carsData();

public:
	void OnVisit(chars id) override;
	void OnLeave(chars id) override;
	void OnEvent(chars id, const Context& json) override;

public:
	sint32 mXCount;
    sint32 mYCount;
	float mCarSize;
	float mFriction;
	String mTiles;
	Map<Car> mCars;
};
