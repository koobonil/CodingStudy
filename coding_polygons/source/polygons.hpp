#pragma once
#include <service/boss_zay.hpp>

sint32 Add(float& x, float& y);
sint32 Act(sint32& type, float& vx, float& vy);
sint32 Score(sint32 type);
void Render(ZayPanel& panel, sint32 type, sint32 ani);

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

class Fly
{
    BOSS_DECLARE_NONCOPYABLE_CLASS(Fly)
public:
    Fly()
    {
        mType = 0;
        mVecCount = 0;
        mDeathAnimate = 0;
    }
    ~Fly() {}
    Fly(Fly&& rhs) {operator=(ToReference(rhs));}
    Fly& operator=(Fly&& rhs)
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

class SyncuClient
{
protected:
	SyncuClient(chars name);
	~SyncuClient();

public:
	void Resize(sint32 width, sint32 height);
	void CheckMessage();
	virtual void OnVisit(chars id) = 0;
	virtual void OnLeave(chars id) = 0;
	virtual void OnEvent(chars id, const Context& json) = 0;

private:
	void OnMessage(Context& json);

public:
	id_socket mSocket;
	String mSocketMessage;
	String mSHCode;
	String mSHPassword;
	sint32 mSHCount;
	sint32 mScreenWidth;
	sint32 mScreenHeight;
	sint32 mLastIDX;
	sint32 mSumIDX;
};

class polygonsData : public ZayObject, public SyncuClient
{
public:
    polygonsData();
    ~polygonsData();

public:
    sint32 OnAdd(float& x, float& y);
    sint32 OnAct(sint32& type, float& vx, float& vy);
    sint32 OnScore(sint32 type);
    void OnRender(ZayPanel& panel, sint32 type, sint32 ani);

public:
	void OnVisit(chars id) override;
	void OnLeave(chars id) override;
	void OnEvent(chars id, const Context& json) override;

public:
    sint32 mScore;
    sint32 mAddWait;
    Map<Area> mAreas;
    Array<Fly> mFlies;
};
