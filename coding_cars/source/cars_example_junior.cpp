#include <boss.hpp>
#include "cars_example.hpp"
#include <resource.hpp>

////////////////////////////////////////////////////////////////////////////////
#define LEVEL_NUMBER LEVEL_JUNIOR

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 0
MISSION_DECLARE("STEP_0")

String CarsExample::Junior::OnInit(int& xCount, int& yCount, float& carSize, float& friction)
{
	xCount = 10;
	yCount = 10;
	carSize = 0.4;
	friction = 0.01;
	return
		"----------"
		"----xxx---"
		"----------"
		"--x-----x-"
		"--x-----x-"
		"--x-----x-"
		"----------"
		"----------"
		"----xxx---"
		"----------";
}

void CarsExample::Junior::OnCreateCar(Point& pos, float& angle360)
{
	pos.x = 5;
	pos.y = 5;
	angle360 = 90;
}

void CarsExample::Junior::OnCrashCar(Point otherPos, Point& vec)
{
}

void CarsExample::Junior::OnCrashTile(char tile, Wall wall, Point& vec)
{
}

void CarsExample::Junior::OnInput(Joystick joystick, Point& vec, float& angle360)
{
}

void CarsExample::Junior::OnRenderCar(ZayPanel& panel, float angle360, int fart)
{
	ZAY_RGBA(panel, 0, 0, 0, 64)
		panel.circle();
}

void CarsExample::Junior::OnRenderTile(ZayPanel& panel, char tile)
{
	if(tile != '-')
	ZAY_RGB(panel, 255, 0, 255)
		panel.fill();
}
