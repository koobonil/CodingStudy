#include <boss.hpp>
#include "missioncoding_list.hpp"
#include "classes.hpp"

#include <resource.hpp>

ZAY_DECLARE_VIEW_CLASS("listView", listData)

ZAY_VIEW_API OnCommand(CommandType type, chars topic, id_share in, id_cloned_share* out)
{
}

ZAY_VIEW_API OnNotify(chars sender, chars topic, id_share in, id_cloned_share* out)
{
}

ZAY_VIEW_API OnGesture(GestureType type, sint32 x, sint32 y)
{
}

ZAY_VIEW_API OnRender(ZayPanel& panel)
{
    ZAY_RGB(panel, 192, 192, 192)
        panel.fill();

	auto& Data = MissionData::ST();
	ZAY_INNER(panel, 5)
	for(sint32 i = 0, iend = Data.elements().Count(); i < iend; ++i)
	{
		auto CurElement = Data.elements().AccessByOrder(i);
		const String CurButton = String::Format("list_%d", i);
		const String CurPlayButton = String::Format("play_%d", i);

		// 미션버튼
		ZAY_XYWH(panel, 0, i * 40, panel.w(), 40)
		ZAY_INNER_UI(panel, 5, CurButton,
			ZAY_GESTURE_T(t, i)
			{
				if(t == GT_InReleased)
				{
					auto CurElement = MissionData::ST().elements().AccessByOrder(i);
					MissionData::ST().SetFocus(CurElement->mName);
					Platform::UpdateAllViews();
				}
			})
		{
			ZAY_RGB(panel, 255, 255, 255)
			ZAY_RGB_IF(panel, 64, 192, 192, !CurElement->mName.Compare(MissionData::ST().focus()))
			ZAY_RGB_IF(panel, 80, 80, 80, panel.state(CurButton) & PS_Pressed)
				panel.fill();
			ZAY_RGB(panel, 0, 0, 0)
			{
				ZAY_FONT(panel, 1.5)
					panel.text(" " + CurElement->mName, UIFA_LeftMiddle, UIFE_Right);
				panel.rect(1);
			}

			// 플레이버튼
			ZAY_XYWH(panel, panel.w() - 100, 0, 100, panel.h())
			ZAY_INNER_UI(panel, 5, CurPlayButton,
				ZAY_GESTURE_T(t, i)
				{
					if(t == GT_InReleased)
					{
						auto CurElement = MissionData::ST().elements().AccessByOrder(i);
						MissionData::ST().SetFocus(CurElement->mName);
						Platform::UpdateAllViews();
						m->next(CurElement->mView);
					}
				})
			{
				ZAY_RGB(panel, 255, 192, 0)
				ZAY_RGB_IF(panel, 80, 80, 80, panel.state(CurPlayButton) & PS_Pressed)
					panel.fill();
				ZAY_RGB(panel, 0, 0, 0)
				{
					ZAY_FONT(panel, 1.2)
						panel.text("PLAY", UIFA_CenterMiddle, UIFE_Right);
					panel.rect(1);
				}
			}
		}
	}
}

listData::listData()
{
}

listData::~listData()
{
}
