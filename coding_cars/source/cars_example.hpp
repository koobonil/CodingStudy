#pragma once
#include <boss.hpp>
#include <service/boss_zay.hpp>
#include "../../missioncoding/source/classes.hpp"

#define MISSION_NAME "08.Cars"

namespace CarsExampleClasses
{
	enum class Wall {LEFT, TOP, RIGHT, BOTTOM};
	enum class Joystick {ARROW_L, ARROW_U, ARROW_R, ARROW_D, BUTTON1, BUTTON2};
}
using namespace CarsExampleClasses;

class CarsExample : public Example
{
private:
	static String OnInit(int& xCount, int& yCount, float& carSize, float& friction);
	static void OnCreateCar(Point& pos, float& angle360);
	static void OnCrashCar(Point otherPos, Point& vec);
	static void OnCrashTile(char tile, Wall wall, Point& vec);
	static void OnInput(Joystick joystick, Point& vec, float& angle360);
	static void OnRenderCar(ZayPanel& panel, float angle360, int fart);
	static void OnRenderTile(ZayPanel& panel, char tile);
	class Beginner
	{
	public:
		static String OnInit(int& xCount, int& yCount, float& carSize, float& friction);
		static void OnCreateCar(Point& pos, float& angle360);
		static void OnCrashCar(Point otherPos, Point& vec);
		static void OnCrashTile(char tile, Wall wall, Point& vec);
		static void OnInput(Joystick joystick, Point& vec, float& angle360);
		static void OnRenderCar(ZayPanel& panel, float angle360, int fart);
		static void OnRenderTile(ZayPanel& panel, char tile);
	};
	class Junior
	{
	public:
		static String OnInit(int& xCount, int& yCount, float& carSize, float& friction);
		static void OnCreateCar(Point& pos, float& angle360);
		static void OnCrashCar(Point otherPos, Point& vec);
		static void OnCrashTile(char tile, Wall wall, Point& vec);
		static void OnInput(Joystick joystick, Point& vec, float& angle360);
		static void OnRenderCar(ZayPanel& panel, float angle360, int fart);
		static void OnRenderTile(ZayPanel& panel, char tile);
	};
	class Senior
	{
	public:
		static String OnInit(int& xCount, int& yCount, float& carSize, float& friction);
		static void OnCreateCar(Point& pos, float& angle360);
		static void OnCrashCar(Point otherPos, Point& vec);
		static void OnCrashTile(char tile, Wall wall, Point& vec);
		static void OnInput(Joystick joystick, Point& vec, float& angle360);
		static void OnRenderCar(ZayPanel& panel, float angle360, int fart);
		static void OnRenderTile(ZayPanel& panel, char tile);
	};

public:
	CarsExample()
	{
		mInit = nullptr;
		mCreateCar = nullptr;
		mCrashCar = nullptr;
		mCrashTile = nullptr;
		mInput = nullptr;
		mRenderCar = nullptr;
		mRenderTile = nullptr;
	}
	~CarsExample()
	{
	}

private:
	void SetLevelCore(int level) override
	{
		switch(level)
		{
		case 0:
			mInit = OnInit;
			mCreateCar = OnCreateCar;
			mCrashCar = OnCrashCar;
			mCrashTile = OnCrashTile;
			mInput = OnInput;
			mRenderCar = OnRenderCar;
			mRenderTile = OnRenderTile;
			break;
		case 1:
			mInit = Beginner::OnInit;
			mCreateCar = Beginner::OnCreateCar;
			mCrashCar = Beginner::OnCrashCar;
			mCrashTile = Beginner::OnCrashTile;
			mInput = Beginner::OnInput;
			mRenderCar = Beginner::OnRenderCar;
			mRenderTile = Beginner::OnRenderTile;
			break;
		case 2:
			mInit = Junior::OnInit;
			mCreateCar = Junior::OnCreateCar;
			mCrashCar = Junior::OnCrashCar;
			mCrashTile = Junior::OnCrashTile;
			mInput = Junior::OnInput;
			mRenderCar = Junior::OnRenderCar;
			mRenderTile = Junior::OnRenderTile;
			break;
		case 3:
			mInit = Senior::OnInit;
			mCreateCar = Senior::OnCreateCar;
			mCrashCar = Senior::OnCrashCar;
			mCrashTile = Senior::OnCrashTile;
			mInput = Senior::OnInput;
			mRenderCar = Senior::OnRenderCar;
			mRenderTile = Senior::OnRenderTile;
			break;
		}
	}

public:
	String (*mInit)(int& xCount, int& yCount, float& carSize, float& friction);
	void (*mCreateCar)(Point& pos, float& angle360);
	void (*mCrashCar)(Point otherPos, Point& vec);
	void (*mCrashTile)(char tile, Wall wall, Point& vec);
	void (*mInput)(Joystick joystick, Point& vec, float& angle360);
	void (*mRenderCar)(ZayPanel& panel, float angle360, int fart);
	void (*mRenderTile)(ZayPanel& panel, char tile);
};
