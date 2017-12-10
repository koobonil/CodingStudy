#include <boss.hpp>
#include "blocks.hpp"

#include <r.hpp>

ZAY_DECLARE_VIEW_CLASS("blocksView", blocksData)

ZAY_VIEW_API OnCommand(CommandType type, chars topic, id_share in, id_cloned_share* out)
{
}

ZAY_VIEW_API OnNotify(chars sender, chars topic, id_share in, id_cloned_share* out)
{
}

ZAY_VIEW_API OnGesture(GestureType type, sint32 x, sint32 y)
{
}

ZAY_VIEW_API OnRender(ZayPanel& panel)
{
    ZAY_RGB(panel, 160, 160, 160)
        panel.fill();

    for(sint32 y = 0; y < m->mYCount; ++y)
    for(sint32 x = 0; x < m->mXCount; ++x)
    {
        const sint32 i = x + y * m->mXCount;
        ZAY_LTRB_UI(panel, panel.w() * x / m->mXCount, panel.h() * y / m->mYCount,
            panel.w() * (x + 1) / m->mXCount, panel.h() * (y + 1) / m->mYCount, String::Format("%d", i),
            ZAY_GESTURE_T(t, i)
            {
                if(t == GT_Pressed)
                {
                    m->OnClick(m->mBlocks.At(i));
                }
                else if(t == GT_Dropping)
                {
                    m->OnClick(m->mBlocks.At(i));
                }
            })
        {
            if((x + y) & 1)
            ZAY_RGB(panel, 176, 176, 176)
                panel.fill();
            m->OnRender(panel, m->mBlocks[i]);
        }
    }
}

blocksData::blocksData()
{
    mXCount = 0;
    mYCount = 0;
    String JsonText = OnSetting(mXCount, mYCount);
    Context Json(ST_Json, SO_OnlyReference, "[" + JsonText + "]");
    for(sint32 i = 0, iend = mXCount * mYCount; i < iend; ++i)
        mBlocks.AtAdding() = Json[i].GetInt(0);
}

blocksData::~blocksData()
{
}

String blocksData::OnSetting(sint32& xcount, sint32& ycount)
{
    return Setting(xcount, ycount);
}

void blocksData::OnClick(sint32& id)
{
    Click(id);
}

void blocksData::OnRender(ZayPanel& panel, sint32 id)
{
    Render(panel, id);
}

/*String blocksData::OnSetting(sint32& xcount, sint32& ycount)
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

void blocksData::OnClick(sint32& id)
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

void blocksData::OnRender(ZayPanel& panel, sint32 id)
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
}*/
