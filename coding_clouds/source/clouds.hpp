#pragma once
#include "clouds_example.hpp"

class cloudsData : public ZayObject, public SyncuClient
{
public:
    cloudsData();
    ~cloudsData();

public:
	void Reset();
	void SetReady();
	void OnVisit(chars id) override;
	void OnLeave(chars id) override;
	void OnEvent(chars id, const Context& json) override;

public:
	float mScrollPos;
	float mScrollSize;
	float mScrollTarget;
	Rects mClouds;
	bool mActorReady;
	Point mActorBegin;
	Point mActorPos;
	Point mActorVec;
	sint32 mGrabbedCloudID;
	uint64 mGrabbedMsec;
	bool mGameOver;
};
