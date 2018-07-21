#include <boss.hpp>
#include "balls_example.hpp"
#include <resource.hpp>

////////////////////////////////////////////////////////////////////////////////
#define LEVEL_NUMBER LEVEL_USER

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 0
MISSION_DECLARE("STEP_0")

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
