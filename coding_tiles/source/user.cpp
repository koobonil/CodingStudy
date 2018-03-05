#include <boss.hpp>
#include <service/boss_zay.hpp>
#include <resource.hpp>

// 리소스
const Color TileColor[7] = {
    {180, 0, 0, 80},
    {0, 180, 0, 80},
    {0, 0, 180, 80},
    {180, 0, 180, 80},
    {180, 64, 0, 80},
    {0, 180, 180, 80},
    {64, 64, 64, 80}
};
point64 TileType[7][4] = {
    {{0,0}, {1,0}, {0,1}, {1,1}},
    {{0,0}, {0,-1}, {0,1}, {0,2}},
    {{0,0}, {0,-1}, {1,0}, {1,1}},
    {{0,0}, {1,-1}, {1,0}, {0,1}},
    {{0,0}, {0,-1}, {-1,0}, {1,0}},
    {{0,0}, {-1,-1}, {-1,0}, {1,0}},
    {{0,0}, {1,-1}, {-1,0}, {1,0}}
};

// 실행정보
int RunColorID = 3;
int RunTypeID = 4;
point64 RunPos = {5, -2};

void ResetRun()
{
    RunColorID = Platform::Utility::Random() % 7;
    RunTypeID = Platform::Utility::Random() % 7;
    RunPos.y = -2;
}

void Init(int& xcount, int& ycount)
{
    xcount = 10;
    ycount = 20;

    ResetRun();
}

sint32 Tick()
{
    RunPos.y++;
    return 120;
}

void Button(int id)
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

void RenderTile(ZayPanel& panel, int x, int y)
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
