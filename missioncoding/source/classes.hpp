#pragma once
#include <boss.hpp>
#include <service/boss_zay.hpp>

////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable:4005) // 매크로 재정의

#define MISSION_VIEW_DECLARE(NAME, VIEW, CLASS) \
	static CLASS* exam = new CLASS;\
	static autorun _mission_ = MissionCollector::AddMission("" NAME, "" VIEW, exam);
#define MISSION_DECLARE(COMMENT) \
	static autorun STEP_VAL(_mission_c) = MissionCollector::AddComment(MISSION_NAME, LEVEL_NUMBER, STEP_NUMBER, COMMENT, __FILE__, __LINE__);

#define STEP_VAL_REPEAT_0(X) X##0
#define STEP_VAL_REPEAT_1(X) X##1
#define STEP_VAL_REPEAT_2(X) X##2
#define STEP_VAL_REPEAT_3(X) X##3
#define STEP_VAL_REPEAT_4(X) X##4
#define STEP_VAL_REPEAT_5(X) X##5
#define STEP_VAL_REPEAT_6(X) X##6
#define STEP_VAL_REPEAT_7(X) X##7
#define STEP_VAL_REPEAT_8(X) X##8
#define STEP_VAL_REPEAT_9(X) X##9
#define STEP_VAL_EXPAND(...) __VA_ARGS__
#define STEP_VAL_REPEAT__(N, X) STEP_VAL_EXPAND(STEP_VAL_REPEAT_##N)(X)
#define STEP_VAL_REPEAT_(N, X) STEP_VAL_REPEAT__(N, X)
#define STEP_VAL_REPEAT(N, X) STEP_VAL_REPEAT_(STEP_VAL_EXPAND(N), X)
#define STEP_VAL(X) STEP_VAL_REPEAT_(STEP_VAL_EXPAND(STEP_NUMBER), X)

#define LEVEL_USER 0
#define LEVEL_BEGINNER 1
#define LEVEL_JUNIOR 2
#define LEVEL_SENIOR 3

#define STEP_DECLARE static int OnStep(int step);
#define STEP_SET(GROUP) mStep = GROUP::OnStep;
#define STEP_API_DECLARE(...) template<int, int, int> __VA_ARGS__##_Step
#define STEP_API_CALL(STEP, NAME) NAME##_Step<MISSION_ID, LEVEL_NUMBER, STEP>
#define STEP_API(...) template<> __VA_ARGS__##_Step<MISSION_ID, LEVEL_NUMBER, STEP_NUMBER>

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
class MissionComment
{
	BOSS_DECLARE_NONCOPYABLE_CLASS(MissionComment)
public:
	MissionComment();
	~MissionComment();
	MissionComment(MissionComment&& rhs);
    MissionComment& operator=(MissionComment&& rhs);

public:
	String mSubject;
	String mFile;
	sint32 mLine;
};
typedef Array<MissionComment> MissionComments;

////////////////////////////////////////////////////////////////////////////////
class MissionElement
{
public:
	MissionElement();
	~MissionElement();

public:
	void SetLevel(sint32 level);
	void SetStep(sint32 step);
	void UpdateComment();
private:
	void UpdateCommentCore(chars code);

public:
	String mName;
	String mView;
	MissionComments mComments[4];
	Example* mExample;
	sint32 mExampleLevel;
	sint32 mExampleStep;
	sint32 mExampleStepCount;
	String mExampleCode;
};

////////////////////////////////////////////////////////////////////////////////
class MissionCollector
{
public:
	static autorun AddMission(chars name, chars view, Example* example);
	static autorun AddComment(chars name, sint32 level, sint32 step, chars subject, chars file, sint32 line);
	static MissionElement* FocusedMission();
	static void RenderUI(ZayPanel& panel);
};

////////////////////////////////////////////////////////////////////////////////
class MissionData
{
public:
	static MissionData& ST() {static MissionData _; return _;}

public:
	MissionElement& GetElement(chars name);
	Map<MissionElement>& elements();
	void SetFocus(chars name);
	chars focus() const;

private:
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
	void RenderUI(ZayPanel& panel);
	virtual void OnVisit(chars id) = 0;
	virtual void OnLeave(chars id) = 0;
	virtual void OnEvent(chars id, const Context& json) = 0;

private:
	void OnMessage(Context& json);

public:
	id_socket mSocket;
	String mSocketMessage;
	String mProjectName;
	String mSHCode;
	String mSHPassword;
	sint32 mSHCount;
	sint32 mScreenWidth;
	sint32 mScreenHeight;
	sint32 mLastIDX;
	sint32 mSumIDX;
};
