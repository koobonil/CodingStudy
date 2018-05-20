#include <boss.hpp>
#include "rains.hpp"
MISSION_VIEW_DECLARE(MISSION_NAME, "rainsView", RainsExample)

#include <resource.hpp>
ZAY_DECLARE_VIEW_CLASS("rainsView", rainsData)

const float LandY = 0.9f;

ZAY_VIEW_API OnCommand(CommandType type, chars topic, id_share in, id_cloned_share* out)
{
    if(type == CT_Size)
    {
        sint32s WH(in);
        m->mScreenSize.w = WH[0];
        m->mScreenSize.h = WH[1];
        m->mTouchPos.x = WH[0] / 2;
        m->mTouchPos.y = WH[1] / 2;
    }
    else if(type == CT_Tick)
    {
        sint32 MakeCount = 2;
        for(sint32 i = 0; i < m->mRainCount; ++i)
        {
            auto& CurRain = m->mRains[i];
            if(CurRain.mVisible)
            {
                const float OldX = CurRain.mPos.x;
                const float OldY = CurRain.mPos.y;
                CurRain.mPos.x += CurRain.mPos.vx;
                CurRain.mPos.y += CurRain.mPos.vy;
                const int X1 = OldX * m->mScreenSize.w - m->mTouchPos.x;
                const int Y1 = OldY * m->mScreenSize.h - m->mTouchPos.y;
                const int X2 = CurRain.mPos.x * m->mScreenSize.w - m->mTouchPos.x;
                const int Y2 = CurRain.mPos.y * m->mScreenSize.h - m->mTouchPos.y;
                const int Result = exam->mTestRain(X1, Y1, X2, Y2);
                if(Result == 1)
                    CurRain.mVisible = false;
                else if(Result == 2 || Result == 3)
                {
                    if(-0.0001f < CurRain.mPos.vy && CurRain.mPos.vy < 0.0001f)
                        CurRain.mVisible = false;
                    else
                    {
                        if(Result == 2)
                            CurRain.mPos.vx = (Platform::Utility::Random() % 1000) / 200000.0f - 0.005f;
                        else CurRain.mPos.vx = (Platform::Utility::Random() % 1000) / 200000.0f;
                        CurRain.mPos.vy *= -0.5f;
                    }
                    CurRain.mPos.vy += 0.0001f;
                }
                else
                {
                    if(LandY + CurRain.mZ * 0.05f < CurRain.mPos.y)
                    {
                        CurRain.mPos.y = LandY + CurRain.mZ * 0.05f;
                        if(-0.0001f < CurRain.mPos.vy && CurRain.mPos.vy < 0.0001f)
                            CurRain.mVisible = false;
                        else
                        {
                            CurRain.mPos.vx = (Platform::Utility::Random() % 1000) / 100000.0f - 0.005f;
                            CurRain.mPos.vy *= -0.1f;
                        }
                    }
                    else CurRain.mPos.vy += 0.0001f;
                }
            }
            else if(0 < MakeCount--)
            {
                CurRain.mVisible = true;
                CurRain.mPos.x = (Platform::Utility::Random() % 1000) / 1000.0f;
                CurRain.mPos.y = (Platform::Utility::Random() % 1000) / 2000.0f - 0.5f;
                CurRain.mPos.vx = 0;
                CurRain.mPos.vy = 0;
                CurRain.mZ = (Platform::Utility::Random() % 1000) / 1000.0f;
            }
        }
        m->invalidate();
    }
}

ZAY_VIEW_API OnNotify(chars sender, chars topic, id_share in, id_cloned_share* out)
{
}

ZAY_VIEW_API OnGesture(GestureType type, sint32 x, sint32 y)
{
    if(type == GT_Pressed || type == GT_InDragging || type == GT_OutDragging)
    {
        m->mTouchPos.x = x;
        m->mTouchPos.y = y;
        m->invalidate();
    }
}

ZAY_VIEW_API OnRender(ZayPanel& panel)
{
    ZAY_RGB(panel, 0, 255, 255)
        panel.fill();

    ZAY_LTRB(panel, 0, panel.h() * LandY, panel.w(), panel.h())
    {
        ZAY_RGB(panel, 128, 255, 0)
            panel.fill();
        ZAY_XYWH(panel, 0, 0, panel.w(), 2)
        ZAY_RGBA(panel, 0, 0, 0, 128)
            panel.fill();
    }

    ZAY_XYWH(panel, m->mTouchPos.x, m->mTouchPos.y, panel.w() / 2, panel.h() / 2)
        exam->mRenderBowl(panel);

    for(sint32 i = 0; i < m->mRainCount; ++i)
    {
        if(m->mRains[i].mVisible)
        {
            const float x = m->mRains[i].mPos.x * panel.w();
            const float y = m->mRains[i].mPos.y * panel.h();
            const float vx = m->mRains[i].mPos.vx * panel.w() * 3;
            const float vy = m->mRains[i].mPos.vy * panel.h() * 3;
            const float r = m->mRains[i].mZ * 3;
            ZAY_RGBA(panel, 255, 0, 0, (2 - m->mRains[i].mZ) * 100)
            {
                if(2.5f < r)
                    panel.line(Point(x, y), Point(x - vx, y - vy), 1);
                ZAY_XYRR(panel, x, y, r, r)
                    panel.circle();
            }
        }
    }

	MissionCollector::RenderUI(panel);
}

rainsData::rainsData()
{
}

rainsData::~rainsData()
{
}
