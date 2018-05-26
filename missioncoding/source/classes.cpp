#include <boss.hpp>
#include "classes.hpp"

////////////////////////////////////////////////////////////////////////////////
Example::Example()
{
	mStep = nullptr;
}

Example::~Example()
{
}

int Example::SetLevel(int level)
{
	if(mStep) mStep(0);
	mStep = nullptr;
	SetLevelCore(level);
	return (mStep)? mStep(-1) : 1;
}

void Example::SetStep(int step)
{
	if(mStep) mStep(step);
}

////////////////////////////////////////////////////////////////////////////////
MissionElement::MissionElement()
{
	mExample = nullptr;
	mExampleLevel = -1;
	mExampleStep = -1;
	mExampleStepCount = -1;
}

MissionElement::~MissionElement()
{
	delete mExample;
}

void MissionElement::SetLevel(sint32 level)
{
	if(mExampleLevel != level)
	{
		mExampleLevel = level;
		mExampleStep = 0;
		mExampleStepCount = mExample->SetLevel(level);
	}
}

void MissionElement::SetStep(sint32 step)
{
	if(mExampleStep != step)
	{
		mExampleStep = step;
		mExample->SetStep(step);
	}
}

////////////////////////////////////////////////////////////////////////////////
autorun MissionCollector::AddMission(chars name, chars view, Example* example)
{
	auto& CurElement = MissionData::ST().mElements(name);
	BOSS_ASSERT("동일한 이름의 미션이 존재합니다", CurElement.mName.Length() == 0);
	CurElement.mName = name;
	CurElement.mView = view;
	CurElement.mExample = example;
	#ifdef MISSIONCODING
		CurElement.SetLevel(1);
	#else
		CurElement.SetLevel(0);
	#endif
	return true;
}

autorun MissionCollector::AddComment(chars name, sint32 level, sint32 step, chars comment)
{
	auto& CurElement = MissionData::ST().mElements(name);
	CurElement.mComments[level].AtWherever(step) = comment;
	return true;
}

MissionElement* MissionCollector::FocusedMission()
{
	const String FocusName = MissionData::ST().mFocus;
	return MissionData::ST().mElements.Access(FocusName);
}

void MissionCollector::RenderUI(ZayPanel& panel)
{
	#ifdef MISSIONCODING
		ZAY_XYWH(panel, 0, 0, 80, 30)
		ZAY_INNER_UI(panel, 3, "ui_back",
			ZAY_GESTURE_VNT(v, n, t)
			{
				if(t == GT_Pressed)
					v->next("listView");
			})
		{
			ZAY_RGBA(panel, 255, 255, 255, 32)
				panel.fill();
			ZAY_RGBA(panel, 0, 0, 0, 64)
			{
				panel.text("BACK", UIFA_CenterMiddle, UIFE_Right);
				panel.rect(1);
			}
		}
	#endif
}

////////////////////////////////////////////////////////////////////////////////
SyncuClient::SyncuClient(chars name)
{
	mSocket = Platform::Socket::OpenForTcp();
	Platform::Socket::Connect(mSocket, "www.finalbossbehindthedoor.com", 10125);

	String SHCodeJson = String::FromAsset("shcode.json");
	const Context SHCode(ST_Json, SO_OnlyReference, SHCodeJson, SHCodeJson.Length());
	mSHCode = SHCode("process_id").GetString();
	mSHPassword = SHCode("process_pw").GetString();
	mSHCount = 0;

	if (0 < mSHCode.Length())
	{
		const String SignUp = String::Format("#json begin {\"type\":\"signup\", \"app_name\":\"%s\", "
			"\"process_id\":\"%s\", \"process_pw\":\"%s\"} #json end", name, (chars) mSHCode, (chars) mSHPassword);
		Platform::Socket::Send(mSocket, (bytes)(chars) SignUp, SignUp.Length());
	}
	else
	{
		const String SignUp = String::Format("#json begin {\"type\":\"signup\", \"app_name\":\"%s\"} #json end", name);
		Platform::Socket::Send(mSocket, (bytes)(chars) SignUp, SignUp.Length());
	}

	mScreenWidth = 0;
	mScreenHeight = 0;
	mLastIDX = -1;
	mSumIDX = 0;
}

SyncuClient::~SyncuClient()
{
	Platform::Socket::Close(mSocket);

	if (0 < mSHCode.Length())
	{
		Context SHCode;
		SHCode.At("process_id").Set(mSHCode);
		SHCode.At("process_pw").Set(mSHPassword);
		SHCode.SaveJson().ToAsset("shcode.json");
	}
}

void SyncuClient::Resize(sint32 width, sint32 height)
{
	mScreenWidth = width;
	mScreenHeight = height;
}

void SyncuClient::CheckMessage()
{
	sint32 Length = Platform::Socket::RecvAvailable(mSocket, 10);
	while (0 < Length)
	{
		uint08 ReadBuffer[1024];
		const sint32 ReadLength = Platform::Socket::Recv(mSocket, ReadBuffer, Math::Min(Length, 1024));
		if (0 < ReadLength)
		{
			Length -= ReadLength;
			mSocketMessage.Add((chars) ReadBuffer, ReadLength);
			// 버퍼연결
			static const String JsonBegin("#json begin");
			static const String JsonEnd("#json end");
			sint32 BeginPos = 0, EndPos = 0;
			while ((EndPos = mSocketMessage.Find(0, JsonEnd)) != -1)
			{
				if ((BeginPos = mSocketMessage.Find(0, JsonBegin)) != -1)
				{
					const sint32 JsonPos = BeginPos + JsonBegin.Length();
					const sint32 JsonSize = EndPos - JsonPos;
					Context Json(ST_Json, SO_OnlyReference, ((chars) mSocketMessage) + JsonPos, JsonSize);
					OnMessage(Json);
					mSocketMessage = mSocketMessage.Right(mSocketMessage.Length() - (EndPos + JsonEnd.Length()));
				}
			}
		}
	}
}

void SyncuClient::RenderUI(ZayPanel& panel)
{
	const float BaseSize = Math::MinF(panel.w(), panel.h());
    ZAY_FONT(panel, BaseSize / 300)
    {
        // SH코드
        ZAY_FONT(panel, 1.2)
        ZAY_RGB(panel, 128, 0, 128)
        {
            if(0 < mSHCount)
                panel.text(String::Format("%s(%d) ", (chars) mSHCode, mSHCount), UIFA_RightTop);
            else panel.text(String::Format("%s ", (chars) mSHCode), UIFA_RightTop);
        }
        // 마지막 메시지ID
        ZAY_RGBA(panel, 0, 0, 0, 64)
            panel.text(String::Format(" idx: %d, sum: %d", mLastIDX, mSumIDX), UIFA_LeftBottom);
    }
}

void SyncuClient::OnMessage(Context& json)
{
	chars Type = json("type").GetString();
	mLastIDX = json("idx").GetInt(-1);
	mSumIDX++;

	if (!String::Compare(Type, "signin"))
	{
		mSHCode = json("process_id").GetString();
		mSHPassword = json("process_pw").GetString();
	}
	else if (!String::Compare(Type, "visit"))
	{
		mSHCount++;
		OnLeave(json("id").GetString());
	}
	else if (!String::Compare(Type, "leave"))
	{
		mSHCount--;
		OnLeave(json("id").GetString());
	}
	else if (!String::Compare(Type, "event"))
		OnEvent(json("id").GetString(), json("payload"));
}
