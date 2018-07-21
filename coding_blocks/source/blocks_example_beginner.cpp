#include <boss.hpp>
#include "blocks_example.hpp"
#include <resource.hpp>

////////////////////////////////////////////////////////////////////////////////
#define LEVEL_NUMBER LEVEL_BEGINNER

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 0
MISSION_DECLARE("STEP_0")

String BlocksExample::Beginner::OnInit(int& xcount, int& ycount)
{
	xcount = 10;
    ycount = 10;
    return
        "1,2,3,4,5,6,7,8,9,10,"
        "0,0,0,0,0,0,0,0,0,0,"
        "0,0,0,0,0,0,0,0,0,0,"
        "0,0,0,0,0,0,0,0,0,0,"
        "0,0,0,0,0,0,0,0,0,0,"
        "0,0,0,0,0,0,0,0,0,0,"
        "0,0,0,0,0,0,0,0,0,0,"
        "0,0,0,0,0,0,0,0,0,0,"
        "0,0,0,0,0,0,0,0,0,0,"
        "0,0,0,0,0,0,0,0,0,0";
}

void BlocksExample::Beginner::OnClick(int& id)
{
	static sint32 SaveColor = 1;
    if(0 < id && id <= 10)
        SaveColor = id;
    else
    {
        if(SaveColor < 10)
            id = 10 + SaveColor;
        else id = 0;
    }
}

void BlocksExample::Beginner::OnRender(ZayPanel& panel, int id)
{
	static const Color Colors[9] = {Color::Black, Color::White,
        Color::Red, Color::Green, Color::Blue, Color::Cyan,
        Color::Magenta, Color::Yellow, Color::DarkRed};

    if(id == 0) {}
    else if(id <= 10)
    {
        if(id == 10)
        {
            ZAY_RGB(panel, 255, 0, 0)
            {
                panel.line(Point(0, 0), Point(panel.w(), panel.h()), 2);
                panel.line(Point(panel.w(), 0), Point(0, panel.h()), 2);
            }
        }
        else ZAY_INNER(panel, 10)
        {
            ZAY_XYWH(panel, 2, 2, panel.w(), panel.h())
            ZAY_RGBA(panel, 0, 0, 0, 128)
                panel.circle();
            ZAY_COLOR(panel, Colors[id - 1])
                panel.circle();
        }
    }
    else
    {
        id = id % 10;
        ZAY_INNER(panel, 5)
        ZAY_COLOR(panel, Colors[id - 1])
            panel.fill();
    }
}
