#pragma once
#include <service/boss_zay.hpp>

sint32 Add(float& x, float& y);
sint32 Act(sint32& type, float& vx, float& vy);
void Render(ZayPanel& panel, sint32 type);
sint32 RenderUser(ZayPanel& panel, sint32 id, float vx, float vy);

class Fly
{
public:
    Fly()
	{
		mType = 0;
		mVecCount = 0;
		mLinking = false;
		mPrev = nullptr;
		mNext = nullptr;
	}
    ~Fly() {}

public:
	sint32 mType;
    Point mPos;
	Point mVec;
    sint32 mVecCount;
	bool mLinking;
	Fly* mPrev;
	Fly* mNext;
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
	Fly mTail;
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

class bulletsData : public ZayObject, public SyncuClient
{
public:
    bulletsData();
    ~bulletsData();

public:
	sint32 OnAdd(float& x, float& y);
	sint32 OnAct(sint32& type, float& vx, float& vy);
	void OnRender(ZayPanel& panel, sint32 type);
	sint32 OnRenderUser(ZayPanel& panel, sint32 id, float vx, float vy);

public:
	void OnVisit(chars id) override;
	void OnLeave(chars id) override;
	void OnEvent(chars id, const Context& json) override;

public:
	Map<Airplane> mAirplanes;
	Map<Fly> mFlies;
	sint32 mLastFlyID;
	sint32 mAddWait;
};
