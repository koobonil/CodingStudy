#include <boss.hpp>
#include "rains_example.hpp"
#include <resource.hpp>

////////////////////////////////////////////////////////////////////////////////
#define LEVEL_NUMBER LEVEL_SENIOR

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 0
MISSION_DECLARE("STEP_0")

static float WaterHeight = 0;

int RainsExample::Senior::OnTestRain(int x1, int y1, int x2, int y2)
{
	if(-120 < y2 && y2 < 0)
    {
        if(x1 <= -50 && -50 <= x2) return 1;
        if(x2 <= -50 && -50 <= x1) return 1;
        if(x1 <= 50 && 50 <= x2) return 1;
        if(x2 <= 50 && 50 <= x1) return 1;
    }

    if(x2 < -100 || 100 < x2)
        return 0;
    else if(-50 < x2 && x2 < 50)
    {
        int WaterY = 0 - WaterHeight;
        if(y1 <= WaterY && WaterY <= y2)
        {
            WaterHeight = Math::MinF(WaterHeight + 0.2f, 120);
            return 1;
        }
    }
    else
    {
        if(y1 <= -120 && -120 <= y2)
            return (Platform::Utility::Random() % 2) + 2;
    }
    return 0;
}

void RainsExample::Senior::OnRenderBowl(ZayPanel& panel)
{
	ZAY_LTRB(panel, -50, 0 - WaterHeight, 50, 0)
    ZAY_RGB(panel, 255, 0, 0)
        panel.fill();

    Points a;
    a.AtAdding() = Point(-100, -120);
    a.AtAdding() = Point(-50, -120);
    a.AtAdding() = Point(-50, 0);
    a.AtAdding() = Point(50, 0);
    a.AtAdding() = Point(50, -120);
    a.AtAdding() = Point(100, -120);

    ZAY_RGB(panel, 0, 0, 0)
        panel.polyline(a, 3);
}
