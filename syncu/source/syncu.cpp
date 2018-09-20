#include <boss.hpp>
#include "syncu.hpp"

#include <resource.hpp>

ZAY_DECLARE_VIEW_CLASS("syncuView", syncuData)

ZAY_VIEW_API OnCommand(CommandType type, chars topic, id_share in, id_cloned_share* out)
{
    if(type == CT_Tick)
    {
        for(sint32 i = 0; i < 10; ++i)
        {
            if(m->mSHScroll[i] != m->mSHScrollPhy[i])
            {
                m->mSHScrollPhy[i] = m->mSHScrollPhy[i] * 0.8f + m->mSHScroll[i] * 0.2f;
                m->invalidate();
            }
        }
    }
}

ZAY_VIEW_API OnNotify(chars sender, chars topic, id_share in, id_cloned_share* out)
{
}

ZAY_VIEW_API OnGesture(GestureType type, sint32 x, sint32 y)
{
}

ZAY_VIEW_API OnRender(ZayPanel& panel)
{
    ZAY_RGB(panel, 255, 255, 255)
        panel.fill();

    const sint32 BaseSize = Math::Min(panel.w(), panel.h());
    ZAY_FONT(panel, BaseSize / 300.0f)
    {
        // 제목
        ZAY_LTRB(panel, 0, 0, panel.w(), panel.h() * 0.4)
        {
            ZAY_RGB(panel, 0, 0, 255)
            ZAY_FONT(panel, 4)
                panel.text("Sync U", UIFA_CenterMiddle);
        }

        // SH코드
        const sint32 SHWidth = BaseSize / 11;
        const sint32 SHHeight = panel.h();
        ZAY_XYWH(panel, panel.w() / 2 - SHWidth * 5, panel.h() * 0.4, SHWidth * 10, panel.h() * 0.2)
        ZAY_FONT(panel, 1.3)
        {
            const chars WordCode[10] = {"S", "H", 0, 0, 0, "-", 0, 0, 0, 0};
            for(sint32 i = 0; i < 10; ++i)
            {
                const String UIName = String::Format("SH-%d", i);
                ZayPanel::SubGestureCB NullCB = nullptr;
                ZAY_XYWH_UI_SCISSOR(panel, SHWidth * i, 0, SHWidth, panel.h(), UIName, (WordCode[i])? NullCB :
                    ZAY_GESTURE_VNTXY(v, n, t, x, y, SHHeight, i)
                    {
                        if(t == GT_InDragging || t == GT_OutDragging)
                        {
                            m->mSHScroll[i] -= (y - v->oldxy(n).y) * 10.0f / SHHeight;
                            m->mSHScrollValue[i] = Math::Clamp((sint32) (m->mSHScroll[i] + 0.5f), 0, 9);
                            v->invalidate();
                        }
                        else if(t == GT_InReleased || t == GT_OutReleased)
                            m->mSHScroll[i] = m->mSHScrollValue[i];
                    })
                {
                    ZAY_RGBA_IF(panel, 64, 0, 96, 224, WordCode[i])
                    ZAY_RGBA_IF(panel, 128, 64, 192, 128, !WordCode[i])
                        panel.fill();
                    if(WordCode[i])
                    {
                        ZAY_RGB(panel, 255, 255, 255)
                            panel.text(WordCode[i], UIFA_CenterMiddle);
                    }
                    else
                    {
                        ZAY_RGB(panel, 0, 0, 0)
                        for(sint32 y = 0; y < 10; ++y)
                        {
                            ZAY_XYWH(panel, 0, panel.h() * (y - m->mSHScrollPhy[i]), panel.w(), panel.h())
                                panel.text(String::Format("%d", y), UIFA_CenterMiddle);
                        }
                    }
                }
            }
            ZAY_RGB(panel, 0, 0, 0)
                panel.rect(1);
        }

        // 연결
        ZAY_LTRB(panel, 0, panel.h() * 0.6, panel.w(), panel.h())
        {
            ZAY_RGBA(panel, 0, 0, 0, 64)
                panel.text("www.GeeGeePms.com", UIFA_CenterBottom);
            // 선택
            ZAY_XYRR(panel, panel.w() / 2, panel.h() * 1 / 3, BaseSize / 3, BaseSize / 20)
            {
                chars UIName[2] = {"TOUCHPAD", "JOYSTICK"};
                for(sint32 i = 0; i < 2; ++i)
                {
                    ZAY_LTRB_UI(panel, panel.w() * i / 2, 0, panel.w() * (i + 1) / 2, panel.h(), UIName[i],
                        ZAY_GESTURE_T(t, i)
                        {
                            if(t == GT_Pressed)
                                m->mSelectID = i;
                        })
                    {
                        if(i == m->mSelectID)
                        {
                            ZAY_RGBA(panel, 64, 64, 64, 192)
                                panel.fill();
                        }
                        else
                        {
                            ZAY_RGBA(panel, 255, 255, 255, 192)
                                panel.fill();
                            ZAY_LTRB(panel, 1, 1, panel.w(), panel.h())
                            ZAY_RGBA(panel, 255, 128, 255, 192)
                                panel.fill();
                        }
                        ZAY_RGB_IF(panel, 255, 255, 0, i == m->mSelectID)
                        ZAY_RGB_IF(panel, 0, 0, 0, i != m->mSelectID)
                        ZAY_FONT(panel, 1.1)
                            panel.text(UIName[i], UIFA_CenterMiddle);
                    }
                }
                ZAY_RGB(panel, 0, 0, 0)
                    panel.rect(1);
            }
            // 입장
            ZAY_XYRR_UI(panel, panel.w() / 2, panel.h() * 2 / 3, BaseSize / 6, BaseSize / 20, "ENTER",
                ZAY_GESTURE_T(t)
                {
                    if(t == GT_Pressed)
                    {
                        chars SHCodeBase = "SH000-0000";
                        String SHCode;
                        for(sint32 i = 0; i < 10; ++i)
                            SHCode = SHCode + (char) (SHCodeBase[i] + m->mSHScrollValue[i]);
                        Platform::Option::SetText("SHCode", SHCode);
                        if(m->mSelectID == 0)
                            m->next("syncu_touchpadView");
                        else if(m->mSelectID == 1)
                            m->next("syncu_joystickView");
                    }
                })
            {
                ZAY_RGBA(panel, 0, 0, 128, 192)
                    panel.fill();
                ZAY_RGB(panel, 255, 255, 0)
                ZAY_FONT(panel, 1.1)
                    panel.text("ENTER", UIFA_CenterMiddle);
                ZAY_RGB(panel, 0, 0, 0)
                    panel.rect(1);
            }
        }
    }
}

syncuData::syncuData()
{
    String SHCodeJson = String::FromAsset("shcode.json");
    const Context SHCode(ST_Json, SO_OnlyReference, SHCodeJson, SHCodeJson.Length());
    mSelectID = SHCode("SH")("View").GetInt(0);
    for(sint32 i = 0; i < 10; ++i)
    {
        const sint32 Code = SHCode("SH")("Code")[i].GetInt(0);
        mSHScroll[i] = Code;
        mSHScrollPhy[i] = Code;
        mSHScrollValue[i] = Code;
    }
}

syncuData::~syncuData()
{
    Context SHCode;
    SHCode.At("SH").At("View").Set(String::FromInteger(mSelectID));
    for(sint32 i = 0; i < 10; ++i)
        SHCode.At("SH").At("Code").At(i).Set(String::FromInteger(mSHScrollValue[i]));
    SHCode.SaveJson().ToAsset("shcode.json");
}

syncuObject::syncuObject()
{
    mSocket = Platform::Socket::OpenForTcp();
    Platform::Socket::Connect(mSocket, "www.geegeepms.com", 10125);
    mSHCode = Platform::Option::GetText("SHCode");
    mLastIDX = -1;
    mEvent.At("type").Set("event_from_connecter");
    mEvent.At("id").Set(mSHCode);
    mEvent.At("idx").Set("-1");
    mEvent.At("payload").At("type").Set("pressed");
    mEvent.At("payload").At("x").Set("0");
    mEvent.At("payload").At("y").Set("0");
}

syncuObject::~syncuObject()
{
    Platform::Socket::Close(mSocket);
}

void syncuObject::RenderUI(ZayPanel& panel)
{
    const sint32 BaseSize = Math::Min(panel.w(), panel.h());

    // 마지막 메시지ID
    ZAY_RGBA(panel, 0, 0, 0, 64)
        panel.text(String::Format(" idx: %d", mLastIDX), UIFA_LeftBottom);

    // 닫기버튼
    ZAY_XYWH(panel, panel.w() - BaseSize / 10, 0, BaseSize / 10, BaseSize / 10)
    ZAY_INNER_UI(panel, BaseSize / 40, "X",
        ZAY_GESTURE_T(t, this)
        {
            if(t == GT_Pressed)
            {
                next("syncuView");
            }
        })
    {
        ZAY_RGB(panel, 255, 0, 0)
            panel.fill();
        ZAY_RGB(panel, 0, 0, 0)
            panel.text("x", UIFA_CenterMiddle);
        ZAY_RGB(panel, 0, 0, 0)
            panel.rect(1);
    }
}

void syncuObject::SendEvent()
{
    const String JsonText = mEvent.SaveJson();
    Platform::Socket::Send(mSocket, (bytes) "#json begin ", 12);
    Platform::Socket::Send(mSocket, (bytes)(chars) JsonText, JsonText.Length());
    Platform::Socket::Send(mSocket, (bytes) "#json end ", 10);
}
