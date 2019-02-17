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
        for(sint32 i = 0; i < m->mBalls.Count(); ++i)
        {
            m->mBalls.At(i).mPos += m->mBalls[i].mVec;
            exam->mTick((Time - m->mBalls[i].mTime) * 0.001f, m->mBalls.At(i).mPos, m->mBalls.At(i).mVec);
            // 벽충돌
            if(m->mBalls[i].mPos.x - m->mBalls[i].mSizeR < 0)
            {
                if(exam->mCrashWall(Wall::LEFT, m->mBalls.At(i).mVec))
                    m->mBalls.At(i).mTime = Time;
            }
            else if(m->mBalls[i].mPos.y - m->mBalls[i].mSizeR < 0)
            {
                if(exam->mCrashWall(Wall::TOP, m->mBalls.At(i).mVec))
                    m->mBalls.At(i).mTime = Time;
            }
            else if(Width <= m->mBalls[i].mPos.x + m->mBalls[i].mSizeR)
            {
                if(exam->mCrashWall(Wall::RIGHT, m->mBalls.At(i).mVec))
                    m->mBalls.At(i).mTime = Time;
            }
            else if(Height <= m->mBalls[i].mPos.y + m->mBalls[i].mSizeR)
            {
                if(exam->mCrashWall(Wall::BOTTOM, m->mBalls.At(i).mVec))
                    m->mBalls.At(i).mTime = Time;
            }
            // 공충돌
            else if(exam->mCrashBall)
			{
				for(sint32 j = 0; j < m->mBalls.Count(); ++j)
				{
					if(i == j) continue;
					if(m->mBalls[j].mTime == Time) continue;
					const float Distance = Math::Distance(m->mBalls[i].mPos.x, m->mBalls[i].mPos.y, m->mBalls[j].mPos.x, m->mBalls[j].mPos.y);
					if(Distance < m->mBalls[i].mSizeR + m->mBalls[j].mSizeR)
					{
						if(exam->mCrashBall(m->mBalls.At(i).mVec, m->mBalls.At(j).mVec))
						{
							m->mBalls.At(i).mTime = Time;
							m->mBalls.At(j).mTime = Time;
							break;
						}
					}
				}
			}
        }
        m->invalidate();
    }
}

ZAY_VIEW_API OnNotify(NotifyType type, chars topic, id_share in, id_cloned_share* out)
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
        const sint32 Count = m->mBalls.Count();
        auto& NewBall = m->mBalls.AtAdding();
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

    for(sint32 i = 0; i < m->mBalls.Count(); ++i)
    {
        ZAY_XYRR(panel, m->mBalls[i].mPos.x, m->mBalls[i].mPos.y, m->mBalls[i].mSizeR, m->mBalls[i].mSizeR)
        ZAY_COLOR(panel, m->mBalls[i].mColor)
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
