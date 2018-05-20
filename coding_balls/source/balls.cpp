#include <boss.hpp>
#include "balls.hpp"
MISSION_VIEW_DECLARE(MISSION_NAME, "ballsView", BallsExample)

#include <resource.hpp>
ZAY_DECLARE_VIEW_CLASS("ballsView", ballsData)

ZAY_VIEW_API OnCommand(CommandType type, chars topic, id_share in, id_cloned_share* out)
{
    static sint32 Width = 0;
    static sint32 Height = 0;
	if(type == CT_Size)
    {
        sint32s WH = in;
        Width = WH[0];
        Height = WH[1];
    }
    else if(type == CT_Tick)
    {
        uint64 Time = Platform::Utility::CurrentTimeMsec();
        for(sint32 i = 0; i < m->Balls.Count(); ++i)
        {
            m->Balls.At(i).mPos += m->Balls[i].mVec;
            exam->mTick((Time - m->Balls[i].mTime) * 0.001f, m->Balls.At(i).mPos, m->Balls.At(i).mVec);
            // 벽충돌
            if(m->Balls[i].mPos.x - m->Balls[i].mSizeR < 0)
            {
                if(exam->mCrashWall(Wall::LEFT, m->Balls.At(i).mVec))
                    m->Balls.At(i).mTime = Time;
            }
            else if(m->Balls[i].mPos.y - m->Balls[i].mSizeR < 0)
            {
                if(exam->mCrashWall(Wall::TOP, m->Balls.At(i).mVec))
                    m->Balls.At(i).mTime = Time;
            }
            else if(Width <= m->Balls[i].mPos.x + m->Balls[i].mSizeR)
            {
                if(exam->mCrashWall(Wall::RIGHT, m->Balls.At(i).mVec))
                    m->Balls.At(i).mTime = Time;
            }
            else if(Height <= m->Balls[i].mPos.y + m->Balls[i].mSizeR)
            {
                if(exam->mCrashWall(Wall::BOTTOM, m->Balls.At(i).mVec))
                    m->Balls.At(i).mTime = Time;
            }
            // 공충돌
            else if(exam->mCrashBall)
			{
				for(sint32 j = 0; j < m->Balls.Count(); ++j)
				{
					if(i == j) continue;
					if(m->Balls[j].mTime == Time) continue;
					const float Distance = Math::Distance(m->Balls[i].mPos.x, m->Balls[i].mPos.y, m->Balls[j].mPos.x, m->Balls[j].mPos.y);
					if(Distance < m->Balls[i].mSizeR + m->Balls[j].mSizeR)
					{
						if(exam->mCrashBall(m->Balls.At(i).mVec, m->Balls.At(j).mVec))
						{
							m->Balls.At(i).mTime = Time;
							m->Balls.At(j).mTime = Time;
							break;
						}
					}
				}
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
    const Color BallColor[4] = {
        Color(255, 0, 0, 128),
        Color(0, 0, 255, 128),
        Color(255, 255, 0, 128),
        Color(0, 255, 0, 128)};

    if(type == GT_Pressed)
    {
        const sint32 Count = m->Balls.Count();
        auto& NewBall = m->Balls.AtAdding();
        NewBall.mTime = Platform::Utility::CurrentTimeMsec();
        NewBall.mSizeR = 20;
        NewBall.mColor = BallColor[Count % 4];
        NewBall.mPos = Point(x, y);
        const float Radian = Math::ToRadian(Platform::Utility::Random() % 360);
        NewBall.mVec.x = 10 * Math::Cos(Radian);
        NewBall.mVec.y = 10 * Math::Sin(Radian);
    }
}

ZAY_VIEW_API OnRender(ZayPanel& panel)
{
    ZAY_RGB(panel, 160, 160, 160)
        panel.fill();

    for(sint32 i = 0; i < m->Balls.Count(); ++i)
    {
        ZAY_XYRR(panel, m->Balls[i].mPos.x, m->Balls[i].mPos.y, m->Balls[i].mSizeR, m->Balls[i].mSizeR)
        ZAY_COLOR(panel, m->Balls[i].mColor)
            panel.circle();
    }

	MissionCollector::RenderUI(panel);
}

ballsData::ballsData()
{
}

ballsData::~ballsData()
{
}
