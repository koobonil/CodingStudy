﻿#include <boss.hpp>
#include "maptool.hpp"
MISSION_VIEW_DECLARE(MISSION_NAME, "maptoolView", MapToolExample)

#include <resource.hpp>
ZAY_DECLARE_VIEW_CLASS("maptoolView", maptoolData)

ZAY_VIEW_API OnCommand(CommandType type, chars topic, id_share in, id_cloned_share* out)
{
}

ZAY_VIEW_API OnNotify(NotifyType type, chars topic, id_share in, id_cloned_share* out)
{
}

ZAY_VIEW_API OnGesture(GestureType type, sint32 x, sint32 y)
{
}

ZAY_VIEW_API OnRender(ZayPanel& panel)
{
    ZAY_RGB(panel, 128, 128, 128)
        panel.fill();

    exam->mRender(panel, m->mDoc);

	MissionCollector::RenderUI(panel);
}

maptoolData::maptoolData()
{
    exam->mInit(mDoc);
}

maptoolData::~maptoolData()
{
    exam->mQuit(mDoc);
}
