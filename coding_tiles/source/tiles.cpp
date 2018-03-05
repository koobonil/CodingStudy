#include <boss.hpp>
#include "tiles.hpp"

#include <resource.hpp>

ZAY_DECLARE_VIEW_CLASS("tilesView", tilesData)

ZAY_VIEW_API OnCommand(CommandType type, chars topic, id_share in, id_cloned_share* out)
{
    if(type == CT_Size)
    {
        sint32s WH(in);
        m->mTileW = (WH[0] - 20) / m->mXCount;
        m->mTileH = (WH[1] - 20) / m->mYCount;
        m->mTileX = (WH[0] - m->mTileW * m->mXCount) / 2;
        m->mTileY = (WH[1] - m->mTileH * m->mYCount) / 2;
    }
    else if(type == CT_Tick)
    {
        if(m->mTickWait-- == 0)
            m->mTickWait = m->OnTick();
        m->invalidate();
    }
}

ZAY_VIEW_API OnNotify(chars sender, chars topic, id_share in, id_cloned_share* out)
{
}

ZAY_VIEW_API OnGesture(GestureType type, sint32 x, sint32 y)
{
    if(type == GT_Moving)
        m->OnButton(Math::Clamp((x - m->mTileX) / m->mTileW, 0, m->mXCount - 1));
    else if(type == GT_Pressed)
        m->OnButton(-1);
    else if(type == GT_ExtendPress)
        m->OnButton(-2);
    else if(type == GT_WheelDown)
        m->OnButton(-3);
}

ZAY_VIEW_API OnRender(ZayPanel& panel)
{
    ZAY_RGB(panel, 0, 0, 0)
        panel.fill();
    for(sint32 y = 0; y < m->mYCount; ++y)
    for(sint32 x = 0; x < m->mXCount; ++x)
        ZAY_XYWH(panel, m->mTileX + m->mTileW * x, m->mTileY + m->mTileH * y, m->mTileW, m->mTileH)
            m->OnRenderTile(panel, x, y);
}

tilesData::tilesData()
{
    mXCount = 1;
    mYCount = 1;
    mTickWait = 0;
    mTileW = 0;
    mTileH = 0;
    mTileX = 0;
    mTileY = 0;
    OnInit(mXCount, mYCount);
}

tilesData::~tilesData()
{
}

void tilesData::OnInit(int& xcount, int& ycount)
{
    Init(xcount, ycount);
}

sint32 tilesData::OnTick()
{
    return Tick();
}

void tilesData::OnButton(int id)
{
    Button(id);
}

void tilesData::OnRenderTile(ZayPanel& panel, int x, int y)
{
    RenderTile(panel, x, y);
}
