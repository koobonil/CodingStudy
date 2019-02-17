#include <boss.hpp>
#include "polygons.hpp"
MISSION_VIEW_DECLARE(MISSION_NAME, "polygonsView", PolygonsExample)

#include <resource.hpp>
ZAY_DECLARE_VIEW_CLASS("polygonsView", polygonsData)

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

        // 파리처리
        if(m->mAddWait-- == 0)
        {
            auto& NewFly = m->mFlies.AtAdding();
            m->mAddWait = exam->mAdd(NewFly.mPos.x, NewFly.mPos.y);
        }
        for(sint32 i = 0, iend = m->mFlies.Count(); i < iend; ++i)
        {
            auto& CurFly = m->mFlies.At(i);
            if(CurFly.mDeathAnimate == 0)
            {
                if(CurFly.mVecCount-- == 0)
                    CurFly.mVecCount = exam->mAct(CurFly.mType, CurFly.mVec.x, CurFly.mVec.y);
                CurFly.mPos += CurFly.mVec;
                while(CurFly.mPos.x > m->mScreenWidth / 2) CurFly.mPos.x -= m->mScreenWidth;
                while(CurFly.mPos.y > m->mScreenHeight / 2) CurFly.mPos.y -= m->mScreenHeight;
                while(CurFly.mPos.x < -m->mScreenWidth / 2) CurFly.mPos.x += m->mScreenWidth;
                while(CurFly.mPos.y < -m->mScreenHeight / 2) CurFly.mPos.y += m->mScreenHeight;
            }
        }
    }
}

ZAY_VIEW_API OnNotify(NotifyType type, chars topic, id_share in, id_cloned_share* out)
{
}

ZAY_VIEW_API OnGesture(GestureType type, sint32 x, sint32 y)
{
    x -= m->mScreenWidth / 2;
    y -= m->mScreenHeight / 2;
    auto& CurArea = m->mAreas("MY");
    const sint32 SpotCount = CurArea.mSpotCount;

    if(type == GT_Pressed)
    {
        CurArea.mTouchEnable = true;
        CurArea.mTouchPos = Point(x, y);
    }
    else if(type == GT_InDragging || type == GT_OutDragging)
    {
        CurArea.mTouchPos = Point(x, y);
    }
    else if(type == GT_InReleased || type == GT_OutReleased)
    {
        CurArea.mTouchEnable = false;
        if(SpotCount < 3)
        {
            CurArea.mSpotPos[SpotCount] = CurArea.mTouchPos;
            CurArea.mSpotCount++;
            CurArea.mSpotAnimate = 10;
        }
    }
}

ZAY_VIEW_API OnRender(ZayPanel& panel)
{
    ZAY_RGB(panel, 0, 162, 232)
        panel.fill();

    // 파리들
    for(sint32 i = 0, iend = m->mFlies.Count(); i < iend; ++i)
    {
        auto& CurFly = m->mFlies[i];
        ZAY_XYRR(panel, panel.w() / 2 + CurFly.mPos.x, panel.h() / 2 + CurFly.mPos.y, 0, 0)
            exam->mRender(panel, CurFly.mType, CurFly.mDeathAnimate);
    }
    // 파리제거
    for(sint32 i = m->mFlies.Count() - 1; 0 <= i; --i)
    {
        auto& CurFly = m->mFlies.At(i);
        if(0 < CurFly.mDeathAnimate && --CurFly.mDeathAnimate == 0)
        {
            const sint32 CurScore = exam->mScore(CurFly.mType);
            m->mScore = Math::Max(0, m->mScore + CurScore);
            m->mFlies.SubtractionSection(i);
        }
    }

    // 영역들
    for(sint32 i = 0, iend = m->mAreas.Count(); i < iend; ++i)
    {
        auto& CurArea = *m->mAreas.AccessByOrder(i);
        // 출력
        for(sint32 j = 0, jend = CurArea.mSpotCount; j < jend; ++j)
        {
            auto& CurSpotPos = CurArea.mSpotPos[j];
            auto& NextSpotPos = CurArea.mSpotPos[(j + 1) % jend];
            ZAY_XYRR(panel, panel.w() / 2 + CurSpotPos.x, panel.h() / 2 + CurSpotPos.y, 5, 5)
            ZAY_RGBA(panel, 0, 0, 0, 128)
            ZAY_RGBA_IF(panel, 255, 255, 0, 128, CurArea.mSpotAnimate & 1)
                panel.circle();
            if(1 < jend)
            {
                ZAY_XYRR(panel, panel.w() / 2, panel.h() / 2, 0, 0)
                ZAY_RGB(panel, 0, 0, 0)
                ZAY_RGB_IF(panel, 255, 255, 0, CurArea.mSpotAnimate & 1)
                    panel.line(CurSpotPos, NextSpotPos, 2);
            }
        }
        // 애니메이션
        if(CurArea.mSpotCount == 3 && 0 < CurArea.mSpotAnimate)
        {
            if(--CurArea.mSpotAnimate == 0)
            {
                // 죽을 파리선정
                for(sint32 j = 0, jend = m->mFlies.Count(); j < jend; ++j)
                {
                    auto& CurFly = m->mFlies.At(j);
                    if(CurFly.mDeathAnimate == 0)
                    {
                        sint32 TestValue = 0;
                        for(sint32 k = 0, kend = CurArea.mSpotCount; k < kend; ++k)
                        {
                            auto& CurSpotPos = CurArea.mSpotPos[k];
                            auto& NextSpotPos = CurArea.mSpotPos[(k + 1) % kend];
                            if((CurSpotPos.x - NextSpotPos.x) * (CurFly.mPos.y - NextSpotPos.y) -
                                (CurSpotPos.y - NextSpotPos.y) * (CurFly.mPos.x - NextSpotPos.x) < 0)
                                TestValue++;
                            else TestValue--;
                        }
                        if(Math::Abs(TestValue) == CurArea.mSpotCount)
                            CurFly.mDeathAnimate = 10;
                    }
                }
                CurArea.mSpotCount = 0;
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
            panel.text(String::Format(" %06d", m->mScore), UIFA_LeftTop);
    }

    // 터치좌표
    for(sint32 i = 0, iend = m->mAreas.Count(); i < iend; ++i)
    {
        chararray ID;
        auto& CurArea = *m->mAreas.AccessByOrder(i, &ID);
        if(CurArea.mTouchEnable)
        {
            bool IsMyTouch = !String::Compare(&ID[0], "MY");
            ZAY_RGB_IF(panel, 0, 0, 255, IsMyTouch)
            ZAY_RGB_IF(panel, 128, 0, 128, !IsMyTouch)
            ZAY_XYRR(panel, panel.w() / 2 + CurArea.mTouchPos.x, panel.h() / 2 + CurArea.mTouchPos.y, 0, 0)
            {
                ZAY_LTRB(panel, -20, -2, 20, 2)
                    panel.fill();
                ZAY_LTRB(panel, -2, -20, 2, 20)
                    panel.fill();
            }
        }
    }

	m->RenderUI(panel);
	MissionCollector::RenderUI(panel);
}

polygonsData::polygonsData() : SyncuClient("polygons")
{
    mScore = 0;
    mAddWait = 0;
	mAreas("MY");
}

polygonsData::~polygonsData()
{
}

void polygonsData::OnVisit(chars id)
{
}

void polygonsData::OnLeave(chars id)
{
	mAreas.Remove(id);
}

void polygonsData::OnEvent(chars id, const Context& json)
{
	auto& CurArea = mAreas(id);
	const sint32 SpotCount = CurArea.mSpotCount;
	chars PayloadType = json("type").GetString();
	if (!String::Compare(PayloadType, "released"))
	{
		CurArea.mTouchEnable = false;
		if (SpotCount < 3)
		{
			CurArea.mSpotPos[SpotCount] = CurArea.mTouchPos;
			CurArea.mSpotCount++;
			CurArea.mSpotAnimate = 10;
		}
	}
	else
	{
		const sint32 PayloadX = json("x").GetInt();
		const sint32 PayloadY = json("y").GetInt();
		const sint32 x = mScreenWidth * PayloadX / 1000 - m->mScreenWidth / 2;
		const sint32 y = mScreenHeight * PayloadY / 1000 - m->mScreenHeight / 2;
		if (!String::Compare(PayloadType, "pressed"))
		{
			CurArea.mTouchEnable = true;
			CurArea.mTouchPos = Point(x, y);
		}
		else if (!String::Compare(PayloadType, "dragging"))
			CurArea.mTouchPos = Point(x, y);
	}
}
