#include <boss.hpp>
#include "bullets.hpp"
MISSION_VIEW_DECLARE(MISSION_NAME, "bulletsView", BulletsExample)

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
			CurAirplane.AnimationOnce(m->mGameType, m->mScreenWidth, m->mScreenHeight);

			// 접촉처리
            bool HasGravity = false;
            sint32 NextLoopBegin = CurAirplane.mLoopBegin;
            for(sint32 j = 0, jend = m->mFlies.Count(); j < jend; ++j)
			{
                const sint32 CurIndex = (j + CurAirplane.mLoopBegin) % jend;
                if(CurIndex == m->mPressedBulletFly)
                    continue;

				auto& CurFly = m->mFlies[CurIndex];
				const float CurDistance = Math::Distance(CurAirplane.mPos.x, CurAirplane.mPos.y, CurFly.mPos.x, CurFly.mPos.y);
				if(CurDistance < CurAirplane.mSizeR + CurFly.mSizeR)
				{
                    switch(exam->mEff(CurFly.mType))
                    {
                    case Eff::GRAVITY:
                        if(!HasGravity)
                        {
                            HasGravity = true;
                            if(CurAirplane.mLoopLockID != CurIndex)
                            {
                                NextLoopBegin = CurIndex;
                                CurAirplane.mLoopLockID = CurIndex;
                                CurAirplane.mLoopLockCount = 10;
                            }
                            else if(--CurAirplane.mLoopLockCount <= 0)
                                NextLoopBegin = CurIndex + 1;

                            if(const float OldForce = Math::Sqrt(CurAirplane.mVec.x * CurAirplane.mVec.x + CurAirplane.mVec.y * CurAirplane.mVec.y))
                            {
                                const float Rad = Math::Atan(CurAirplane.mPos.x - CurFly.mPos.x, CurAirplane.mPos.y - CurFly.mPos.y);
                                const float NewOX = CurFly.mSizeR * Math::Cos(Rad);
                                const float NewOY = CurFly.mSizeR * Math::Sin(Rad);
                                CurAirplane.mVec.x = -NewOY;
                                CurAirplane.mVec.y = NewOX;
                                const float NewForce = Math::Sqrt(CurAirplane.mVec.x * CurAirplane.mVec.x + CurAirplane.mVec.y * CurAirplane.mVec.y);
                                CurAirplane.mVec.x *= OldForce / NewForce;
                                CurAirplane.mVec.y *= OldForce / NewForce;
                                CurAirplane.mPos.x = CurAirplane.mPos.x * 0.95 + (CurFly.mPos.x + NewOX) * 0.05;
                                CurAirplane.mPos.y = CurAirplane.mPos.y * 0.95 + (CurFly.mPos.y + NewOY) * 0.05;
                            }
                        }
                        break;
                    case Eff::LINEUP:
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
					    else // 꼬리물기
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
                        break;
                    case Eff::CRASH:
                        if(CurAirplane.mDamageAni == 0)
                        {
                            CurAirplane.mDamageAni = 20;
                            auto CurTail = CurAirplane.mTail.mNext;
                            CurAirplane.mTail.mNext = nullptr;
			                while(CurTail)
			                {
                                auto NextTail = CurTail->mNext;
                                CurTail->mVecCount = exam->mAct(CurTail->mType, CurTail->mVec.x, CurTail->mVec.y);
				                CurTail->mPos += CurTail->mVec;
                                CurTail->mLinking = false;
                                CurTail->mPrev = nullptr;
                                CurTail->mNext = nullptr;
				                CurTail = NextTail;
			                }
                        }
                        break;
                    }
				}
			}
            CurAirplane.mLoopBegin = NextLoopBegin;

			// 꼬리당기기
			auto CurPos = CurAirplane.mPos;
            auto CurSizeR = CurAirplane.mSizeR;
			auto CurTail = CurAirplane.mTail.mNext;
			while(CurTail)
			{
				if(CurPos.x < CurTail->mPos.x - m->mScreenWidth / 2) CurPos.x += m->mScreenWidth;
				if(CurPos.y < CurTail->mPos.y - m->mScreenHeight / 2) CurPos.y += m->mScreenHeight;
				if(CurTail->mPos.x + m->mScreenWidth / 2 < CurPos.x) CurPos.x -= m->mScreenWidth;
				if(CurTail->mPos.y + m->mScreenHeight / 2 < CurPos.y) CurPos.y -= m->mScreenHeight;
				const float CurDistance = Math::Distance(CurPos.x, CurPos.y, CurTail->mPos.x, CurTail->mPos.y);
				if(CurSizeR + CurTail->mSizeR < CurDistance)
				{
					CurTail->mPos.x = (CurTail->mPos.x - CurPos.x) * (CurSizeR + CurTail->mSizeR) / CurDistance + CurPos.x;
					CurTail->mPos.y = (CurTail->mPos.y - CurPos.y) * (CurSizeR + CurTail->mSizeR) / CurDistance + CurPos.y;
				}
				CurPos = CurTail->mPos;
                CurSizeR = CurTail->mSizeR;
				CurTail = CurTail->mNext;
			}
		}

		// 파리처리
        if(m->mAddWait-- == 0)
        {
            auto& NewFly = m->mFlies[++m->mLastFlyID];
            m->mAddWait = exam->mAdd(NewFly.mPos.x, NewFly.mPos.y, NewFly.mSizeR);
        }
        for(sint32 i = 0, iend = m->mFlies.Count(); i < iend; ++i)
        {
            auto& CurFly = m->mFlies[i];
			if(!CurFly.mLinking)
			{
				if(CurFly.mVecCount-- == 0)
					CurFly.mVecCount = exam->mAct(CurFly.mType, CurFly.mVec.x, CurFly.mVec.y);
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
        if(m->mGameType == GameType::Unknown || m->mGameType == GameType::Key)
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
            m->mGameType = GameType::Key;
        }
	}
}

ZAY_VIEW_API OnNotify(chars sender, chars topic, id_share in, id_cloned_share* out)
{
}

ZAY_VIEW_API OnGesture(GestureType type, sint32 x, sint32 y)
{
    if(m->mGameType == GameType::Unknown && m->mAirplanes.Count() == 1)
    {
        if(type == GT_Pressed || type == GT_InDragging || type == GT_OutDragging)
        {
            const Point Pos = m->mAirplanes("MY").mPos;
            const float VecX = x - m->mScreenWidth / 2 - Pos.x;
            const float VecY = y - m->mScreenHeight / 2 - Pos.y;
            const float Rad = Math::Atan(VecX, VecY);
            sint32 Dist = 120;
            m->mFirstVector.x = Dist * Math::Cos(Rad);
            m->mFirstVector.y = Dist * Math::Sin(Rad);
            m->invalidate();
        }
        else if(type == GT_InReleased || type == GT_OutReleased)
        {
            m->mAirplanes("MY").mVec = m->mFirstVector / 60;
            m->mGameType = GameType::Touch;
        }
    }
    else if(m->mGameType == GameType::Touch)
    {
        if(type == GT_Pressed)
        {
            for(sint32 i = m->mFlies.Count() - 1; 0 <= i; --i)
		    {
                auto& CurFly = m->mFlies[i];
			    const float CurDistance = Math::Distance(x - m->mScreenWidth / 2, y - m->mScreenHeight / 2, CurFly.mPos.x, CurFly.mPos.y);
                if(CurDistance < CurFly.mSizeR)
                {
                    m->mPressedBulletFly = i;
                    break;
                }
            }
        }
        else if(type == GT_InReleased || type == GT_OutReleased)
            m->mPressedBulletFly = -1;
    }
}

ZAY_VIEW_API OnRender(ZayPanel& panel)
{
    ZAY_RGB(panel, 192, 192, 192)
        panel.fill();

	// 파리들
    for(sint32 i = 0, iend = m->mFlies.Count(); i < iend; ++i)
    {
        auto& CurFly = m->mFlies[i];
        ZAY_RGBA_IF(panel, 224, 224, 192, 24, i == m->mPressedBulletFly)
        ZAY_RGBA_IF(panel, 128, 128, 128, 192, i != m->mPressedBulletFly)
        ZAY_XYRR(panel, panel.w() / 2 + CurFly.mPos.x, panel.h() / 2 + CurFly.mPos.y, CurFly.mSizeR, CurFly.mSizeR)
            exam->mRender(panel, CurFly.mType);
    }

	// 비행기
    for(sint32 i = 0, iend = m->mAirplanes.Count(); i < iend; ++i)
    {
        chararray ID;
        auto& CurAirplane = *m->mAirplanes.AccessByOrder(i, &ID);
        ZAY_XYRR(panel, panel.w() / 2 + CurAirplane.mPos.x, panel.h() / 2 + CurAirplane.mPos.y, 0, 0)
		{
            // 본체
			bool IsMyTouch = !String::Compare(&ID[0], "MY");
			CurAirplane.mSizeR = exam->mRenderUser(panel, (IsMyTouch)? 0 : Parser::GetInt(&ID[1]),
				CurAirplane.mVec.x, CurAirplane.mVec.y);
            // 데이지표현
            if(0 < CurAirplane.mDamageAni)
            {
                const sint32 SizeR = CurAirplane.mSizeR + (20 - CurAirplane.mDamageAni) * 5;
                ZAY_XYRR(panel, panel.w() / 2, panel.h() / 2, SizeR, SizeR)
                {
                    CurAirplane.mDamageAni--;
                    ZAY_RGBA(panel, 255, 0, 0, CurAirplane.mDamageAni * 10)
                        panel.circle();
                }
            }
		}
        // 초기방향성
        if(i == 0 && m->mGameType == GameType::Unknown)
        {
            const Point LineBegin = Point(panel.w() / 2 + CurAirplane.mPos.x, panel.h() / 2 + CurAirplane.mPos.y);
            const Point LineEnd = Point(LineBegin.x + m->mFirstVector.x, LineBegin.y + m->mFirstVector.y);
            ZAY_RGBA(panel, 255, 255, 0, 128)
                panel.line(LineBegin, LineEnd, 5);
        }
    }

	m->RenderUI(panel);
	MissionCollector::RenderUI(panel);
}

bulletsData::bulletsData() : SyncuClient("bullets")
{
	mAirplanes("MY");
	mLastFlyID = -1;
	mAddWait = 0;
    mGameType = GameType::Unknown;
    mPressedBulletFly = -1;
}

bulletsData::~bulletsData()
{
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
