#include <boss.hpp>
#include "tiles_example.hpp"
#include <resource.hpp>

////////////////////////////////////////////////////////////////////////////////
#define LEVEL_NUMBER LEVEL_BEGINNER

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 0
MISSION_DECLARE("STEP_0")

// 리소스
static const Color TileColor[7] = {
    {180, 0, 0, 80},
    {0, 180, 0, 80},
    {0, 0, 180, 80},
    {180, 0, 180, 80},
    {180, 64, 0, 80},
    {0, 180, 180, 80},
    {64, 64, 64, 80}
};
static point64 TileType[7][4] = {
    {{0,0}, {1,0}, {0,1}, {1,1}},
    {{0,0}, {0,-1}, {0,1}, {0,2}},
    {{0,0}, {0,-1}, {1,0}, {1,1}},
    {{0,0}, {1,-1}, {1,0}, {0,1}},
    {{0,0}, {0,-1}, {-1,0}, {1,0}},
    {{0,0}, {-1,-1}, {-1,0}, {1,0}},
    {{0,0}, {1,-1}, {-1,0}, {1,0}}
};

// 실행정보
static int RunColorID = 3;
static int RunTypeID = 4;
static point64 RunPos = {5, -2};

static void ResetRun()
{
    RunColorID = Platform::Utility::Random() % 7;
    RunTypeID = Platform::Utility::Random() % 7;
    RunPos.y = -2;
}

void TilesExample::Beginner::OnInit(int& xcount, int& ycount)
{
    xcount = 10;
    ycount = 20;

    ResetRun();
}

int TilesExample::Beginner::OnTick()
{
    RunPos.y++;
    return 120;
}

void TilesExample::Beginner::OnButton(int id)
{
    if(0 <= id)
    {
        RunPos.x = id;
    }
    else if(id == -1)
    {
        for(int i = 0; i < 4; ++i)
        {
            point64 OldPos = TileType[RunTypeID][i];
            point64 NewPos;
            NewPos.x = OldPos.y;
            NewPos.y = -OldPos.x;
            TileType[RunTypeID][i] = NewPos;
        }
    }
    else if(id == -2)
    {
        for(int i = 0; i < 4; ++i)
        {
            point64 OldPos = TileType[RunTypeID][i];
            point64 NewPos;
            NewPos.x = -OldPos.y;
            NewPos.y = OldPos.x;
            TileType[RunTypeID][i] = NewPos;
        }
    }
    else if(id == -3)
    {
        RunPos.y++;
    }
}

void TilesExample::Beginner::OnRenderTile(ZayPanel& panel, int x, int y)
{
    // 배경
    if((x + y) % 2 == 1)
    {
        ZAY_RGB(panel, 150, 90, 30)
            panel.stretch(R("back"), true);
    }
    else
    {
        ZAY_RGB(panel, 90, 120, 30)
            panel.stretch(R("back"), true);
    }

    // 런타일
    for(int i = 0; i < 4; ++i)
    {
        point64 CurPos = TileType[RunTypeID][i];

        if(x == RunPos.x + CurPos.x && y == RunPos.y + CurPos.y)
        {
            ZAY_COLOR(panel, TileColor[RunColorID])
                panel.stretch(R("tile"), true);
        }
    }
}
