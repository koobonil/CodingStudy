#pragma once
#include "polygons_example.hpp"

class Area
{
    BOSS_DECLARE_NONCOPYABLE_CLASS(Area)
public:
    Area()
    {
        mTouchEnable = false;
        mSpotCount = 0;
        mSpotAnimate = 0;
    }
    ~Area() {}
    Area(Area&& rhs) {operator=(ToReference(rhs));}
    Area& operator=(Area&& rhs)
    {
        mTouchEnable = rhs.mTouchEnable;
        mTouchPos = rhs.mTouchPos;
        mSpotPos[0] = rhs.mSpotPos[0];
        mSpotPos[1] = rhs.mSpotPos[1];
        mSpotPos[2] = rhs.mSpotPos[2];
        mSpotCount = rhs.mSpotCount;
        mSpotAnimate = rhs.mSpotAnimate;
        return *this;
    }

public:
    bool mTouchEnable;
    Point mTouchPos;
    Point mSpotPos[3];
    sint32 mSpotCount;
    sint32 mSpotAnimate;
};

class PolygonFly
{
    BOSS_DECLARE_NONCOPYABLE_CLASS(PolygonFly)
public:
    PolygonFly()
    {
        mType = 0;
        mVecCount = 0;
        mDeathAnimate = 0;
    }
    ~PolygonFly() {}
    PolygonFly(PolygonFly&& rhs) {operator=(ToReference(rhs));}
    PolygonFly& operator=(PolygonFly&& rhs)
    {
        mType = rhs.mType;
        mPos = rhs.mPos;
        mVec = rhs.mVec;
        mVecCount = rhs.mVecCount;
        mDeathAnimate = rhs.mDeathAnimate;
        return *this;
    }

public:
    sint32 mType;
    Point mPos;
    Point mVec;
    sint32 mVecCount;
    sint32 mDeathAnimate;
};

class polygonsData : public ZayObject, public SyncuClient
{
public:
    polygonsData();
    ~polygonsData();

public:
	void OnVisit(chars id) override;
	void OnLeave(chars id) override;
	void OnEvent(chars id, const Context& json) override;

public:
    sint32 mScore;
    sint32 mAddWait;
    Map<Area> mAreas;
    Array<PolygonFly> mFlies;
};
