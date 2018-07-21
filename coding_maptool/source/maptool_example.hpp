#pragma once
#include <boss.hpp>
#include <service/boss_zay.hpp>
#include "../../missioncoding/source/classes.hpp"

#define MISSION_ID 9
#define MISSION_CLASS MapToolExample
#define MISSION_NAME "09.MapTool"

STEP_API_DECLARE(void OnInit)(Context& doc) {}
STEP_API_DECLARE(void OnQuit)(Context& doc) {}
STEP_API_DECLARE(void OnRender)(ZayPanel& panel, Context& doc) {}

class MapToolExample : public Example
{
private:
    static void OnInit(Context& doc);
    static void OnQuit(Context& doc);
	static void OnRender(ZayPanel& panel, Context& doc);
	class Beginner
	{
	public:
		static void OnInit(Context& doc);
        static void OnQuit(Context& doc);
	    static void OnRender(ZayPanel& panel, Context& doc);
	};
	class Junior
	{
	public:
		static void OnInit(Context& doc);
        static void OnQuit(Context& doc);
	    static void OnRender(ZayPanel& panel, Context& doc);
	};
	class Senior
	{
	public:
		static void OnInit(Context& doc);
        static void OnQuit(Context& doc);
	    static void OnRender(ZayPanel& panel, Context& doc);
	};

public:
	MapToolExample()
	{
		mInit = nullptr;
        mQuit = nullptr;
        mRender = nullptr;
	}
	~MapToolExample()
	{
	}

private:
	void SetLevelCore(int level) override
	{
		switch(level)
		{
		case 0:
			mInit = OnInit;
            mQuit = OnQuit;
            mRender = OnRender;
			break;
		case 1:
            mInit = Beginner::OnInit;
            mQuit = Beginner::OnQuit;
            mRender = Beginner::OnRender;
			break;
		case 2:
            mInit = Junior::OnInit;
            mQuit = Junior::OnQuit;
            mRender = Junior::OnRender;
			break;
		case 3:
            mInit = Senior::OnInit;
            mQuit = Senior::OnQuit;
            mRender = Senior::OnRender;
			break;
		}
	}

public:
	void (*mInit)(Context& doc);
    void (*mQuit)(Context& doc);
	void (*mRender)(ZayPanel& panel, Context& doc);
};
