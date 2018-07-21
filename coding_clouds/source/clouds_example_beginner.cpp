#include <boss.hpp>
#include "clouds_example.hpp"
#include <resource.hpp>

////////////////////////////////////////////////////////////////////////////////
#define LEVEL_NUMBER LEVEL_BEGINNER

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 0
MISSION_DECLARE("STEP_0")

float CloudsExample::Beginner::OnTickScroll(Point& actor_pos, Point& actor_vec, const Point* grabbed_cloud, float grabbed_sec)
{
	if(grabbed_cloud)
	{
		actor_vec.x = actor_vec.x * 0.95 + grabbed_cloud->x * 0.05;
		actor_vec.y = actor_vec.y * 0.95 + grabbed_cloud->y * 0.05;
	}

	actor_vec.y += 0.1;
	return 0.002;
}

Rect CloudsExample::Beginner::OnAddCloud(float& skywidth)
{
	skywidth = 0.25;
	return Rect(0.4, 0.2, 0.6, 0.3);
}
