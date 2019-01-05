#include <boss.hpp>
#include "bullets_example.hpp"
#include <resource.hpp>

////////////////////////////////////////////////////////////////////////////////
#define LEVEL_NUMBER LEVEL_USER

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 0
MISSION_DECLARE("STEP_0")

enum {Unknown, Star, People, Enemy};
static int Count = 0;

int BulletsExample::OnAdd(float& x, float& y, float& r)
{
    Count++;
	x = (Platform::Utility::Random() % 600) - 300.0f;
    y = (Platform::Utility::Random() % 600) - 300.0f;
    if(Count < 10) r = 50 + (Platform::Utility::Random() % 50);
    else if(Count < 30) r = 15;
    else r = 5;
    return (Count < 33)? 10 : -1;
}

int BulletsExample::OnAct(int& type, float& vx, float& vy)
{
	if(type == Unknown)
    {
        if(Count < 10) type = Star;
        else if(Count < 30) type = People;
        else type = Enemy;
    }

    if(type != Star)
    {
        vx = ((Platform::Utility::Random() % 100) - 50.0f) * 0.03f;
        vy = ((Platform::Utility::Random() % 100) - 50.0f) * 0.03f;
        if(type == People)
            return 1000 + (Platform::Utility::Random() % 1000);
        else if(type == Enemy)
            return 10 + (Platform::Utility::Random() % 10);
    }
    return 0;
}

Eff BulletsExample::OnEff(int type)
{
    if(type == Star)
        return Eff::GRAVITY;
    else if(type == People)
        return Eff::LINEUP;
    else if(type == Enemy)
        return Eff::CRASH;
    return Eff::LINEUP;
}

void BulletsExample::OnRender(ZayPanel& panel, int type)
{
    if(type == Star)
    {
        ZAY_RGBA(panel, 128, 0, 128, 128)
            panel.circle();
    }
    else if(type == People)
    {
        ZAY_RGB(panel, 255, 255, 0)
            panel.circle();
    }
    else if(type == Enemy)
    {
        ZAY_RGB(panel, 255, 0, 0)
            panel.fill();
    }
}

int BulletsExample::OnRenderUser(ZayPanel& panel, int id, float vx, float vy)
{
    const int SizeR = 30;
    Points Dots;
    Dots.AtAdding() = Point(-SizeR / 2, -SizeR / 4);
    Dots.AtAdding() = Point(0, -SizeR);
    Dots.AtAdding() = Point(+SizeR / 2, -SizeR / 4);
    Dots.AtAdding() = Point(+SizeR / 2, +SizeR / 2);
    Dots.AtAdding() = Point(-SizeR / 2, +SizeR / 2);

    const float Rad = Math::Atan(vx, vy) + Math::PI() / 2;
    for(sint32 i = 0, iend = Dots.Count(); i < iend; ++i)
    {
        const float NewX = Dots[i].x * Math::Cos(Rad) - Dots[i].y * Math::Sin(Rad);
        const float NewY = Dots[i].x * Math::Sin(Rad) + Dots[i].y * Math::Cos(Rad);
        Dots.At(i) = Point(NewX, NewY);
    }

    ZAY_RGB(panel, 0, 128, 192)
        panel.polygon(Dots);
	return SizeR * 2 / 3;
}
