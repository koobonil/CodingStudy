#include <boss.hpp>
#include "missioncoding_level.hpp"
#include "classes.hpp"

#include <resource.hpp>

ZAY_DECLARE_VIEW_CLASS("levelView", levelData)

ZAY_VIEW_API OnCommand(CommandType type, chars topic, id_share in, id_cloned_share* out)
{
}

ZAY_VIEW_API OnNotify(NotifyType type, chars topic, id_share in, id_cloned_share* out)
{
}

ZAY_VIEW_API OnGesture(GestureType type, sint32 x, sint32 y)
{
}

ZAY_VIEW_API OnRender(ZayPanel& panel)
{
    ZAY_RGB(panel, 192, 192, 192)
        panel.fill();

	if(auto CurElement = MissionCollector::FocusedMission())
	ZAY_INNER(panel, 5)
	{
		chars Symbols[4] = {"U", "B", "J", "S"};
		for(sint32 i = 0; i < 4; ++i)
		{
			ZAY_LTRB(panel, panel.w() * i / 4, 0, panel.w() * (i + 1) / 4, 50)
			ZAY_INNER_UI(panel, 5, String::Format("Level_%d", i),
				ZAY_GESTURE_T(t, i, CurElement)
				{
					if(t == GT_Pressed)
					{
						CurElement->SetLevel(i);
						Platform::UpdateAllViews();
					}
				})
			ZAY_COLOR(panel, (i == CurElement->mExampleLevel)? Color(255, 128, 0) : Color(0, 0, 0))
			{
				ZAY_RGBA(panel, 128, 128, 128, 64)
					panel.fill();
				ZAY_RGB(panel, 0, 0, 0)
					panel.text(Symbols[i], UIFA_CenterMiddle, UIFE_Right);
				panel.rect(2);
			}
		}

		ZAY_LTRB(panel, 0, 50, panel.w(), panel.h())
		ZAY_INNER_SCISSOR(panel, 5)
		{
			ZAY_RGBA(panel, 0, 0, 0, 32)
				panel.fill();
			ZAY_INNER(panel, 5)
			for(sint32 i = 0, iend = CurElement->mExampleStepCount; i < iend; ++i)
			{
				ZAY_LTRB(panel, 0, 40 * i, panel.w(), 40 * (i + 1))
				ZAY_INNER_UI(panel, 5, String::Format("Step_%d", i),
					ZAY_GESTURE_T(t, i, CurElement)
					{
						if(t == GT_Pressed)
						{
							CurElement->SetStep(i);
							Platform::UpdateAllViews();
						}
					})
				ZAY_COLOR(panel, (i == CurElement->mExampleStep)? Color(255, 128, 0) : Color(0, 0, 0))
				{
					ZAY_RGBA(panel, 128, 128, 128, 64)
						panel.fill();
					ZAY_RGB(panel, 0, 0, 0)
						panel.text(CurElement->mComments[CurElement->mExampleLevel].AtWherever(i).mSubject, UIFA_CenterMiddle, UIFE_Right);
					panel.rect(2);
				}
			}
		}
	}
}

levelData::levelData()
{
}

levelData::~levelData()
{
}
