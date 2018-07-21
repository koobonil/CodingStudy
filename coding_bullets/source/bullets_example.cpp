#include <boss.hpp>
#include "bullets_example.hpp"
#include <resource.hpp>

////////////////////////////////////////////////////////////////////////////////
#define LEVEL_NUMBER LEVEL_USER

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 0
MISSION_DECLARE("STEP_0")

int BulletsExample::OnAdd(float& x, float& y)
{
    return -1;
}

int BulletsExample::OnAct(int& type, float& vx, float& vy)
{
	return 0;
}

void BulletsExample::OnRender(ZayPanel& panel, int type)
{
}

int BulletsExample::OnRenderUser(ZayPanel& panel, int id, float vx, float vy)
{
	return 0;
}
