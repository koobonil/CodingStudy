#pragma once
#include <service/boss_zay.hpp>
#include <element/boss_tween.hpp>

////////////////////////////////////////////////////////////////////////////////
// Global
////////////////////////////////////////////////////////////////////////////////
class Global
{
    BOSS_DECLARE_NONCOPYABLE_CLASS(Global)
private:
    Global()
    {
        mServer = Platform::Server::Create(false);
        Platform::Server::Listen(mServer, 10125);
    }
    ~Global()
    {
        Platform::Server::Release(mServer);
    }

private:
    inline static Global& ST()
    {static Global _; return _;}

public:
    inline static id_server server()
    {return ST().mServer;}

public:
    static void Send(sint32 peerid, const Context& context)
    {
        Platform::BroadcastNotify(String::Format("SendLog:%d", peerid),
            String::Format("%s, %d", context("type").GetString(), context("idx").GetInt(-1)));
        const String JsonText = context.SaveJson();
        Platform::Server::SendToPeer(ST().mServer, peerid, "#json begin ", 12);
        Platform::Server::SendToPeer(ST().mServer, peerid, (chars) JsonText, JsonText.Length());
        Platform::Server::SendToPeer(ST().mServer, peerid, " #json end", 10);
    }
    static void CreateProcess(chars appname, String& process_id, String& process_pw, sint32 process_peerid)
    {
        // 코드제작
        sint32 SHCode = 125;
        for(chars a = appname; *a; ++a)
            SHCode += (*a & 0xFF);
        SHCode %= 1000;

        // 인덱스제작
        sint32 SHIndex = 0;
        while(auto OldSHIndex = ST().mProcessLastIndex.Access(SHCode))
        {
            if(*OldSHIndex < 9999)
            {
                SHIndex = *OldSHIndex + 1;
                break;
            }
            else SHCode = (SHCode + 1) % 1000;
        }
        ST().mProcessLastIndex[SHCode] = SHIndex;

        // 키생성
        process_id = String::Format("SH%03d-%04d", SHCode, SHIndex);
        process_pw = String::Format("%06d", Platform::Utility::Random() % 1000000);

        // 등록
        auto& NewProcess = ST().mProcessMap(process_id);
        NewProcess.mAppName = appname;
        NewProcess.mPassword = process_pw;
        NewProcess.mProcessPeerID = process_peerid;
    }
    static chars LinkProcessAndGetPassword(chars process_id, sint32 process_peerid)
    {
        if(auto CurProcess = ST().mProcessMap.Access(process_id))
        {
            CurProcess->mProcessPeerID = process_peerid;
            return CurProcess->mPassword;
        }
        return nullptr;
    }
    static bool BindConnecterPeerID(chars process_id, sint32 connecter_peerid)
    {
        if(auto CurProcess = ST().mProcessMap.Access(process_id))
        {
            CurProcess->mConnecterPeerIDs.AtAdding() = connecter_peerid;
            return true;
        }
        return false;
    }
    static bool UnbindConnecterPeerID(chars process_id, sint32 connecter_peerid)
    {
        if(auto CurProcess = ST().mProcessMap.Access(process_id))
        {
            for(sint32 i = 0, iend = CurProcess->mConnecterPeerIDs.Count(); i < iend; ++i)
            {
                if(CurProcess->mConnecterPeerIDs[i] == connecter_peerid)
                {
                    CurProcess->mConnecterPeerIDs.SubtractionSection(i);
                    return true;
                }
            }
        }
        return false;
    }
    static sint32 GetProcessPeerID(chars process_id)
    {
        if(auto CurProcess = ST().mProcessMap.Access(process_id))
            return CurProcess->mProcessPeerID;
        return -1;
    }
    static const sint32s* GetConnecterPeerIDs(chars process_id)
    {
        if(auto CurProcess = ST().mProcessMap.Access(process_id))
            return &CurProcess->mConnecterPeerIDs;
        return nullptr;
    }

private:
    struct Process
    {
        String mAppName;
        String mPassword;
        sint32 mProcessPeerID;
        sint32s mConnecterPeerIDs;
    };
private:
    id_server mServer;
    Map<Process> mProcessMap;
    Map<sint32> mProcessLastIndex;
};

////////////////////////////////////////////////////////////////////////////////
// Role
////////////////////////////////////////////////////////////////////////////////
class Role
{
protected:
    Role() {}
    ~Role() {}

public:
    virtual void DestroyMe() = 0;
    virtual String GetStatus() const = 0;
    virtual bool OnMessage(chars type, Context& json, sint32 peerid) = 0;
};

class RoleProcess : public Role
{
private:
    RoleProcess() {}
    ~RoleProcess() {}

public:
    static Role* Create()
    {
        return new RoleProcess();
    }

private:
    void DestroyMe() override
    {
        delete this;
    }
    String GetStatus() const override
    {
        auto PeerIDs = Global::GetConnecterPeerIDs(mProcessID);
        return String::Format("Process: %s (%d)", (chars) mProcessID, (PeerIDs)? PeerIDs->Count() : 0);
    }
    bool OnMessage(chars type, Context& json, sint32 peerid) override
    {
        if(!String::Compare(type, "signup"))
        {
            if(chars AppName = json("app_name").GetString(nullptr))
            {
                mProcessID = json("process_id").GetString();
                String ProcessPW = json("process_pw").GetString();
                // 매칭연결
                if(0 < mProcessID.Length())
                {
                    chars OldProcessPW = Global::LinkProcessAndGetPassword(mProcessID, peerid);
                    if(!OldProcessPW || !!ProcessPW.Compare(OldProcessPW))
                        mProcessID = "";
                }
                // 신규발급
                if(mProcessID.Length() == 0)
                    Global::CreateProcess(AppName, mProcessID, ProcessPW, peerid);
                // 전송
                Context Json;
                Json.At("type").Set("signin");
                Json.At("process_id").Set(mProcessID);
                Json.At("process_pw").Set(ProcessPW);
                Global::Send(peerid, Json);
                return true;
            }
        }
        else if(!String::Compare(type, "delivery"))
        {
            if(0 < mProcessID.Length())
            if(chars ConnecterPeerID = json("id").GetString(nullptr))
            {
                // 개별이벤트
                if(!!String::Compare(ConnecterPeerID, "ALL"))
                {
                    json.At("type").Set("delivery_to_connecter");
                    json.At("id").Set(mProcessID);
                    const sint32 PeerID = Parser::GetInt(&ConnecterPeerID[1]);
                    Global::Send(PeerID, json);
                }
                // 전체이벤트
                else if(auto PeerIDs = Global::GetConnecterPeerIDs(mProcessID))
                {
                    json.At("type").Set("delivery_to_connecter");
                    json.At("id").Set(mProcessID);
                    for(sint32 i = 0, iend = PeerIDs->Count(); i < iend; ++i)
                        Global::Send((*PeerIDs)[i], json);
                }
            }
        }
        return false;
    }

private:
    String mProcessID;
};

class RoleConnecter : public Role
{
private:
    RoleConnecter()
    {
        mBinded = false;
        mProcessPeerID = -1;
        mConnecterPeerID = -1;
    }
    ~RoleConnecter() {TryLeave();}

public:
    static Role* Create()
    {
        return new RoleConnecter();
    }

private:
    void DestroyMe() override
    {
        delete this;
    }
    String GetStatus() const override
    {
        return String::Format("%d/Connecter: %s", mProcessPeerID, (chars) mConnecterID);
    }
    bool OnMessage(chars type, Context& json, sint32 peerid) override
    {
        if(!String::Compare(type, "event_from_connecter"))
        {
            if(chars ProcessID = json("id").GetString(nullptr))
            {
                // 정보업데이트
                if(mProcessPeerID == -1 || !!String::Compare(mProcessID, ProcessID))
                {
                    // 접속지변경
                    TryLeave();
                    mProcessID = ProcessID;
                    mProcessPeerID = Global::GetProcessPeerID(ProcessID);
                    mConnecterID = String::Format("P%d", peerid);
                    mConnecterPeerID = peerid;
                }
                // 패킷전달
                if(mProcessPeerID != -1)
                {
                    // 신규피어 알림
                    if(!mBinded && (mBinded = Global::BindConnecterPeerID(ProcessID, peerid)))
                    {
                        Context Json;
                        Json.At("type").Set("visit");
                        Json.At("id").Set(mConnecterID);
                        Global::Send(mProcessPeerID, Json);
                    }
                    // 이벤트 전달
                    if(mBinded)
                    {
                        json.At("type").Set("event");
                        json.At("id").Set(mConnecterID);
                        Global::Send(mProcessPeerID, json);
                    }
                    return true;
                }
            }
        }
        return false;
    }

private:
    void TryLeave()
    {
        if(mProcessPeerID != -1)
        {
            Global::UnbindConnecterPeerID(mProcessID, mConnecterPeerID);
            Context Json;
            Json.At("type").Set("leave");
            Json.At("id").Set(mConnecterID);
            Global::Send(mProcessPeerID, Json);
        }
    }

private:
    bool mBinded;
    String mProcessID;
    sint32 mProcessPeerID;
    String mConnecterID;
    sint32 mConnecterPeerID;
};

class RoleChatting : public Role
{
    typedef Map<RoleChatting*> Room;
    static Tree<Room>& AllRoom()
    {static Tree<Room> _; return _;}

    static void AddPeer(chars room, RoleChatting* peer)
    {
        auto CurRoom = AllRoom()(room).Value();
        if(!CurRoom) // 없으면 트리생성
            CurRoom = AllRoom()(room).CreateValue();
        CurRoom->AtAdding() = peer;
    }
    static void SubPeer(chars room, RoleChatting* peer)
    {
        if(auto CurRoom = AllRoom()(room).Value())
        {
            // 해당 피어를 비운뒤
            for(sint32 i = 0, iend = CurRoom->Count(); i < iend; ++i)
                if(*CurRoom->Access(i) == peer)
                {
                    *CurRoom->Access(i) = nullptr;
                    break;
                }
            // 후위부터 맵정리
            for(sint32 i = CurRoom->Count() - 1; 0 <= i; --i)
            {
                if(*CurRoom->Access(i) != nullptr)
                    break;
                CurRoom->Remove(i);
            }
            // 트리정리
            if(CurRoom->Count() == 0)
                AllRoom().Remove(room);
        }
    }
    static void SendToPeers(chars room, const Context& msg)
    {
        if(auto CurRoom = AllRoom()(room).Value())
        {
            for(sint32 i = 0, iend = CurRoom->Count(); i < iend; ++i)
            {
                if(auto CurPeer = *CurRoom->Access(i))
                    Global::Send(CurPeer->mConnecterPeerID, msg);
            }
        }
    }

private:
    RoleChatting()
    {
        mConnecterPeerID = -1;
    }
    ~RoleChatting() {TryLeave();}

public:
    static Role* Create()
    {
        return new RoleChatting();
    }

private:
    void DestroyMe() override
    {
        delete this;
    }
    String GetStatus() const override
    {
        return String::Format("%s/Chatting: %s", (chars) mRoomID, "noname");
    }
    bool OnMessage(chars type, Context& json, sint32 peerid) override
    {
        if(!String::Compare(type, "chat"))
        {
            String NewRoomID = json("room").GetString("global");
            if(!!mRoomID.Compare(NewRoomID))
            {
                SubPeer(mRoomID, this);
                AddPeer(mRoomID = NewRoomID, this);
            }
            mConnecterPeerID = peerid;
            SendToPeers(mRoomID, json);
            return true;
        }
        return false;
    }

private:
    void TryLeave()
    {
        SubPeer(mRoomID, this);
    }

private:
    String mRoomID;
    sint32 mConnecterPeerID;
};

////////////////////////////////////////////////////////////////////////////////
// Scroll
////////////////////////////////////////////////////////////////////////////////
class ScrollElement
{
    BOSS_DECLARE_NONCOPYABLE_CLASS(ScrollElement)
public:
    ScrollElement() {}
    ~ScrollElement() {}
public:
    virtual void Render(ZayPanel& panel, chars name) const {}
    virtual bool IsSame(const void* key) const {return false;}
    virtual bool AddMessage(bytes data, sint32 size) {return false;}
    virtual bool OnNotify(chars topic) {return false;}
};
typedef Object<ScrollElement> ScrollObject;

class ScrollView
{
    BOSS_DECLARE_NONCOPYABLE_CLASS(ScrollView)
public:
    ScrollView(chars name, Updater* updater) : mName(name), mScroll(updater)
    {
        mScroll.Reset(0);
        mCommand = 0;
        mCursor = 0;
    }
    ~ScrollView() {}

public:
    void AddElement(buffer element)
    {
        mElements.AtAdding() = ScrollObject(element);
    }

    void AddElementForMax(buffer element, sint32 max)
    {
        if(max < mElements.Count())
            mElements.SubtractionSection(0, mElements.Count() - max);
        mElements.AtAdding() = ScrollObject(element);
    }

    bool SubElement(const void* key)
    {
        for(sint32 i = 0; i < mElements.Count(); ++i)
            if(mElements[i]->IsSame(key))
            {
                mElements.SubtractionSection(i);
                mCommand = 0.1f;
                mCursor = Math::Min(mCursor, mElements.Count() - 1);
                return true;
            }
        return false;
    }

    ScrollElement* GetElement(const void* key)
    {
        for(sint32 i = 0; i < mElements.Count(); ++i)
            if(mElements[i]->IsSame(key))
                return mElements.At(i).Ptr();
        return nullptr;
    }

    void Render(ZayPanel& panel)
    {
        const float ScrollHeight = panel.h();
        if(mCommand != 0)
        {
            Update(ScrollHeight, mCommand);
            mCommand = 0;
        }

        const float ScrollPos = mScroll.value();        
        for(sint32 i = 0, iend = mElements.Count(); i < iend; ++i)
        {
            const String ElementName = String::Format("%s-%d", (chars) mName, i);
            ZAY_XYWH_UI(panel, 0, IconSize * (i - ScrollPos), panel.w(), IconSize, ElementName,
                ZAY_GESTURE_VNT(v, n, t, this, i)
                {
                    if(t == GT_InReleased)
                    {
                        mCommand = 0.5f;
                        mCursor = i;
                    }
                })
            {
                ZAY_RGB_IF(panel, 192, 192, 255, i == mCursor)
                ZAY_RGB_IF(panel, 255, 255, 255, i != mCursor)
                    panel.fill();
                mElements[i]->Render(panel, ElementName);
                ZAY_RGB(panel, 0, 0, 0)
                    panel.rect(1);
            }
        }
    }

public:
    void OnSizing()
    {
        mCommand = 0.1f;
    }

    void OnWheelUp()
    {
        mCommand = 0.3f;
        mCursor = Math::Max(0, mCursor - 1);
    }

    void OnWheelDown()
    {
        mCommand = 0.3f;
        mCursor = Math::Min(mCursor + 1, mElements.Count() - 1);
    }

    void OnScrollTop()
    {
        mCommand = 0.3f;
        mCursor = 0;
    }

    void OnScrollBottom()
    {
        mCommand = 0.3f;
        mCursor = mElements.Count() - 1;
    }

private:
    void Update(sint32 height, float sec)
    {
        const float ScrollMid = (height - IconSize) / 2;
        const float ScrollEnd = IconSize * mElements.Count() - height;
        mScroll.MoveTo(Math::ClampF(mCursor * IconSize - ScrollMid, 0, ScrollEnd) / IconSize, sec);
    }

private:
    const sint32 InnerGap = 10;
    const sint32 ButtonSize = 80;
    const sint32 ButtonSizeSmall = 50;
    const sint32 IconSize = 40;

private:
    const String mName;
    Tween1D mScroll;
    float mCommand;
    sint32 mCursor;
    Array<ScrollObject> mElements;
};

////////////////////////////////////////////////////////////////////////////////
// Elements
////////////////////////////////////////////////////////////////////////////////
class Peer : public ScrollElement
{
    BOSS_DECLARE_NONCOPYABLE_CLASS(Peer)
public:
    Peer()
    {
        mPeerID = -1;
        mPeerRole = nullptr;
    }
    ~Peer()
    {
        if(mPeerRole)
            mPeerRole->DestroyMe();
    }
    Peer(Peer&& rhs) {operator=(ToReference(rhs));}
    Peer& operator=(Peer&& rhs)
    {
        mPeerID = rhs.mPeerID; rhs.mPeerID = -1;
        mPeerRole = rhs.mPeerRole; rhs.mPeerRole = nullptr;
        mMessage = ToReference(rhs.mMessage);
        return *this;
    }

public:
    static buffer Create(sint32 id)
    {
        buffer NewBuffer = Buffer::Alloc<Peer>(BOSS_DBG 1);
        ((Peer*) NewBuffer)->mPeerID = id;
        return NewBuffer;
    }

    static const void* ToKey(sint32 id)
    {
        static sint32 ID = 0;
        ID = id;
        return (const void*) &ID;
    }

public:
    void Render(ZayPanel& panel, chars name) const override
    {
        // 정보
        ZAY_RGB(panel, 128, 0, 0)
        {
            const String CurStatus = (mPeerRole)? mPeerRole->GetStatus() : String("Unknown");
            panel.text(String::Format("  [%d] %s", mPeerID, (chars) CurStatus), UIFA_LeftMiddle);
        }

        // 종료버튼
        const sint32 BaseSize = panel.h();
        const String XName = String::Format("%s-x", name);
        ZAY_XYWH(panel, panel.w() - BaseSize, 0, BaseSize, BaseSize)
        ZAY_INNER_UI(panel, BaseSize / 3, XName,
            ZAY_GESTURE_T(t, this)
            {
                if(t == GT_InReleased)
                    Platform::Server::KickPeer(Global::server(), mPeerID);
            })
        {
            ZAY_RGB(panel, 255, 0, 0)
            ZAY_RGB_IF(panel, 64, 64, 64, panel.state(XName) & PS_Pressed)
                panel.fill();
            ZAY_RGB(panel, 0, 0, 0)
                panel.rect(1);
        }
    }

    bool IsSame(const void* key) const override
    {
        return (*((const sint32*) key) == mPeerID);
    }

    bool AddMessage(bytes data, sint32 size) override
    {
        bool UpdateScreen = false;
        mMessage.Add((chars) data, size);
        static const String JsonBegin("#json begin");
        static const String JsonEnd("#json end");
        sint32 BeginPos = 0, EndPos = 0;
        while((EndPos = mMessage.Find(0, JsonEnd)) != -1)
        {
            if((BeginPos = mMessage.Find(0, JsonBegin)) != -1)
            {
                const sint32 JsonPos = BeginPos + JsonBegin.Length();
                const sint32 JsonSize = EndPos - JsonPos;
                Context Json(ST_Json, SO_OnlyReference, ((chars) mMessage) + JsonPos, JsonSize);
                Platform::BroadcastNotify(String::Format("RecvLog:%d", mPeerID),
                    String::Format("%s, %d", Json("type").GetString(), Json("idx").GetInt(-1)));
                UpdateScreen |= OnMessage(Json);
                mMessage = mMessage.Right(mMessage.Length() - (EndPos + JsonEnd.Length()));
            }
            else
            {
                Platform::Server::KickPeer(Global::server(), mPeerID);
                break;
            }
        }
        return UpdateScreen;
    }

private:
    bool OnMessage(Context& json)
    {
        chars Type = json("type").GetString();
        if(!mPeerRole)
        {
            if(!String::Compare(Type, "signup"))
                mPeerRole = RoleProcess::Create();
            else if(!String::Compare(Type, "event_from_connecter"))
                mPeerRole = RoleConnecter::Create();
            else if(!String::Compare(Type, "chat"))
                mPeerRole = RoleChatting::Create();
        }
        if(mPeerRole)
            return mPeerRole->OnMessage(Type, json, mPeerID);
        return false;
    }

private:
    sint32 mPeerID;
    Role* mPeerRole;
    String mMessage;
};

class Log : public ScrollElement
{
    BOSS_DECLARE_NONCOPYABLE_CLASS(Log)
public:
    Log() {}
    ~Log() {}
    Log(Log&& rhs) {operator=(ToReference(rhs));}
    Log& operator=(Log&& rhs)
    {
        mText = ToReference(rhs.mText);
        return *this;
    }

public:
    static const sint32 Max = 64;

public:
    static buffer Create(chars text)
    {
        buffer NewBuffer = Buffer::Alloc<Log>(BOSS_DBG 1);
        ((Log*) NewBuffer)->mText = text;
        return NewBuffer;
    }

public:
    void Render(ZayPanel& panel, chars name) const override
    {
        ZAY_RGB(panel, 0, 0, 0)
            panel.text(mText, UIFA_LeftMiddle);
    }

private:
    String mText;
};

////////////////////////////////////////////////////////////////////////////////
// splitterData
////////////////////////////////////////////////////////////////////////////////
class splitterData : public ZayObject
{
public:
    splitterData();
    ~splitterData();

public:
    ScrollView mPeerView;
    ScrollView mLogView;
};
