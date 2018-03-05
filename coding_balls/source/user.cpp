#include <boss.hpp>
#include "balls.hpp"

void ballsData::OnTick(float sec, Point& pos, Point& vec)
{
    vec.y += 1;
}

bool ballsData::OnCrashWall(Wall wall, Point& vec)
{
    if(wall == Wall::LEFT)
    {
        vec.x = Math::AbsF(vec.x);
    }
    if(wall == Wall::TOP)
    {
        vec.y = Math::AbsF(vec.y);
    }
    if(wall == Wall::RIGHT)
    {
        vec.x = -Math::AbsF(vec.x);
    }
    if(wall == Wall::BOTTOM)
    {
        vec.y = -Math::AbsF(vec.y) * 0.95;
    }
    return true;
}

bool ballsData::OnCrashBall(Point& vecA, Point& vecB)
{
    return false;
}
