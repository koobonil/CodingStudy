#include <boss.hpp>
#include "syncu_touchpad.hpp"

#include <resource.hpp>

ZAY_DECLARE_VIEW_CLASS("syncu_touchpadView", syncu_touchpadData)

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
        // 스크린
        const float WidthRate = (panel.w() - BaseSize / 5) / m->mScreenWidth;
        const float HeightRate = (panel.h() - BaseSize / 5) / m->mScreenHeight;
        const float ScreenRate = Math::MinF(WidthRate, HeightRate);
        const sint32 ScreenWidth = m->mScreenWidth * ScreenRate;
        const sint32 ScreenHeight = m->mScreenHeight * ScreenRate;
        const sint32 ScreenX = (panel.w() - ScreenWidth) / 2;
        const sint32 ScreenY = (panel.h() - ScreenHeight) / 2;
        ZAY_XYWH_UI_SCISSOR(panel, ScreenX, ScreenY, ScreenWidth, ScreenHeight, "SCREEN",
            ZAY_GESTURE_TXY(t, x, y, ScreenRate, ScreenX, ScreenY)
            {
                const sint32 X = Math::Clamp((x - ScreenX) / ScreenRate, 0, m->mScreenWidth);
                const sint32 Y = Math::Clamp((y - ScreenY) / ScreenRate, 0, m->mScreenHeight);
                branch;
                jump(t == GT_Pressed)
                {
                    m->mPressed = true;
                    m->mCursorPos.x = X;
                    m->mCursorPos.y = Y;
                    m->OnPressed();
                }
                jump(t == GT_InDragging)
                {
                    if(m->mPressed)
                    {
                        m->mCursorPos.x = X;
                        m->mCursorPos.y = Y;
                        m->OnDragging();
                        m->invalidate();
                    }
                }
                jump(t == GT_OutDragging)
                {
                    if(m->mPressed)
                    {
                        m->mPressed = false;
                        m->OnReleased();
                        m->invalidate();
                    }
                }
                jump(t == GT_InReleased)
                {
                    if(m->mPressed)
                    {
                        m->mPressed = false;
                        m->OnReleased();
                    }
                }
            })
        {
            // 배경
            ZAY_RGB(panel, 255, 255, 255)
                panel.fill();
            // 커서
            if(m->mPressed)
            {
                ZAY_RGBA(panel, 0, 0, 0, 64)
                ZAY_XYRR(panel, m->mCursorPos.x * ScreenRate, m->mCursorPos.y * ScreenRate,
                    BaseSize / 50, BaseSize / 50)
                    panel.circle();
            }
            // 외곽선
            ZAY_RGB(panel, 0, 0, 0)
            ZAY_INNER(panel, 1)
                panel.rect(1);
        }
        m->RenderUI(panel);
    }
}

syncu_touchpadData::syncu_touchpadData()
{
    mEvent.At("payload").At("type").Set("pressed");
    mEvent.At("payload").At("x").Set("0");
    mEvent.At("payload").At("y").Set("0");

    mScreenWidth = 1000;
    mScreenHeight = 1000;
    mPressed = false;
    mCursorPos.x = 0;
    mCursorPos.y = 0;
}

syncu_touchpadData::~syncu_touchpadData()
{
}

void syncu_touchpadData::OnPressed()
{
    mEvent.At("idx").Set(String::FromInteger(++mLastIDX));
    mEvent.At("payload").At("type").Set("pressed");
    mEvent.At("payload").At("x").Set(String::FromInteger(mCursorPos.x));
    mEvent.At("payload").At("y").Set(String::FromInteger(mCursorPos.y));
    SendEvent();
}

void syncu_touchpadData::OnDragging()
{
    mEvent.At("idx").Set(String::FromInteger(++mLastIDX));
    mEvent.At("payload").At("type").Set("dragging");
    mEvent.At("payload").At("x").Set(String::FromInteger(mCursorPos.x));
    mEvent.At("payload").At("y").Set(String::FromInteger(mCursorPos.y));
    SendEvent();
}

void syncu_touchpadData::OnReleased()
{
    mEvent.At("idx").Set(String::FromInteger(++mLastIDX));
    mEvent.At("payload").At("type").Set("released");
    SendEvent();
}
