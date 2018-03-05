#pragma once
#include <service/boss_zay.hpp>

void Init(int& xcount, int& ycount);
sint32 Tick();
void Button(int id);
void RenderTile(ZayPanel& panel, int x, int y);

class tilesData : public ZayObject
{
public:
    tilesData();
    ~tilesData();

public:
    void OnInit(int& xcount, int& ycount);
    sint32 OnTick();
    void OnButton(int id);
    void OnRenderTile(ZayPanel& panel, int x, int y);

public:
    sint32 mXCount;
    sint32 mYCount;
    sint32 mTickWait;
    sint32 mTileW;
    sint32 mTileH;
    sint32 mTileX;
    sint32 mTileY;
};
