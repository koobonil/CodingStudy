#include <boss.hpp>
#include "balls_example.hpp"
#include <resource.hpp>

MISSION_JUNIOR_DECLARE(MISSION_NAME, 0, "STEP_0")

void BallsExample::Junior::OnTick(float sec, Point& pos, Point& vec)
{
    pos.y += 10 * sec;
}

bool BallsExample::Junior::OnCrashWall(Wall wall, Point& vec)
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

bool BallsExample::Junior::OnCrashBall(Point& vecA, Point& vecB)
{
	vecA.x *= -1;
    vecB.x *= -1;
    return false;
}
