#include <boss.hpp>
#include "clouds.hpp"
MISSION_VIEW_DECLARE(MISSION_NAME, "cloudsView", CloudsExample)

#include <resource.hpp>
ZAY_DECLARE_VIEW_CLASS("cloudsView", cloudsData)

ZAY_VIEW_API OnCommand(CommandType type, chars topic, id_share in, id_cloned_share* out)
{
	if(type == CT_Size)
	{
		sint32s WH = in;
		m->Resize(WH[0], WH[1]);
		if(m->mActorReady)
			m->SetReady();
	}
	else if(type == CT_Tick)
	{
		m->CheckMessage();
		m->invalidate();

		if(!m->mGameOver)
		{
			if(m->mActorReady)
				m->SetReady();
			else m->mActorPos += m->mActorVec;

			// 잡은 구름의 상대위치
			Point GrabbedCloud;
			float GrabbedTime = 0;
			if(m->mGrabbedCloudID != -1)
			{
				const Point CloudPos = m->mClouds[m->mGrabbedCloudID].Center();
				GrabbedCloud.x = CloudPos.x - m->mActorPos.x;
				GrabbedCloud.y = CloudPos.y - m->mActorPos.y;
				GrabbedTime = (Platform::Utility::CurrentTimeMsec() - m->mGrabbedMsec) / 1000.0;
			}

			// 스크롤링
			m->mScrollTarget += m->mScreenWidth * exam->mTickScroll(m->mActorPos, m->mActorVec,
				(m->mGrabbedCloudID != -1)? &GrabbedCloud : nullptr, GrabbedTime);
			m->mScrollTarget = Math::MaxF(m->mScrollTarget, m->mActorPos.x - m->mScreenWidth * 3 / 4);
			const float OldScrollPos = m->mScrollPos;
			m->mScrollPos = m->mScrollPos * 0.97 + m->mScrollTarget * 0.03;

			// 구름준비
			while(m->mScrollSize - m->mScrollPos < m->mScreenWidth)
			{
				float GetSkyWidth = 1;
				Rect CloudRect = exam->mAddCloud(GetSkyWidth);
				const float NewCloudSize = m->mScreenWidth * GetSkyWidth;
				Rect& NewCloudRect = m->mClouds.AtAdding();
				NewCloudRect.l = m->mScrollSize + NewCloudSize * CloudRect.l;
				NewCloudRect.t = m->mScreenHeight * CloudRect.t;
				NewCloudRect.r = m->mScrollSize + NewCloudSize * CloudRect.r;
				NewCloudRect.b = m->mScreenHeight * CloudRect.b;
				m->mScrollSize += NewCloudSize;
			}

			// 죽었는지 조사
			m->mGameOver = (m->mGrabbedCloudID == -1 && m->mScreenHeight * 2 < m->mActorPos.y);
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
			/*auto& CurAirplane = m->mAirplanes("MY");
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
			}*/
		}
	}
}

ZAY_VIEW_API OnNotify(chars sender, chars topic, id_share in, id_cloned_share* out)
{
}

ZAY_VIEW_API OnGesture(GestureType type, sint32 x, sint32 y)
{
	if(type == GT_Pressed)
	{
		if(m->mGameOver)
			m->Reset();
	}
}

ZAY_VIEW_API OnRender(ZayPanel& panel)
{
    ZAY_RGB(panel, 0, 255, 255)
	ZAY_RGB_IF(panel, 32, 32, 32, m->mGameOver)
        panel.fill();

	// 구름들
	for(sint32 i = 0, iend = m->mClouds.Count(); i < iend; ++i)
	{
		const Rect& CurCloud = m->mClouds[i];
		const String UIName = String::Format("cloud_%d", i);
		ZAY_RECT_UI(panel, CurCloud - Point(m->mScrollPos, 0), UIName,
			ZAY_GESTURE_T(t, i)
			{
				if(t == GT_Pressed)
				{
					m->mActorReady = false;
					m->mGrabbedCloudID = i;
					m->mGrabbedMsec = Platform::Utility::CurrentTimeMsec();
				}
				else if(t == GT_InReleased || t == GT_OutReleased)
					m->mGrabbedCloudID = -1;
			})
		ZAY_RGBA(panel, 255, 255, 255, 128)
		ZAY_RGBA_IF(panel, 255, 255, 0, 128, i == m->mGrabbedCloudID)
		{
			if(exam->mRenderCloud)
				exam->mRenderCloud(panel);
			else ZAY_RGBA(panel, 128, 128, 128, 192)
				panel.circle();
		}
	}

	ZAY_XYRR(panel, m->mActorPos.x - m->mScrollPos, m->mActorPos.y, 30, 30)
	{
		// 시작지점
		if(m->mActorReady)
		{
			ZAY_LTRB(panel, -20, panel.h() / 2 - 10, panel.w() + 20, panel.h() + 40)
			ZAY_RGBA(panel, 255, 255, 0, 128)
			{
				if(exam->mRenderCloud)
					exam->mRenderCloud(panel);
				else ZAY_RGBA(panel, 128, 128, 128, 192)
					panel.circle();
			}
		}
		// 캐릭터
		if(exam->mRenderActor)
		{
			Point GrabbedCloud;
			if(m->mGrabbedCloudID != -1)
			{
				const Point CloudPos = m->mClouds[m->mGrabbedCloudID].Center();
				GrabbedCloud.x = CloudPos.x - m->mActorPos.x;
				GrabbedCloud.y = CloudPos.y - m->mActorPos.y;
			}
			exam->mRenderActor(panel, (m->mGrabbedCloudID != -1)? &GrabbedCloud : nullptr);
		}
		else
		{
			ZAY_RGB(panel, 0, 255, 0)
				panel.fill();
			ZAY_RGB(panel, 0, 0, 0)
			ZAY_RGB_IF(panel, 255, 255, 0, m->mGrabbedCloudID != -1)
				panel.rect(1);
			// 벡터표시
			ZAY_RGB(panel, 255, 0, 0)
			{
				const Point MidPos = Point(panel.w() / 2, panel.h() / 2);
				const Point VecPos = MidPos + m->mActorVec;
				panel.line(MidPos, VecPos, 1);
				ZAY_XYRR(panel, VecPos.x, VecPos.y, 4, 4)
					panel.fill();
			}
		}
	}

	// UI요소
    const float BaseSize = Math::MinF(panel.w(), panel.h());
    ZAY_FONT(panel, BaseSize / 300)
    {
        // 게임점수
        ZAY_FONT(panel, 2)
        ZAY_RGB(panel, 255, 0, 0)
            panel.text(String::Format(" %.1fM", (m->mActorPos.x - m->mActorBegin.x) / 10.0f), UIFA_LeftTop);
    }

	//m->RenderUI(panel);
	MissionCollector::RenderUI(panel);
}

cloudsData::cloudsData() : SyncuClient("clouds")
{
	Reset();
}

cloudsData::~cloudsData()
{
}

void cloudsData::Reset()
{
	mScrollPos = 0;
	mScrollSize = 0;
	mScrollTarget = 0;
	mClouds.Clear();
	mActorBegin = Point(0, 0);
	mActorPos = Point(0, 0);
	mActorVec = Point(0, 0);
	mActorReady = true;
	mGrabbedCloudID = -1;
	mGrabbedMsec = 0;
	mGameOver = false;
}

void cloudsData::SetReady()
{
	mActorBegin.x = mScrollPos + mScreenWidth / 2;
	mActorBegin.y = mScreenHeight * 3 / 4;
	mActorPos = mActorBegin;
	mActorVec = Point(0, 0);
}

void cloudsData::OnVisit(chars id)
{
}

void cloudsData::OnLeave(chars id)
{
	//////////////////////////////////
}

void cloudsData::OnEvent(chars id, const Context& json)
{
	//auto& CurAirplane = mAirplanes(id);
	chars PayloadType = json("type").GetString();
	if(!String::Compare(PayloadType, "joystick"))
	{
		const sint32 DegreeCode = json("degree").GetInt(-2);
		/*if(0 <= DegreeCode)
		{
			CurAirplane.mDegree = DegreeCode;
			CurAirplane.mDegreeCancelOnce = false;
		}
		else if(DegreeCode == -1)
			CurAirplane.mDegreeCancelOnce = true;*/
	}
}
