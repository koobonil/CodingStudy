#include <boss.hpp>
#include "rains_example.hpp"
#include <resource.hpp>

////////////////////////////////////////////////////////////////////////////////
#define LEVEL_NUMBER LEVEL_USER

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 0
MISSION_DECLARE("STEP_0")

int RainsExample::OnTestRain(int x1, int y1, int x2, int y2)
{
	return 0;
}

void RainsExample::OnRenderBowl(ZayPanel& panel)
{
}
