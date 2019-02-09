#include <boss.hpp>
#include "splitter.hpp"

#include <resource.hpp>

ZAY_DECLARE_VIEW_CLASS("splitterView", splitterData)

ZAY_VIEW_API OnCommand(CommandType type, chars topic, id_share in, id_cloned_share* out)
{
    if(type == CT_Size)
    {
        m->mPeerView.OnSizing();
        m->mLogView.OnSizing();
        m->invalidate();
    }
    else if(type == CT_Tick)
    {
        // 패킷처리
        while(Platform::Server::TryNextPacket(Global::server()))
        {
            const sint32 PeerID = Platform::Server::GetPacketPeerID(Global::server());
            switch(Platform::Server::GetPacketType(Global::server()))
            {
            case packettype_entrance:
                m->mLogView.AddElementForMax(Log::Create(String::Format("  ※ Peer(%d) visited", PeerID)), Log::Max);
                m->mLogView.OnScrollBottom();
                m->mPeerView.AddElement(Peer::Create(PeerID));
                break;
            case packettype_message:
                if(auto CurElement = m->mPeerView.GetElement(Peer::ToKey(PeerID)))
                {
                    sint32 GetSize = 0;
                    bytes Data = Platform::Server::GetPacketBuffer(Global::server(), &GetSize);
                    if(CurElement->AddMessage(Data, GetSize))
                        m->invalidate();
                }
                break;
            case packettype_leaved:
                m->mPeerView.SubElement(Peer::ToKey(PeerID));
                m->mLogView.AddElementForMax(Log::Create(String::Format("  ※ Peer(%d) leaved", PeerID)), Log::Max);
                m->mLogView.OnScrollBottom();
                break;
            case packettype_kicked:
                m->mPeerView.SubElement(Peer::ToKey(PeerID));
                m->mLogView.AddElementForMax(Log::Create(String::Format("  ※ Peer(%d) kicked", PeerID)), Log::Max);
                m->mLogView.OnScrollBottom();
                break;
            }
            m->invalidate();
        }
    }
}

ZAY_VIEW_API OnNotify(NotifyType type, chars topic, id_share in, id_cloned_share* out)
{
    if(!String::Compare(topic, "SendLog:", 8))
    {
        const String TypeCode(in);
        m->mLogView.AddElementForMax(Log::Create(String::Format("  [%s] ◁◁◁ %s", topic + 8, (chars) TypeCode)), Log::Max);
        m->mLogView.OnScrollBottom();
        m->invalidate();
    }
    else if(!String::Compare(topic, "RecvLog:", 8))
    {
        const String TypeCode(in);
        m->mLogView.AddElementForMax(Log::Create(String::Format("  [%s] ▶▶▶ %s", topic + 8, (chars) TypeCode)), Log::Max);
        m->mLogView.OnScrollBottom();
        m->invalidate();
    }
}

ZAY_VIEW_API OnGesture(GestureType type, sint32 x, sint32 y)
{
    if(type == GT_WheelUp || type == GT_WheelUpPeeked)
    {
        if(Rect(m->rect("PeerView")).PtInRect(x, y))
            m->mPeerView.OnWheelUp();
        else if(Rect(m->rect("LogView")).PtInRect(x, y))
            m->mLogView.OnWheelUp();
        m->invalidate();
    }
    else if(type == GT_WheelDown || type == GT_WheelDownPeeked)
    {
        if(Rect(m->rect("PeerView")).PtInRect(x, y))
            m->mPeerView.OnWheelDown();
        else if(Rect(m->rect("LogView")).PtInRect(x, y))
            m->mLogView.OnWheelDown();
        m->invalidate();
    }
}

ZAY_VIEW_API OnRender(ZayPanel& panel)
{
    ZAY_RGB(panel, 128, 160, 192)
        panel.fill();

    // 연결된 피어
    ZAY_LTRB_UI_SCISSOR(panel, 10, 10, panel.w() - 10, panel.h() * 0.5 - 5, "PeerView")
    {
        ZAY_RGB(panel, 224, 224, 224)
            panel.fill();
        m->mPeerView.Render(panel);
        ZAY_INNER(panel, 2)
        ZAY_RGB(panel, 0, 0, 0)
            panel.rect(2);
    }

    // 로그
    ZAY_LTRB_UI_SCISSOR(panel, 10, panel.h() * 0.5 + 5, panel.w() - 10, panel.h() - 10, "LogView")
    {
        ZAY_RGB(panel, 192, 192, 216)
            panel.fill();
        m->mLogView.Render(panel);
        ZAY_INNER(panel, 1)
        ZAY_RGB(panel, 0, 0, 0)
            panel.rect(1);
    }
}

splitterData::splitterData() : mPeerView("PeerView", updater()), mLogView("LogView", updater())
{
}

splitterData::~splitterData()
{
}
