#pragma once
#include <boss.hpp>
#include <service/boss_zay.hpp>
#include "../../missioncoding/source/classes.hpp"

#define MISSION_ID 6
#define MISSION_CLASS BulletsExample
#define MISSION_NAME "06.Bullets"

namespace BulletsExampleClasses
{
	enum class Eff {GRAVITY, LINEUP, CRASH};
}
using namespace BulletsExampleClasses;

STEP_API_DECLARE(int OnAdd)(float& x, float& y, float& r) {}
STEP_API_DECLARE(int OnAct)(int& type, float& vx, float& vy) {}
STEP_API_DECLARE(Eff OnEff)(int type) {}
STEP_API_DECLARE(void OnRender)(ZayPanel& panel, int type) {}
STEP_API_DECLARE(int OnRenderUser)(ZayPanel& panel, int id, float vx, float vy) {}

class BulletsExample : public Example
{
private:
	static int OnAdd(float& x, float& y, float& r);
	static int OnAct(int& type, float& vx, float& vy);
    static Eff OnEff(int type);
	static void OnRender(ZayPanel& panel, int type);
	static int OnRenderUser(ZayPanel& panel, int id, float vx, float vy);
	class Beginner
	{
	public:
		static int OnAdd(float& x, float& y, float& r);
		static int OnAct(int& type, float& vx, float& vy);
        static Eff OnEff(int type);
		static void OnRender(ZayPanel& panel, int type);
		static int OnRenderUser(ZayPanel& panel, int id, float vx, float vy);
	};
	class Junior
	{
	public:
		static int OnAdd(float& x, float& y, float& r);
		static int OnAct(int& type, float& vx, float& vy);
        static Eff OnEff(int type);
		static void OnRender(ZayPanel& panel, int type);
		static int OnRenderUser(ZayPanel& panel, int id, float vx, float vy);
	};
	class Senior
	{
	public:
		static int OnAdd(float& x, float& y, float& r);
		static int OnAct(int& type, float& vx, float& vy);
        static Eff OnEff(int type);
		static void OnRender(ZayPanel& panel, int type);
		static int OnRenderUser(ZayPanel& panel, int id, float vx, float vy);
	};

public:
	BulletsExample()
	{
		mAdd = nullptr;
		mAct = nullptr;
        mEff = nullptr;
		mRender = nullptr;
		mRenderUser = nullptr;
	}
	~BulletsExample()
	{
	}

private:
	void SetLevelCore(int level) override
	{
		switch(level)
		{
		case 0:
			mAdd = OnAdd;
			mAct = OnAct;
            mEff = OnEff;
			mRender = OnRender;
			mRenderUser = OnRenderUser;
			break;
		case 1:
			mAdd = Beginner::OnAdd;
			mAct = Beginner::OnAct;
            mEff = Beginner::OnEff;
			mRender = Beginner::OnRender;
			mRenderUser = Beginner::OnRenderUser;
			break;
		case 2:
			mAdd = Junior::OnAdd;
			mAct = Junior::OnAct;
            mEff = Junior::OnEff;
			mRender = Junior::OnRender;
			mRenderUser = Junior::OnRenderUser;
			break;
		case 3:
			mAdd = Senior::OnAdd;
			mAct = Senior::OnAct;
            mEff = Senior::OnEff;
			mRender = Senior::OnRender;
			mRenderUser = Senior::OnRenderUser;
			break;
		}
	}

public:
	int (*mAdd)(float& x, float& y, float& r);
	int (*mAct)(int& type, float& vx, float& vy);
    Eff (*mEff)(int type);
	void (*mRender)(ZayPanel& panel, int type);
	int (*mRenderUser)(ZayPanel& panel, int id, float vx, float vy);
};
