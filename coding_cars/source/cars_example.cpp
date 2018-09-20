#include <boss.hpp>
#include "cars_example.hpp"
#include <resource.hpp>

////////////////////////////////////////////////////////////////////////////////
#define LEVEL_NUMBER LEVEL_USER

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 0
MISSION_DECLARE("STEP_0")

String CarsExample::OnInit(int& xCount, int& yCount, float& carSize, float& friction)
{
	xCount = 15;
	yCount = 15;
	carSize = 0.8;
	friction = 0.02;
	return
		"ooooooooooooooo"
		"o-------------o"
		"o----xxoxx----o"
		"o----xxoxx----o"
		"o----ooooo----o"
		"o----xxoxx----o"
		"o----xxoxx----o"
		"o-------------o"
		"o----oxxxo----o"
		"o----xoxox----o"
		"o----xxoxx----o"
		"o----xoxox----o"
		"o----oxxxo----o"
		"o-------------o"
		"ooooooooooooooo";
}

void CarsExample::OnCreateCar(Point& pos, float& angle360)
{
    pos.x = 3;
	pos.y = 7.5;
	angle360 = 90;
}

void CarsExample::OnCrashCar(Point otherPos, Point& vec)
{
    vec.x += -otherPos.x / 10;
	vec.y += -otherPos.y / 10;
}

void CarsExample::OnCrashTile(char tile, Wall wall, Point& vec)
{
    if(tile == 'o')
	{
		switch(wall)
		{
		case Wall::LEFT: vec.x = Math::AbsF(vec.x); break;
		case Wall::TOP: vec.y = Math::AbsF(vec.y); break;
		case Wall::RIGHT: vec.x = -Math::AbsF(vec.x); break;
		case Wall::BOTTOM: vec.y = -Math::AbsF(vec.y); break;
		}
	}
}

void CarsExample::OnInput(Joystick joystick, Point& vec, float& angle360)
{
    if(joystick == Joystick::ARROW_L)
		angle360 = Math::CycleF(angle360 - 5, 0, 360);
	if(joystick == Joystick::ARROW_R)
		angle360 = Math::CycleF(angle360 + 5, 0, 360);
	if(joystick == Joystick::BUTTON1)
	{
		vec.x += 0.05 * Math::Sin(Math::ToRadian(angle360));
		vec.y += -0.05 * Math::Cos(Math::ToRadian(angle360));
	}
}

void CarsExample::OnRenderCar(ZayPanel& panel, float angle360, int fart)
{
    ZAY_RGBA(panel, 0, 0, 0, 64)
		panel.circle();

	float Rad = Math::ToRadian(angle360);

	Points Dots;
	Dots.AtAdding() = Point(+0.0, -1.0);
	Dots.AtAdding() = Point(+1.0, -0.2);
	Dots.AtAdding() = Point(+1.0, +0.8);
	Dots.AtAdding() = Point(-1.0, +0.8);
	Dots.AtAdding() = Point(-1.0, -0.2);
	for(int i = 0; i < 5; ++i)
	{
		Point& CurDot = Dots.At(i);
		float NewX = CurDot.x * Math::Cos(Rad) - CurDot.y * Math::Sin(Rad);
		float NewY = CurDot.x * Math::Sin(Rad) + CurDot.y * Math::Cos(Rad);
		CurDot.x = (panel.w() + NewX * panel.w()) / 2;
		CurDot.y = (panel.h() + NewY * panel.h()) / 2;
	}

	ZAY_RGB(panel, 255, 192, 0)
		panel.polygon(Dots);

	if(0 <= fart && fart < 10)
	{
		ZAY_RGBA(panel, 96, 96, 96, 200 - fart * 20)
		ZAY_INNER(panel, -3 * fart)
			panel.circle();
	}
}

void CarsExample::OnRenderTile(ZayPanel& panel, char tile)
{
    if(tile == 'x')
	ZAY_RGBA(panel, 0, 0, 0, 128)
		panel.fill();

	if(tile == 'o')
	ZAY_RGB(panel, 0, 128, 0)
		panel.fill();
}
