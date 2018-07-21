#include <boss.hpp>
#include "balls_example.hpp"
#include <resource.hpp>

////////////////////////////////////////////////////////////////////////////////
#define LEVEL_NUMBER LEVEL_SENIOR

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 0
MISSION_DECLARE("STEP_0")

void BallsExample::Senior::OnTick(float sec, Point& pos, Point& vec)
{
    vec.y += 1;
}

bool BallsExample::Senior::OnCrashWall(Wall wall, Point& vec)
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

bool BallsExample::Senior::OnCrashBall(Point& vecA, Point& vecB)
{
    return false;
}
