#pragma once
#include <boss.hpp>
#include <element/boss_point.hpp>
#include "../../missioncoding/source/classes.hpp"

#define MISSION_NAME "01.Balls"

namespace BallsExampleClasses
{
	enum class Wall {LEFT, TOP, RIGHT, BOTTOM};
}
using namespace BallsExampleClasses;

class BallsExample : public Example
{
private:
	static void OnTick(float sec, Point& pos, Point& vec);
	static bool OnCrashWall(Wall wall, Point& vec);
	static bool OnCrashBall(Point& vecA, Point& vecB);
	class Beginner
	{
	public:
		STEP_DECLARE
		static void OnTick(float sec, Point& pos, Point& vec);
		static bool OnCrashWall(Wall wall, Point& vec);
	};
	class Junior
	{
	public:
		static void OnTick(float sec, Point& pos, Point& vec);
		static bool OnCrashWall(Wall wall, Point& vec);
		static bool OnCrashBall(Point& vecA, Point& vecB);
	};
	class Senior
	{
	public:
		static void OnTick(float sec, Point& pos, Point& vec);
		static bool OnCrashWall(Wall wall, Point& vec);
		static bool OnCrashBall(Point& vecA, Point& vecB);
	};

public:
	BallsExample()
	{
		mTick = nullptr;
		mCrashWall = nullptr;
		mCrashBall = nullptr;
	}
	~BallsExample()
	{
	}

private:
	void SetLevelCore(int level) override
	{
		switch(level)
		{
		case 0:
			mTick = OnTick;
			mCrashWall = OnCrashWall;
			mCrashBall = OnCrashBall;
			break;
		case 1:
			STEP_SET(Beginner)
			mTick = Beginner::OnTick;
			mCrashWall = Beginner::OnCrashWall;
			mCrashBall = nullptr;
			break;
		case 2:
			mTick = Junior::OnTick;
			mCrashWall = Junior::OnCrashWall;
			mCrashBall = Junior::OnCrashBall;
			break;
		case 3:
			mTick = Senior::OnTick;
			mCrashWall = Senior::OnCrashWall;
			mCrashBall = Senior::OnCrashBall;
			break;
		}
	}

public:
	void (*mTick)(float sec, Point& pos, Point& vec);
	bool (*mCrashWall)(Wall wall, Point& vec);
	bool (*mCrashBall)(Point& vecA, Point& vecB);
};
