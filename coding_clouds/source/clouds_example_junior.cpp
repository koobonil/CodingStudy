#include <boss.hpp>
#include "clouds_example.hpp"
#include <resource.hpp>

////////////////////////////////////////////////////////////////////////////////
MISSION_JUNIOR_DECLARE(MISSION_NAME, 0, "모든 요소를 준비")

static float STEP_FUNC(0, OnTickScroll)(Point& actor_pos, Point& actor_vec, const Point* grabbed_cloud, float grabbed_sec)
{
	if(grabbed_cloud)
	{
		actor_vec.x = actor_vec.x * 0.95 + grabbed_cloud->x * 0.05;
		actor_vec.y = actor_vec.y * 0.95 + grabbed_cloud->y * 0.05;
	}

	actor_vec.y += 0.1;
	return 0.002;
}

static Rect STEP_FUNC(0, OnAddCloud)(float& skywidth)
{
	return Rect(0.4, 0.4, 0.6, 0.6);
}

static void STEP_FUNC(0, OnRenderCloud)(ZayPanel& panel)
{
	ZAY_RGB(panel, 128, 128, 128)
		panel.circle();
}

static void STEP_FUNC(0, OnRenderActor)(ZayPanel& panel, const Point* grabbed_cloud)
{
	ZAY_RGB(panel, 0, 0, 255)
		panel.circle();
}

////////////////////////////////////////////////////////////////////////////////
MISSION_JUNIOR_DECLARE(MISSION_NAME, 1, "구름의 타입을 4개로")

static Rect STEP_FUNC(1, OnAddCloud)(float& skywidth)
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
MISSION_JUNIOR_DECLARE(MISSION_NAME, 2, "구름을 이쁘게")

static void STEP_FUNC(2, OnRenderCloud)(ZayPanel& panel)
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
MISSION_JUNIOR_DECLARE(MISSION_NAME, 3, "구름줄을 표현")

static void STEP_FUNC(3, OnRenderActor)(ZayPanel& panel, const Point* grabbed_cloud)
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
MISSION_JUNIOR_DECLARE(MISSION_NAME, 4, "구름을 잡아보자")

static bool NeedCalc = true;
static float FirstDistance = 0;

static float STEP_FUNC(4, OnTickScroll)(Point& actor_pos, Point& actor_vec, const Point* grabbed_cloud, float grabbed_sec)
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
MISSION_JUNIOR_DECLARE(MISSION_NAME, 5, "흔들흔들")

static Point OldActorPos;

static float STEP_FUNC(5, OnTickScroll)(Point& actor_pos, Point& actor_vec, const Point* grabbed_cloud, float grabbed_sec)
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
MISSION_JUNIOR_DECLARE(MISSION_NAME, 6, "2초만에 구름까지")

static float STEP_FUNC(6, OnTickScroll)(Point& actor_pos, Point& actor_vec, const Point* grabbed_cloud, float grabbed_sec)
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
	case 6: return OnTickScroll_STEP6(actor_pos, actor_vec, grabbed_cloud, grabbed_sec); break;
	case 5: return OnTickScroll_STEP5(actor_pos, actor_vec, grabbed_cloud, grabbed_sec); break;
	case 4: return OnTickScroll_STEP4(actor_pos, actor_vec, grabbed_cloud, grabbed_sec); break;
	case 3:
	case 2:
	case 1:
	case 0: return OnTickScroll_STEP0(actor_pos, actor_vec, grabbed_cloud, grabbed_sec); break;
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
	case 1: return OnAddCloud_STEP1(skywidth); break;
	case 0: return OnAddCloud_STEP0(skywidth); break;
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
	case 2: return OnRenderCloud_STEP2(panel); break;
	case 1:
	case 0: return OnRenderCloud_STEP0(panel); break;
	}
}

void CloudsExample::Junior::OnRenderActor(ZayPanel& panel, const Point* grabbed_cloud)
{
	switch(gStep)
	{
	case 6:
	case 5:
	case 4:
	case 3: return OnRenderActor_STEP3(panel, grabbed_cloud); break;
	case 2:
	case 1:
	case 0: return OnRenderActor_STEP0(panel, grabbed_cloud); break;
	}
}
