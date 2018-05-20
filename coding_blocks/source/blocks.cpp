﻿#include <boss.hpp>
#include "blocks.hpp"
MISSION_VIEW_DECLARE(MISSION_NAME, "blocksView", BlocksExample)

#include <resource.hpp>
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

    for(sint32 y = 0; y < m->YCount; ++y)
    for(sint32 x = 0; x < m->XCount; ++x)
    {
        const sint32 i = x + y * m->XCount;
        ZAY_LTRB_UI(panel, panel.w() * x / m->XCount, panel.h() * y / m->YCount,
            panel.w() * (x + 1) / m->XCount, panel.h() * (y + 1) / m->YCount, String::Format("%d", i),
            ZAY_GESTURE_T(t, i)
            {
                if(t == GT_Pressed)
                {
                    exam->mClick(m->Blocks.At(i));
                }
                else if(t == GT_Dropping)
                {
                    exam->mClick(m->Blocks.At(i));
                }
            })
        {
            if((x + y) & 1)
            ZAY_RGB(panel, 176, 176, 176)
                panel.fill();
            exam->mRender(panel, m->Blocks[i]);
        }
    }

	MissionCollector::RenderUI(panel);
}

blocksData::blocksData()
{
    XCount = 0;
    YCount = 0;
    String JsonText = exam->mSetting(XCount, YCount);
    Context Json(ST_Json, SO_OnlyReference, "[" + JsonText + "]");
    for(sint32 i = 0, iend = XCount * YCount; i < iend; ++i)
        Blocks.AtAdding() = Json[i].GetInt(0);
}

blocksData::~blocksData()
{
}
