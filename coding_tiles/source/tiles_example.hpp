#pragma once
#include <boss.hpp>
#include <service/boss_zay.hpp>
#include "../../missioncoding/source/classes.hpp"

#define MISSION_NAME "04.Tiles"

class TilesExample : public Example
{
private:
	static void OnInit(int& xcount, int& ycount);
	static int OnTick();
	static void OnButton(int id);
	static void OnRenderTile(ZayPanel& panel, int x, int y);
	class Beginner
	{
	public:
		static void OnInit(int& xcount, int& ycount);
		static int OnTick();
		static void OnButton(int id);
		static void OnRenderTile(ZayPanel& panel, int x, int y);
	};
	class Junior
	{
	public:
		static void OnInit(int& xcount, int& ycount);
		static int OnTick();
		static void OnButton(int id);
		static void OnRenderTile(ZayPanel& panel, int x, int y);
	};
	class Senior
	{
	public:
		static void OnInit(int& xcount, int& ycount);
		static int OnTick();
		static void OnButton(int id);
		static void OnRenderTile(ZayPanel& panel, int x, int y);
	};

public:
	TilesExample()
	{
		mInit = nullptr;
		mTick = nullptr;
		mButton = nullptr;
		mRenderTile = nullptr;
	}
	~TilesExample()
	{
	}

private:
	void SetLevelCore(int level) override
	{
		switch(level)
		{
		case 0:
			mInit = OnInit;
			mTick = OnTick;
			mButton = OnButton;
			mRenderTile = OnRenderTile;
			break;
		case 1:
			mInit = Beginner::OnInit;
			mTick = Beginner::OnTick;
			mButton = Beginner::OnButton;
			mRenderTile = Beginner::OnRenderTile;
			break;
		case 2:
			mInit = Junior::OnInit;
			mTick = Junior::OnTick;
			mButton = Junior::OnButton;
			mRenderTile = Junior::OnRenderTile;
			break;
		case 3:
			mInit = Senior::OnInit;
			mTick = Senior::OnTick;
			mButton = Senior::OnButton;
			mRenderTile = Senior::OnRenderTile;
			break;
		}
	}

public:
	void (*mInit)(int& xcount, int& ycount);
	int (*mTick)();
	void (*mButton)(int id);
	void (*mRenderTile)(ZayPanel& panel, int x, int y);
};
