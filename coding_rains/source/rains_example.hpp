﻿#pragma once
#include <boss.hpp>
#include <service/boss_zay.hpp>
#include "../../missioncoding/source/classes.hpp"

#define MISSION_ID 3
#define MISSION_CLASS RainsExample
#define MISSION_NAME "03.Rains"

STEP_API_DECLARE(int OnTestRain)(int x1, int y1, int x2, int y2) {}
STEP_API_DECLARE(void OnRenderBowl)(ZayPanel& panel) {}

class RainsExample : public Example
{
private:
	static int OnTestRain(int x1, int y1, int x2, int y2);
	static void OnRenderBowl(ZayPanel& panel);
	class Beginner
	{
	public:
		static int OnTestRain(int x1, int y1, int x2, int y2);
		static void OnRenderBowl(ZayPanel& panel);
	};
	class Junior
	{
	public:
		static int OnTestRain(int x1, int y1, int x2, int y2);
		static void OnRenderBowl(ZayPanel& panel);
	};
	class Senior
	{
	public:
		static int OnTestRain(int x1, int y1, int x2, int y2);
		static void OnRenderBowl(ZayPanel& panel);
	};

public:
	RainsExample()
	{
		mTestRain = nullptr;
		mRenderBowl = nullptr;
	}
	~RainsExample()
	{
	}

private:
	void SetLevelCore(int level) override
	{
		switch(level)
		{
		case 0:
			mTestRain = OnTestRain;
			mRenderBowl = OnRenderBowl;
			break;
		case 1:
			mTestRain = Beginner::OnTestRain;
			mRenderBowl = Beginner::OnRenderBowl;
			break;
		case 2:
			mTestRain = Junior::OnTestRain;
			mRenderBowl = Junior::OnRenderBowl;
			break;
		case 3:
			mTestRain = Senior::OnTestRain;
			mRenderBowl = Senior::OnRenderBowl;
			break;
		}
	}

public:
	int (*mTestRain)(int x1, int y1, int x2, int y2);
	void (*mRenderBowl)(ZayPanel& panel);
};
