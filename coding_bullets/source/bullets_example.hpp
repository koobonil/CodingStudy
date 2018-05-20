#pragma once
#include <boss.hpp>
#include <service/boss_zay.hpp>
#include "../../missioncoding/source/classes.hpp"

#define MISSION_NAME "06.Bullets"

class BulletsExample : public Example
{
public:
	static int OnAdd(float& x, float& y);
	static int OnAct(int& type, float& vx, float& vy);
	static void OnRender(ZayPanel& panel, int type);
	static int OnRenderUser(ZayPanel& panel, int id, float vx, float vy);

public:
	class Beginner
	{
	public:
		static int OnAdd(float& x, float& y);
		static int OnAct(int& type, float& vx, float& vy);
		static void OnRender(ZayPanel& panel, int type);
		static int OnRenderUser(ZayPanel& panel, int id, float vx, float vy);
	};
	class Junior
	{
	public:
		static int OnAdd(float& x, float& y);
		static int OnAct(int& type, float& vx, float& vy);
		static void OnRender(ZayPanel& panel, int type);
		static int OnRenderUser(ZayPanel& panel, int id, float vx, float vy);
	};
	class Senior
	{
	public:
		static int OnAdd(float& x, float& y);
		static int OnAct(int& type, float& vx, float& vy);
		static void OnRender(ZayPanel& panel, int type);
		static int OnRenderUser(ZayPanel& panel, int id, float vx, float vy);
	};

public:
	BulletsExample()
	{
		mAdd = nullptr;
		mAct = nullptr;
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
			mRender = OnRender;
			mRenderUser = OnRenderUser;
			break;
		case 1:
			mAdd = Beginner::OnAdd;
			mAct = Beginner::OnAct;
			mRender = Beginner::OnRender;
			mRenderUser = Beginner::OnRenderUser;
			break;
		case 2:
			mAdd = Junior::OnAdd;
			mAct = Junior::OnAct;
			mRender = Junior::OnRender;
			mRenderUser = Junior::OnRenderUser;
			break;
		case 3:
			mAdd = Senior::OnAdd;
			mAct = Senior::OnAct;
			mRender = Senior::OnRender;
			mRenderUser = Senior::OnRenderUser;
			break;
		}
	}

public:
	int (*mAdd)(float& x, float& y);
	int (*mAct)(int& type, float& vx, float& vy);
	void (*mRender)(ZayPanel& panel, int type);
	int (*mRenderUser)(ZayPanel& panel, int id, float vx, float vy);
};
