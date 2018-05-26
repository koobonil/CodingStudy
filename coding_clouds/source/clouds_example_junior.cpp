#include <boss.hpp>
#include "clouds_example.hpp"
#include <resource.hpp>

MISSION_JUNIOR_DECLARE(MISSION_NAME, 0, "STEP_0")

static bool NeedCalc = true;
static float FirstDistance = 0;
static Point OldActorPos;

float CloudsExample::Junior::OnTickScroll(Point& actor_pos, Point& actor_vec, const Point* grabbed_cloud, float grabbed_sec)
{
	if(grabbed_cloud)
	{
		if(NeedCalc)
		{
			NeedCalc = false;
			FirstDistance = Math::Sqrt(grabbed_cloud->x * grabbed_cloud->x + grabbed_cloud->y * grabbed_cloud->y);
		}

		const float CurDistance = Math::Sqrt(grabbed_cloud->x * grabbed_cloud->x + grabbed_cloud->y * grabbed_cloud->y);
		const float CalcedDistance = FirstDistance * Math::MaxF(0, (2 - grabbed_sec) / 2.0f);
		actor_pos.x += grabbed_cloud->x * (CurDistance - CalcedDistance) / CurDistance;
		actor_pos.y += grabbed_cloud->y * (CurDistance - CalcedDistance) / CurDistance;
		actor_vec.x = actor_vec.x * 0.5 + (actor_pos.x - OldActorPos.x) * 0.5;
		actor_vec.y = actor_vec.y * 0.5 + (actor_pos.y - OldActorPos.y) * 0.5;
	}
	else NeedCalc = true;

	actor_vec.y += 0.1;
	OldActorPos = actor_pos;
	return 0.002;
}

Rect CloudsExample::Junior::OnAddCloud(float& skywidth)
{
	skywidth = 0.25;
	switch(Platform::Utility::Random() % 4)
	{
	case 0: return Rect(0.4, 0.2, 0.6, 0.3);
	case 1: return Rect(0.2, 0.3, 0.4, 0.4);
	case 2: return Rect(0.4, 0.4, 0.6, 0.5);
	case 3: return Rect(0.6, 0.1, 0.8, 0.2);
	}
	return Rect(0.4, 0.4, 0.6, 0.6);
}

void CloudsExample::Junior::OnRenderCloud(ZayPanel& panel)
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

void CloudsExample::Junior::OnRenderActor(ZayPanel& panel, const Point* grabbed_cloud)
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
