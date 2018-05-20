﻿#include <boss.hpp>
#include "missioncoding_function.hpp"
#include "classes.hpp"

#include <resource.hpp>

ZAY_DECLARE_VIEW_CLASS("functionView", functionData)

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
    ZAY_RGB(panel, 192, 192, 192)
        panel.fill();
}

functionData::functionData()
{
}

functionData::~functionData()
{
}
