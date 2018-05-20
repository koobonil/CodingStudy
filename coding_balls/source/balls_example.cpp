#include <boss.hpp>
#include "balls_example.hpp"
#include <resource.hpp>

MISSION_USER_DECLARE(MISSION_NAME, 0, "STEP_0")

void BallsExample::OnTick(float sec, Point& pos, Point& vec)
{
}

bool BallsExample::OnCrashWall(Wall wall, Point& vec)
{
    return false;
}

bool BallsExample::OnCrashBall(Point& vecA, Point& vecB)
{
    return false;
}
