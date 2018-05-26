#include <boss.hpp>
#include "clouds_example.hpp"
#include <resource.hpp>

MISSION_SENIOR_DECLARE(MISSION_NAME, 0, "STEP_0")

float CloudsExample::Senior::OnTickScroll(Point& actor_pos, Point& actor_vec, const Point* grabbed_cloud, float grabbed_sec)
{
	actor_vec.y += 0.1;
	return 0.001;
}

Rect CloudsExample::Senior::OnAddCloud(float& skywidth)
{
	return Rect(0.4, 0.2, 0.6, 0.3);
}

void CloudsExample::Senior::OnRenderCloud(ZayPanel& panel)
{
	ZAY_RGB(panel, 128, 128, 128)
	{
		ZAY_XYRR(panel, 0, panel.h() / 2, panel.h() / 3, panel.h() / 3)
			panel.circle();
		ZAY_XYRR(panel, panel.w(), panel.h() / 2, panel.h() / 3, panel.h() / 3)
			panel.circle();
		panel.circle();
	}
}

void CloudsExample::Senior::OnRenderActor(ZayPanel& panel, const Point* grabbed_cloud)
{
	if(grabbed_cloud)
	{
		ZAY_RGB(panel, 255, 128, 0)
		ZAY_XYRR(panel, panel.w() / 2, panel.h() / 2, 0, 0)
			panel.line(Point(0, 0), *grabbed_cloud, 3);
	}

	ZAY_RGB(panel, 0, 0, 255)
		panel.circle();
}
