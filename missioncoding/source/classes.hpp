#pragma once
#include <boss.hpp>
#include <service/boss_zay.hpp>

////////////////////////////////////////////////////////////////////////////////
#define MISSION_VIEW_DECLARE(NAME, VIEW, CLASS) \
	static CLASS* exam = new CLASS;\
	static autorun _mission_ = MissionCollector::AddMission("" NAME, "" VIEW, exam);
#define MISSION_USER_DECLARE(NAME, STEP, COMMENT) \
	static autorun _mission_0_##STEP##_ = MissionCollector::AddComment(NAME, 0, STEP, COMMENT);
#define MISSION_BEGINNER_DECLARE(NAME, STEP, COMMENT) \
	static autorun _mission_1_##STEP##_ = MissionCollector::AddComment(NAME, 1, STEP, COMMENT);
#define MISSION_JUNIOR_DECLARE(NAME, STEP, COMMENT) \
	static autorun _mission_2_##STEP##_ = MissionCollector::AddComment(NAME, 2, STEP, COMMENT);
#define MISSION_SENIOR_DECLARE(NAME, STEP, COMMENT) \
	static autorun _mission_3_##STEP##_ = MissionCollector::AddComment(NAME, 3, STEP, COMMENT);

#define STEP_DECLARE static int OnStep(int step);
#define STEP_SET(GROUP) mStep = GROUP::OnStep;
#define STEP_FUNC(NUMBER, NAME) NAME##_STEP##NUMBER

////////////////////////////////////////////////////////////////////////////////
class Example
{
public:
	Example();
	virtual ~Example();

public:
	int SetLevel(int level);
	void SetStep(int step);

protected:
	virtual void SetLevelCore(int level) = 0;

protected:
	int (*mStep)(int step);
};

////////////////////////////////////////////////////////////////////////////////
class MissionElement
{
public:
	MissionElement();
	~MissionElement();

public:
	void SetLevel(sint32 level);
	void SetStep(sint32 step);

public:
	String mName;
	String mView;
	Strings mComments[4];
	Example* mExample;
	sint32 mExampleLevel;
	sint32 mExampleStep;
	sint32 mExampleStepCount;
};

////////////////////////////////////////////////////////////////////////////////
class MissionCollector
{
public:
	static autorun AddMission(chars name, chars view, Example* example);
	static autorun AddComment(chars name, sint32 level, sint32 step, chars comment);
	static MissionElement* FocusedMission();
	static void RenderUI(ZayPanel& panel);
};

////////////////////////////////////////////////////////////////////////////////
class MissionData
{
public:
	static MissionData& ST() {static MissionData _; return _;}

public:
	Map<MissionElement> mElements;
	String mFocus;
};

////////////////////////////////////////////////////////////////////////////////
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
