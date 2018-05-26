#include <boss.hpp>
#include "clouds_example.hpp"
#include <resource.hpp>

MISSION_USER_DECLARE(MISSION_NAME, 0, "STEP_0")

float CloudsExample::OnTickScroll(Point& actor_pos, Point& actor_vec, const Point* grabbed_cloud, float grabbed_sec)
{
	return 0.001;
}

Rect CloudsExample::OnAddCloud(float& skywidth)
{
	return Rect(0.4, 0.4, 0.6, 0.6);
}

void CloudsExample::OnRenderCloud(ZayPanel& panel)
{
}

void CloudsExample::OnRenderActor(ZayPanel& panel, const Point* grabbed_cloud)
{
}
