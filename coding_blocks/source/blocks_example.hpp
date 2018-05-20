#pragma once
#include <boss.hpp>
#include <service/boss_zay.hpp>
#include "../../missioncoding/source/classes.hpp"

#define MISSION_NAME "02.Blocks"

class BlocksExample : public Example
{
public:
	static String OnSetting(int& xcount, int& ycount);
    static void OnClick(int& id);
    static void OnRender(ZayPanel& panel, int id);

public:
	class Beginner
	{
	public:
		static String OnSetting(int& xcount, int& ycount);
		static void OnClick(int& id);
		static void OnRender(ZayPanel& panel, int id);
	};
	class Junior
	{
	public:
		static String OnSetting(int& xcount, int& ycount);
		static void OnClick(int& id);
		static void OnRender(ZayPanel& panel, int id);
	};
	class Senior
	{
	public:
		static String OnSetting(int& xcount, int& ycount);
		static void OnClick(int& id);
		static void OnRender(ZayPanel& panel, int id);
	};

public:
	BlocksExample()
	{
		mSetting = nullptr;
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
			mSetting = OnSetting;
			mClick = OnClick;
			mRender = OnRender;
			break;
		case 1:
			mSetting = Beginner::OnSetting;
			mClick = Beginner::OnClick;
			mRender = Beginner::OnRender;
			break;
		case 2:
			mSetting = Junior::OnSetting;
			mClick = Junior::OnClick;
			mRender = Junior::OnRender;
			break;
		case 3:
			mSetting = Senior::OnSetting;
			mClick = Senior::OnClick;
			mRender = Senior::OnRender;
			break;
		}
	}

public:
	String (*mSetting)(int& xcount, int& ycount);
	void (*mClick)(int& id);
	void (*mRender)(ZayPanel& panel, int id);
};
