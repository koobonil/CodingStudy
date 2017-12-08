#include <boss.hpp>
#include "balls.hpp"

void ballsData::OnTick(float sec, Point& pos, Point& vec)
{
}

bool ballsData::OnCrashWall(Wall wall, Point& vec)
{
    return false;
}

bool ballsData::OnCrashBall(Point& vecA, Point& vecB)
{
    return false;
}
