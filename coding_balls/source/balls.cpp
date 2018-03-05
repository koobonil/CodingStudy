#include <boss.hpp>
#include "balls.hpp"

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
        for(sint32 i = 0; i < m->balls.Count(); ++i)
        {
            m->balls.At(i).pos += m->balls[i].vec;
            m->OnTick((Time - m->balls[i].time) * 0.001f, m->balls.At(i).pos, m->balls.At(i).vec);
            // 벽충돌
            if(m->balls[i].pos.x - m->balls[i].sizeR < 0)
            {
                if(m->OnCrashWall(ballsData::Wall::LEFT, m->balls.At(i).vec))
                    m->balls.At(i).time = Time;
            }
            else if(m->balls[i].pos.y - m->balls[i].sizeR < 0)
            {
                if(m->OnCrashWall(ballsData::Wall::TOP, m->balls.At(i).vec))
                    m->balls.At(i).time = Time;
            }
            else if(Width <= m->balls[i].pos.x + m->balls[i].sizeR)
            {
                if(m->OnCrashWall(ballsData::Wall::RIGHT, m->balls.At(i).vec))
                    m->balls.At(i).time = Time;
            }
            else if(Height <= m->balls[i].pos.y + m->balls[i].sizeR)
            {
                if(m->OnCrashWall(ballsData::Wall::BOTTOM, m->balls.At(i).vec))
                    m->balls.At(i).time = Time;
            }
            // 공충돌
            else for(sint32 j = 0; j < m->balls.Count(); ++j)
            {
                if(i == j) continue;
                if(m->balls[j].time == Time) continue;
                const float Distance = Math::Distance(m->balls[i].pos.x, m->balls[i].pos.y, m->balls[j].pos.x, m->balls[j].pos.y);
                if(Distance < m->balls[i].sizeR + m->balls[j].sizeR)
                {
                    if(m->OnCrashBall(m->balls.At(i).vec, m->balls.At(j).vec))
                    {
                        m->balls.At(i).time = Time;
                        m->balls.At(j).time = Time;
                        break;
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
        const sint32 Count = m->balls.Count();
        auto& NewBall = m->balls.AtAdding();
        NewBall.time = Platform::Utility::CurrentTimeMsec();
        NewBall.sizeR = 20;
        NewBall.color = BallColor[Count % 4];
        NewBall.pos = Point(x, y);
        const float Radian = Math::ToRadian(Platform::Utility::Random() % 360);
        NewBall.vec.x = 10 * Math::Cos(Radian);
        NewBall.vec.y = 10 * Math::Sin(Radian);
    }
}

ZAY_VIEW_API OnRender(ZayPanel& panel)
{
    ZAY_RGB(panel, 160, 160, 160)
        panel.fill();

    for(sint32 i = 0; i < m->balls.Count(); ++i)
    {
        ZAY_XYRR(panel, m->balls[i].pos.x, m->balls[i].pos.y, m->balls[i].sizeR, m->balls[i].sizeR)
        ZAY_COLOR(panel, m->balls[i].color)
            panel.circle();
    }
}

/*void ballsData::OnTick(float sec, Point& pos, Point& vec)
{
    pos.y += 10 * sec;
}

bool ballsData::OnCrashWall(Wall wall, Point& vec)
{
    switch(wall)
    {
    case Wall::LEFT:
        vec.x = Math::Abs(vec.x);
        break;
    case Wall::TOP:
        vec.y = Math::Abs(vec.y);
        break;
    case Wall::RIGHT:
        vec.x = -Math::Abs(vec.x);
        break;
    case Wall::BOTTOM:
        vec.y = -Math::Abs(vec.y);
        break;
    }
    return true;
}

bool ballsData::OnCrashBall(Point& vecA, Point& vecB)
{
    vecA.x *= -1;
    vecB.x *= -1;
    return true;
}*/
