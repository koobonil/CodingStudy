#include <boss.hpp>
#include "syncu_joystick.hpp"

#include <resource.hpp>

ZAY_DECLARE_VIEW_CLASS("syncu_joystickView", syncu_joystickData)

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
    ZAY_RGB(panel, 224, 224, 224)
        panel.fill();

    const sint32 BaseSize = Math::Min(panel.w(), panel.h());
    ZAY_FONT(panel, BaseSize / 300.0f)
    {
        ZAY_INNER(panel, BaseSize / 20)
        {
            const Image& BGImage = R("joystick");
            const sint32 Width = BGImage.GetWidth();
            const sint32 Height = BGImage.GetHeight();
            const float Rate = Math::MinF(panel.w() / Width, panel.h() / Height);
            const sint32 RatedWidth = Width * Rate + 0.5f;
            const sint32 RatedHeight = Height * Rate + 0.5f;
            ZAY_XYWH(panel, (panel.w() - RatedWidth) / 2, (panel.h() - RatedHeight) / 2, RatedWidth, RatedHeight)
            {
                if(panel.stretch(BGImage, true) == haschild_ok)
                {
                    ZAY_CHILD_AT_UI(panel, 0, 0, "DEGREE",
                        ZAY_GESTURE_TXY(t, x, y)
                        {
                            branch;
                            jump(t == GT_Pressed || t == GT_InDragging || t == GT_OutDragging)
                            {
                                const auto& CurRect = m->rect("DEGREE");
                                const sint32 OX = x - (CurRect.l + CurRect.r) / 2;
                                const sint32 OY = y - (CurRect.t + CurRect.b) / 2;
                                const sint32 Degree = 360 + 135 + Math::ToDegree(Math::Atan(OX, OY));
                                m->OnDegreePressed(Degree % 360);
                                m->invalidate();
                            }
                            jump(t == GT_InReleased || t == GT_OutReleased)
                                m->OnDegreeReleased();
                        })
                    {
                        if(m->mDegree != -1)
                        switch(m->mDegree / 90)
                        {
                        case 0: panel.stretch(R("a_t"), true); break;
                        case 1: panel.stretch(R("a_r"), true); break;
                        case 2: panel.stretch(R("a_b"), true); break;
                        case 3: panel.stretch(R("a_l"), true); break;
                        }
                    }
                    ZAY_CHILD_AT_UI(panel, 1, 0, "BUTTON",
                        ZAY_GESTURE_TXY(t, x, y)
                        {
                            branch;
                            jump(t == GT_Pressed)
                            {
                                const auto& CurRect = m->rect("BUTTON");
                                const sint32 OX = x - (CurRect.l + CurRect.r) / 2;
                                const sint32 OY = y - (CurRect.t + CurRect.b) / 2;
                                const sint32 Degree = 360 + 135 + Math::ToDegree(Math::Atan(OX, OY));
                                m->OnButtonPressed((Degree % 360) / 90);
                            }
                            jump(t == GT_InReleased || t == GT_OutReleased)
                                m->OnButtonReleased();
                        })
                    {
                        if(m->mButton != -1)
                        switch(m->mButton)
                        {
                        case 0: panel.stretch(R("b_t"), true); break;
                        case 1: panel.stretch(R("b_r"), true); break;
                        case 2: panel.stretch(R("b_b"), true); break;
                        case 3: panel.stretch(R("b_l"), true); break;
                        }
                    }
                }
            }
        }

        m->RenderUI(panel);
    }
}

syncu_joystickData::syncu_joystickData()
{
    mEvent.At("payload").At("type").Set("joystick");
    mEvent.At("payload").At("degree").Set("-1");
    mEvent.At("payload").At("button").Set("-1");

    mDegree = -1;
    mButton = -1;
}

syncu_joystickData::~syncu_joystickData()
{
}

void syncu_joystickData::OnDegreePressed(sint32 degree)
{
    mDegree = degree;
    mEvent.At("idx").Set(String::FromInteger(++mLastIDX));
    mEvent.At("payload").At("degree").Set(String::FromInteger(degree));
    SendEvent();
}

void syncu_joystickData::OnDegreeReleased()
{
    mDegree = -1;
    mEvent.At("idx").Set(String::FromInteger(++mLastIDX));
    mEvent.At("payload").At("degree").Set("-1");
    SendEvent();
}

void syncu_joystickData::OnButtonPressed(sint32 button)
{
    mButton = button;
    mEvent.At("idx").Set(String::FromInteger(++mLastIDX));
    mEvent.At("payload").At("button").Set(String::FromInteger(button));
    SendEvent();
}

void syncu_joystickData::OnButtonReleased()
{
    mButton = -1;
    mEvent.At("idx").Set(String::FromInteger(++mLastIDX));
    mEvent.At("payload").At("button").Set("-1");
    SendEvent();
}
