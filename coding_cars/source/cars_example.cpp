﻿#include <boss.hpp>
#include "cars_example.hpp"
#include <resource.hpp>

////////////////////////////////////////////////////////////////////////////////
#define LEVEL_NUMBER LEVEL_USER

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 0
MISSION_DECLARE("STEP_0")

String CarsExample::OnInit(int& xCount, int& yCount, float& carSize, float& friction)
{
	return "";
}

void CarsExample::OnCreateCar(Point& pos, float& angle360)
{
}

void CarsExample::OnCrashCar(Point otherPos, Point& vec)
{
}

void CarsExample::OnCrashTile(char tile, Wall wall, Point& vec)
{
}

void CarsExample::OnInput(Joystick joystick, Point& vec, float& angle360)
{
}

void CarsExample::OnRenderCar(ZayPanel& panel, float angle360, int fart)
{
}

void CarsExample::OnRenderTile(ZayPanel& panel, char tile)
{
}