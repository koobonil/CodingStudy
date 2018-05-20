#include <boss.hpp>
#include "blocks_example.hpp"
#include <resource.hpp>

MISSION_JUNIOR_DECLARE(MISSION_NAME, 0, "STEP_0")

String BlocksExample::Junior::OnSetting(int& xcount, int& ycount)
{
	xcount = 50;
    ycount = 70;
    return "10,11,12,13,14,15,16,17,18,19";
}

static sint32 CurColor = 2;
void BlocksExample::Junior::OnClick(int& id)
{
	if(id < 10)
    {
        id = CurColor;
    }
    else
    {
        CurColor = id - 10;
    }
}

void BlocksExample::Junior::OnRender(ZayPanel& panel, int id)
{
	// 팔레트: 10~19
    ZAY_INNER(panel, 10)
    switch(id)
    {
    case 11: ZAY_RGB(panel, 255, 255, 255) panel.circle(); break;
    case 12: ZAY_RGB(panel,   0,   0,   0) panel.circle(); break;
    case 13: ZAY_RGB(panel, 255,   0,   0) panel.circle(); break;
    case 14: ZAY_RGB(panel,   0, 255,   0) panel.circle(); break;
    case 15: ZAY_RGB(panel,   0,   0, 255) panel.circle(); break;
    case 16: ZAY_RGB(panel, 255, 255,   0) panel.circle(); break;
    case 17: ZAY_RGB(panel, 255,   0, 255) panel.circle(); break;
    case 18: ZAY_RGB(panel,   0, 255, 255) panel.circle(); break;
    case 19: ZAY_RGB(panel, 128, 128, 128) panel.circle(); break;
    }
    if(id == 10)
    {
        ZAY_RGB(panel, 255, 0, 0)
        {
            panel.line(Point(0, 0), Point(panel.w(), panel.h()), 2);
            panel.line(Point(0, panel.h()), Point(panel.w(), 0), 2);
        }
    }

    // 컬러: 0~9
    switch(id)
    {
    case 1: ZAY_RGB(panel, 255, 255, 255) panel.fill(); break;
    case 2: ZAY_RGB(panel,   0,   0,   0) panel.fill(); break;
    case 3: ZAY_RGB(panel, 255,   0,   0) panel.fill(); break;
    case 4: ZAY_RGB(panel,   0, 255,   0) panel.fill(); break;
    case 5: ZAY_RGB(panel,   0,   0, 255) panel.fill(); break;
    case 6: ZAY_RGB(panel, 255, 255,   0) panel.fill(); break;
    case 7: ZAY_RGB(panel, 255,   0, 255) panel.fill(); break;
    case 8: ZAY_RGB(panel,   0, 255, 255) panel.fill(); break;
    case 9: ZAY_RGB(panel, 128, 128, 128) panel.fill(); break;
    }
}
