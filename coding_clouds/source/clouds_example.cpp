#include <boss.hpp>
#include "clouds_example.hpp"
#include <resource.hpp>

////////////////////////////////////////////////////////////////////////////////
#define LEVEL_NUMBER LEVEL_USER

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 0
MISSION_DECLARE("STEP_0")

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
