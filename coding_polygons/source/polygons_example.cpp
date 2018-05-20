#include <boss.hpp>
#include "polygons_example.hpp"
#include <resource.hpp>

MISSION_USER_DECLARE(MISSION_NAME, 0, "STEP_0")

int PolygonsExample::OnAdd(float& x, float& y)
{
    return 20;
}

int PolygonsExample::OnAct(sint32& type, float& vx, float& vy)
{
    return 0;
}

int PolygonsExample::OnScore(sint32 type)
{
    return 0;
}

void PolygonsExample::OnRender(ZayPanel& panel, sint32 type, sint32 ani)
{
}
