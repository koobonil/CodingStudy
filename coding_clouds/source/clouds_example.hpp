#pragma once
#include <boss.hpp>
#include <service/boss_zay.hpp>
#include "../../missioncoding/source/classes.hpp"

#define MISSION_NAME "07.Clouds"

class CloudsExample : public Example
{
private:
	static float OnTickScroll(Point& actor_pos, Point& actor_vec, const Point* grabbed_cloud, float grabbed_sec);
	static Rect OnAddCloud(float& skywidth);
	static void OnRenderCloud(ZayPanel& panel);
	static void OnRenderActor(ZayPanel& panel, const Point* grabbed_cloud);
	class Beginner
	{
	public:
		static float OnTickScroll(Point& actor_pos, Point& actor_vec, const Point* grabbed_cloud, float grabbed_sec);
		static Rect OnAddCloud(float& skywidth);
	};
	class Junior
	{
	public:
		STEP_DECLARE
		static float OnTickScroll(Point& actor_pos, Point& actor_vec, const Point* grabbed_cloud, float grabbed_sec);
		static Rect OnAddCloud(float& skywidth);
		static void OnRenderCloud(ZayPanel& panel);
		static void OnRenderActor(ZayPanel& panel, const Point* grabbed_cloud);
	};
	class Senior
	{
	public:
		static float OnTickScroll(Point& actor_pos, Point& actor_vec, const Point* grabbed_cloud, float grabbed_sec);
		static Rect OnAddCloud(float& skywidth);
		static void OnRenderCloud(ZayPanel& panel);
		static void OnRenderActor(ZayPanel& panel, const Point* grabbed_cloud);
	};

public:
	CloudsExample()
	{
		mTickScroll = nullptr;
		mAddCloud = nullptr;
		mRenderCloud = nullptr;
		mRenderActor = nullptr;
	}
	~CloudsExample()
	{
	}

private:
	void SetLevelCore(int level) override
	{
		switch(level)
		{
		case 0:
			mTickScroll = OnTickScroll;
			mAddCloud = OnAddCloud;
			mRenderCloud = OnRenderCloud;
			mRenderActor = OnRenderActor;
			break;
		case 1:
			mTickScroll = Beginner::OnTickScroll;
			mAddCloud = Beginner::OnAddCloud;
			mRenderCloud = nullptr;
			mRenderActor = nullptr;
			break;
		case 2:
			STEP_SET(Junior)
			mTickScroll = Junior::OnTickScroll;
			mAddCloud = Junior::OnAddCloud;
			mRenderCloud = Junior::OnRenderCloud;
			mRenderActor = Junior::OnRenderActor;
			break;
		case 3:
			mTickScroll = Senior::OnTickScroll;
			mAddCloud = Senior::OnAddCloud;
			mRenderCloud = Senior::OnRenderCloud;
			mRenderActor = Senior::OnRenderActor;
			break;
		}
	}

public:
	float (*mTickScroll)(Point& actor_pos, Point& actor_vec, const Point* grabbed_cloud, float grabbed_sec);
	Rect (*mAddCloud)(float& skywidth);
	void (*mRenderCloud)(ZayPanel& panel);
	void (*mRenderActor)(ZayPanel& panel, const Point* grabbed_cloud);
};
