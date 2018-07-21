#include <boss.hpp>
#include "clouds_example.hpp"
#include <resource.hpp>

////////////////////////////////////////////////////////////////////////////////
#define LEVEL_NUMBER LEVEL_JUNIOR

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 0
MISSION_DECLARE("모든 요소를 준비")

STEP_API(float OnTickScroll)(Point& actor_pos, Point& actor_vec, const Point* grabbed_cloud, float grabbed_sec)
{
	if(grabbed_cloud)
	{
		actor_vec.x = actor_vec.x * 0.95 + grabbed_cloud->x * 0.05;
		actor_vec.y = actor_vec.y * 0.95 + grabbed_cloud->y * 0.05;
	}

	actor_vec.y += 0.1;
	return 0.002;
}

STEP_API(Rect OnAddCloud)(float& skywidth)
{
	return Rect(0.4, 0.4, 0.6, 0.6);
}

STEP_API(void OnRenderCloud)(ZayPanel& panel)
{
	ZAY_RGB(panel, 128, 128, 128)
		panel.circle();
}

STEP_API(void OnRenderActor)(ZayPanel& panel, const Point* grabbed_cloud)
{
	ZAY_RGB(panel, 0, 0, 255)
		panel.circle();
}

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 1
MISSION_DECLARE("구름의 타입을 4개로")

STEP_API(Rect OnAddCloud)(float& skywidth)
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

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 2
MISSION_DECLARE("구름을 이쁘게")

STEP_API(void OnRenderCloud)(ZayPanel& panel)
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

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 3
MISSION_DECLARE("구름줄을 표현")

STEP_API(void OnRenderActor)(ZayPanel& panel, const Point* grabbed_cloud)
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

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 4
MISSION_DECLARE("구름을 잡아보자")

static bool NeedCalc = true;
static float FirstDistance = 0;

STEP_API(float OnTickScroll)(Point& actor_pos, Point& actor_vec, const Point* grabbed_cloud, float grabbed_sec)
{
	if(grabbed_cloud)
	{
		if(NeedCalc)
		{
			NeedCalc = false;
			FirstDistance = Math::Sqrt(grabbed_cloud->x * grabbed_cloud->x + grabbed_cloud->y * grabbed_cloud->y);
		}

		const float CurDistance = Math::Sqrt(grabbed_cloud->x * grabbed_cloud->x + grabbed_cloud->y * grabbed_cloud->y);
		actor_pos.x += grabbed_cloud->x * (CurDistance - FirstDistance) / CurDistance;
		actor_pos.y += grabbed_cloud->y * (CurDistance - FirstDistance) / CurDistance;
	}
	else NeedCalc = true;

	actor_vec.y += 0.1;
	return 0.002;
}

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 5
MISSION_DECLARE("흔들흔들")

static Point OldActorPos;

STEP_API(float OnTickScroll)(Point& actor_pos, Point& actor_vec, const Point* grabbed_cloud, float grabbed_sec)
{
	if(grabbed_cloud)
	{
		if(NeedCalc)
		{
			NeedCalc = false;
			FirstDistance = Math::Sqrt(grabbed_cloud->x * grabbed_cloud->x + grabbed_cloud->y * grabbed_cloud->y);
		}

		const float CurDistance = Math::Sqrt(grabbed_cloud->x * grabbed_cloud->x + grabbed_cloud->y * grabbed_cloud->y);
		actor_pos.x += grabbed_cloud->x * (CurDistance - FirstDistance) / CurDistance;
		actor_pos.y += grabbed_cloud->y * (CurDistance - FirstDistance) / CurDistance;
		actor_vec.x = actor_vec.x * 0.5 + (actor_pos.x - OldActorPos.x) * 0.5;
		actor_vec.y = actor_vec.y * 0.5 + (actor_pos.y - OldActorPos.y) * 0.5;
	}
	else NeedCalc = true;

	actor_vec.y += 0.1;
	OldActorPos = actor_pos;
	return 0.002;
}

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 6
MISSION_DECLARE("2초만에 구름까지")

STEP_API(float OnTickScroll)(Point& actor_pos, Point& actor_vec, const Point* grabbed_cloud, float grabbed_sec)
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

////////////////////////////////////////////////////////////////////////////////
static int gStep = 0;
int CloudsExample::Junior::OnStep(int step)
{
	if(step != -1) gStep = step;
	return 7;
}

float CloudsExample::Junior::OnTickScroll(Point& actor_pos, Point& actor_vec, const Point* grabbed_cloud, float grabbed_sec)
{
	switch(gStep)
	{
	case 6: return STEP_API_CALL(6, OnTickScroll)(actor_pos, actor_vec, grabbed_cloud, grabbed_sec); break;
	case 5: return STEP_API_CALL(5, OnTickScroll)(actor_pos, actor_vec, grabbed_cloud, grabbed_sec); break;
	case 4: return STEP_API_CALL(4, OnTickScroll)(actor_pos, actor_vec, grabbed_cloud, grabbed_sec); break;
	case 3:
	case 2:
	case 1:
	case 0: return STEP_API_CALL(0, OnTickScroll)(actor_pos, actor_vec, grabbed_cloud, grabbed_sec); break;
	}
	return 0;
}

Rect CloudsExample::Junior::OnAddCloud(float& skywidth)
{
	switch(gStep)
	{
	case 6:
	case 5:
	case 4:
	case 3:
	case 2:
	case 1: return STEP_API_CALL(1, OnAddCloud)(skywidth); break;
	case 0: return STEP_API_CALL(0, OnAddCloud)(skywidth); break;
	}
	return Rect();
}

void CloudsExample::Junior::OnRenderCloud(ZayPanel& panel)
{
	switch(gStep)
	{
	case 6:
	case 5:
	case 4:
	case 3:
	case 2: return STEP_API_CALL(2, OnRenderCloud)(panel); break;
	case 1:
	case 0: return STEP_API_CALL(0, OnRenderCloud)(panel); break;
	}
}

void CloudsExample::Junior::OnRenderActor(ZayPanel& panel, const Point* grabbed_cloud)
{
	switch(gStep)
	{
	case 6:
	case 5:
	case 4:
	case 3: return STEP_API_CALL(3, OnRenderActor)(panel, grabbed_cloud); break;
	case 2:
	case 1:
	case 0: return STEP_API_CALL(0, OnRenderActor)(panel, grabbed_cloud); break;
	}
}
