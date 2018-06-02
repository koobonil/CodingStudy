#pragma once
#include <boss.hpp>
#include <service/boss_zay.hpp>
#include "../../missioncoding/source/classes.hpp"

#define MISSION_NAME "05.Polygons"

class PolygonsExample : public Example
{
private:
	static int OnAdd(float& x, float& y);
    static int OnAct(int& type, float& vx, float& vy);
    static int OnScore(int type);
    static void OnRender(ZayPanel& panel, int type, int ani);
	class Beginner
	{
	public:
		static int OnAdd(float& x, float& y);
		static int OnAct(int& type, float& vx, float& vy);
		static int OnScore(int type);
		static void OnRender(ZayPanel& panel, int type, int ani);
	};
	class Junior
	{
	public:
		static int OnAdd(float& x, float& y);
		static int OnAct(int& type, float& vx, float& vy);
		static int OnScore(int type);
		static void OnRender(ZayPanel& panel, int type, int ani);
	};
	class Senior
	{
	public:
		static int OnAdd(float& x, float& y);
		static int OnAct(int& type, float& vx, float& vy);
		static int OnScore(int type);
		static void OnRender(ZayPanel& panel, int type, int ani);
	};

public:
	PolygonsExample()
	{
		mAdd = nullptr;
		mAct = nullptr;
		mScore = nullptr;
		mRender = nullptr;
	}
	~PolygonsExample()
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
			mScore = OnScore;
			mRender = OnRender;
			break;
		case 1:
			mAdd = Beginner::OnAdd;
			mAct = Beginner::OnAct;
			mScore = Beginner::OnScore;
			mRender = Beginner::OnRender;
			break;
		case 2:
			mAdd = Junior::OnAdd;
			mAct = Junior::OnAct;
			mScore = Junior::OnScore;
			mRender = Junior::OnRender;
			break;
		case 3:
			mAdd = Senior::OnAdd;
			mAct = Senior::OnAct;
			mScore = Senior::OnScore;
			mRender = Senior::OnRender;
			break;
		}
	}

public:
	int (*mAdd)(float& x, float& y);
	int (*mAct)(int& type, float& vx, float& vy);
	int (*mScore)(int type);
	void (*mRender)(ZayPanel& panel, int type, int ani);
};
