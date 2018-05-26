#include <boss.hpp>
#include "clouds_example.hpp"
#include <resource.hpp>

MISSION_BEGINNER_DECLARE(MISSION_NAME, 0, "STEP_0")

float CloudsExample::Beginner::OnTickScroll(Point& actor_pos, Point& actor_vec, const Point* grabbed_cloud, float grabbed_sec)
{
	actor_vec.y += 0.1;
	if(grabbed_cloud)
	{
		actor_vec.x = actor_vec.x * 0.95 + grabbed_cloud->x * 0.05;
		actor_vec.y = actor_vec.y * 0.95 + grabbed_cloud->y * 0.05;
	}
	return 0.002;
}

Rect CloudsExample::Beginner::OnAddCloud(float& skywidth)
{
	skywidth = 0.25;
	return Rect(0.4, 0.2, 0.6, 0.3);
}
