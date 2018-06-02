#include <boss.hpp>
#include "cars_example.hpp"
#include <resource.hpp>

MISSION_SENIOR_DECLARE(MISSION_NAME, 0, "STEP_0")

String CarsExample::Senior::OnInit(int& xCount, int& yCount, float& carSize, float& friction)
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

void CarsExample::Senior::OnCreateCar(Point& pos, float& angle360)
{
	pos.x = 5;
	pos.y = 5;
	angle360 = 90;
}

void CarsExample::Senior::OnCrashCar(Point otherPos, Point& vec)
{
}

void CarsExample::Senior::OnCrashTile(char tile, Wall wall, Point& vec)
{
}

void CarsExample::Senior::OnInput(Joystick joystick, Point& vec, float& angle360)
{
}

void CarsExample::Senior::OnRenderCar(ZayPanel& panel, float angle360, int fart)
{
	ZAY_RGBA(panel, 0, 0, 0, 64)
		panel.circle();
}

void CarsExample::Senior::OnRenderTile(ZayPanel& panel, char tile)
{
	if(tile != '-')
	ZAY_RGB(panel, 255, 0, 255)
		panel.fill();
}
