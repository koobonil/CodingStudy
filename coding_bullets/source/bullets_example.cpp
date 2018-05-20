#include <boss.hpp>
#include "bullets_example.hpp"
#include <resource.hpp>

MISSION_USER_DECLARE(MISSION_NAME, 0, "STEP_0")

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
