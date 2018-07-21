#pragma once
#include <boss.hpp>
#include <service/boss_zay.hpp>
#include "../../missioncoding/source/classes.hpp"

#define MISSION_ID 2
#define MISSION_CLASS BlocksExample
#define MISSION_NAME "02.Blocks"

STEP_API_DECLARE(String OnInit)(int& xcount, int& ycount) {}
STEP_API_DECLARE(void OnClick)(int& id) {}
STEP_API_DECLARE(void OnRender)(ZayPanel& panel, int id) {}

class BlocksExample : public Example
{
private:
	static String OnInit(int& xcount, int& ycount);
    static void OnClick(int& id);
    static void OnRender(ZayPanel& panel, int id);
	class Beginner
	{
	public:
		static String OnInit(int& xcount, int& ycount);
		static void OnClick(int& id);
		static void OnRender(ZayPanel& panel, int id);
	};
	class Junior
	{
	public:
		static String OnInit(int& xcount, int& ycount);
		static void OnClick(int& id);
		static void OnRender(ZayPanel& panel, int id);
	};
	class Senior
	{
	public:
		static String OnInit(int& xcount, int& ycount);
		static void OnClick(int& id);
		static void OnRender(ZayPanel& panel, int id);
	};

public:
	BlocksExample()
	{
		mInit = nullptr;
		mClick = nullptr;
		mRender = nullptr;
	}
	~BlocksExample()
	{
	}

private:
	void SetLevelCore(int level) override
	{
		switch(level)
		{
		case 0:
			mInit = OnInit;
			mClick = OnClick;
			mRender = OnRender;
			break;
		case 1:
			mInit = Beginner::OnInit;
			mClick = Beginner::OnClick;
			mRender = Beginner::OnRender;
			break;
		case 2:
			mInit = Junior::OnInit;
			mClick = Junior::OnClick;
			mRender = Junior::OnRender;
			break;
		case 3:
			mInit = Senior::OnInit;
			mClick = Senior::OnClick;
			mRender = Senior::OnRender;
			break;
		}
	}

public:
	String (*mInit)(int& xcount, int& ycount);
	void (*mClick)(int& id);
	void (*mRender)(ZayPanel& panel, int id);
};
