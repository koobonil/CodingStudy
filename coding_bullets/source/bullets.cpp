#include <boss.hpp>
#include "bullets.hpp"

#include <resource.hpp>

ZAY_DECLARE_VIEW_CLASS("bulletsView", bulletsData)

ZAY_VIEW_API OnCommand(CommandType type, chars topic, id_share in, id_cloned_share* out)
{
	if(type == CT_Size)
	{
		sint32s WH = in;
		m->Resize(WH[0], WH[1]);
	}
	else if(type == CT_Tick)
	{
		m->CheckMessage();
		m->invalidate();

		// 비행기처리
		for(sint32 i = 0, iend = m->mAirplanes.Count(); i < iend; ++i)
		{
			auto& CurAirplane = *m->mAirplanes.AccessByOrder(i);
			CurAirplane.AnimationOnce(m->mScreenWidth, m->mScreenHeight);
			// 꼬리물기
			for(sint32 i = 0, iend = m->mFlies.Count(); i < iend; ++i)
			{
				auto& CurFly = *m->mFlies.AccessByOrder(i);
				const float CurDistance = Math::Distance(CurAirplane.mPos.x, CurAirplane.mPos.y, CurFly.mPos.x, CurFly.mPos.y);
				if(CurDistance < CurAirplane.mSizeR)
				{
					if(CurFly.mLinking) // 꼬리자르기
					{
						bool IsMine = false;
						auto CurTail = CurAirplane.mTail.mNext;
						while(CurTail)
						{
							if(CurTail == &CurFly)
							{
								IsMine = true;
								break;
							}
							CurTail = CurTail->mNext;
						}
						if(!IsMine)
						{
							// 새 파리를 상대방에게서 잘라냄
							CurFly.mPrev->mNext = nullptr;
							CurFly.mPrev = nullptr;
							// 새 파리를 현재 꼬리의 파리 끝에 붙임
							auto LastFly = &CurAirplane.mTail;
							while(LastFly->mNext) LastFly = LastFly->mNext;
							CurFly.mPrev = LastFly;
							LastFly->mNext = &CurFly;
						}
					}
					else
					{
						CurFly.mLinking = true;
						// 현재 꼬리의 파리들을 새 파리의 꼬리에 붙임
						if(CurAirplane.mTail.mNext)
						{
							CurFly.mNext = CurAirplane.mTail.mNext;
							CurFly.mNext->mPrev = &CurFly;
						}
						// 새 파리를 현재 꼬리에 붙임
						CurFly.mPrev = &CurAirplane.mTail;
						CurAirplane.mTail.mNext = &CurFly;
					}
				}
			}
			// 꼬리당기기
			auto CurPos = CurAirplane.mPos;
			auto CurTail = CurAirplane.mTail.mNext;
			while(CurTail)
			{
				if(CurPos.x < CurTail->mPos.x - m->mScreenWidth / 2) CurPos.x += m->mScreenWidth;
				if(CurPos.y < CurTail->mPos.y - m->mScreenHeight / 2) CurPos.y += m->mScreenHeight;
				if(CurTail->mPos.x + m->mScreenWidth / 2 < CurPos.x) CurPos.x -= m->mScreenWidth;
				if(CurTail->mPos.y + m->mScreenHeight / 2 < CurPos.y) CurPos.y -= m->mScreenHeight;
				const float CurDistance = Math::Distance(CurPos.x, CurPos.y, CurTail->mPos.x, CurTail->mPos.y);
				if(CurAirplane.mSizeR < CurDistance)
				{
					CurTail->mPos.x = (CurTail->mPos.x - CurPos.x) * CurAirplane.mSizeR / CurDistance + CurPos.x;
					CurTail->mPos.y = (CurTail->mPos.y - CurPos.y) * CurAirplane.mSizeR / CurDistance + CurPos.y;
				}
				CurPos = CurTail->mPos;
				CurTail = CurTail->mNext;
			}
		}

		// 파리처리
        if(m->mAddWait-- == 0)
        {
            auto& NewFly = m->mFlies[++m->mLastFlyID];
            m->mAddWait = m->OnAdd(NewFly.mPos.x, NewFly.mPos.y);
        }
        for(sint32 i = 0, iend = m->mFlies.Count(); i < iend; ++i)
        {
            auto& CurFly = *m->mFlies.AccessByOrder(i);
			if(!CurFly.mLinking)
			{
				if(CurFly.mVecCount-- == 0)
					CurFly.mVecCount = m->OnAct(CurFly.mType, CurFly.mVec.x, CurFly.mVec.y);
				CurFly.mPos += CurFly.mVec;
			}
            while(CurFly.mPos.x > m->mScreenWidth / 2) CurFly.mPos.x -= m->mScreenWidth;
            while(CurFly.mPos.y > m->mScreenHeight / 2) CurFly.mPos.y -= m->mScreenHeight;
            while(CurFly.mPos.x < -m->mScreenWidth / 2) CurFly.mPos.x += m->mScreenWidth;
            while(CurFly.mPos.y < -m->mScreenHeight / 2) CurFly.mPos.y += m->mScreenHeight;
        }
	}
	else if(type == CT_Signal)
	{
		sint32 KeyType = 0;
		branch;
		jump(!String::Compare(topic, "KeyPress")) KeyType = 1;
		jump(!String::Compare(topic, "KeyRelease")) KeyType = 2;
		if(0 < KeyType)
		{
			auto& CurAirplane = m->mAirplanes("MY");
			pointers Key(in);
			sint32 KeyCode = *((sint32*) Key[0]);
			if(KeyType == 2)
			{
				CurAirplane.mKeyPressMap.Remove(KeyCode);
				if(CurAirplane.mKeyPressMap.Count() == 0)
					CurAirplane.mDegreeCancelOnce = true;
			}
			else
			{
				switch(KeyCode)
				{
				case 'A': CurAirplane.mDegree = 315; break; // Left
				case 'W': CurAirplane.mDegree = 45; break; // Top
				case 'D': CurAirplane.mDegree = 135; break; // Right
				case 'S': CurAirplane.mDegree = 225; break; // Bottom
				}
				switch(KeyCode)
				{
				case 0x01000012: CurAirplane.mDegree = 315; break; // Left
				case 0x01000013: CurAirplane.mDegree = 45; break; // Top
				case 0x01000014: CurAirplane.mDegree = 135; break; // Right
				case 0x01000015: CurAirplane.mDegree = 225; break; // Bottom
				}
				CurAirplane.mDegreeCancelOnce = false;
				CurAirplane.mKeyPressMap[KeyCode] = KeyCode;
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
    ZAY_RGB(panel, 192, 192, 192)
        panel.fill();

	// 파리들
    for(sint32 i = 0, iend = m->mFlies.Count(); i < iend; ++i)
    {
        auto& CurFly = m->mFlies[i];
        ZAY_XYRR(panel, panel.w() / 2 + CurFly.mPos.x, panel.h() / 2 + CurFly.mPos.y, 0, 0)
            m->OnRender(panel, CurFly.mType);
    }

	// 비행기
    for(sint32 i = 0, iend = m->mAirplanes.Count(); i < iend; ++i)
    {
        chararray ID;
        auto& CurAirplane = *m->mAirplanes.AccessByOrder(i, &ID);
        ZAY_XYRR(panel, panel.w() / 2 + CurAirplane.mPos.x, panel.h() / 2 + CurAirplane.mPos.y, 0, 0)
		{
			bool IsMyTouch = !String::Compare(&ID[0], "MY");
			CurAirplane.mSizeR = m->OnRenderUser(panel, (IsMyTouch)? 0 : Parser::GetInt(&ID[1]),
				CurAirplane.mVec.x, CurAirplane.mVec.y);
		}
    }

	// UI요소
    const float BaseSize = Math::MinF(panel.w(), panel.h());
    ZAY_FONT(panel, BaseSize / 300)
    {
        // SH코드
        ZAY_FONT(panel, 1.2)
        ZAY_RGB(panel, 128, 0, 128)
        {
            if(0 < m->mSHCount)
                panel.text(String::Format("%s(%d) ", (chars) m->mSHCode, m->mSHCount), UIFA_RightTop);
            else panel.text(String::Format("%s ", (chars) m->mSHCode), UIFA_RightTop);
        }
        // 마지막 메시지ID
        ZAY_RGBA(panel, 0, 0, 0, 64)
            panel.text(String::Format(" idx: %d, sum: %d", m->mLastIDX, m->mSumIDX), UIFA_LeftBottom);
    }
}

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

bulletsData::bulletsData() : SyncuClient("bullets")
{
	mAirplanes("MY");
	mLastFlyID = -1;
	mAddWait = 0;
}

bulletsData::~bulletsData()
{
}

sint32 bulletsData::OnAdd(float& x, float& y)
{
	return Add(x, y);
}

sint32 bulletsData::OnAct(sint32& type, float& vx, float& vy)
{
	return Act(type, vx, vy);
}

void bulletsData::OnRender(ZayPanel& panel, sint32 type)
{
	Render(panel, type);
}

sint32 bulletsData::OnRenderUser(ZayPanel& panel, sint32 id, float vx, float vy)
{
	return RenderUser(panel, id, vx, vy);
}

void bulletsData::OnVisit(chars id)
{
}

void bulletsData::OnLeave(chars id)
{
	mAirplanes.Remove(id);
}

void bulletsData::OnEvent(chars id, const Context& json)
{
	auto& CurAirplane = mAirplanes(id);
	chars PayloadType = json("type").GetString();
	if(!String::Compare(PayloadType, "joystick"))
	{
		const sint32 DegreeCode = json("degree").GetInt(-2);
		if(0 <= DegreeCode)
		{
			CurAirplane.mDegree = DegreeCode;
			CurAirplane.mDegreeCancelOnce = false;
		}
		else if(DegreeCode == -1)
			CurAirplane.mDegreeCancelOnce = true;
	}
}
