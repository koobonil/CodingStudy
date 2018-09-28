#include <boss.hpp>
#include "eventtool_example.hpp"
#include <resource.hpp>

////////////////////////////////////////////////////////////////////////////////
#define LEVEL_NUMBER LEVEL_BEGINNER

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 0
MISSION_DECLARE("STEP_0")

static Image Tile[100];
static Context MapData;

void EventToolExample::Beginner::OnInit(Context& doc)
{
	for(int i = 0; i < 100; ++i)
	{
		String Name = String::Format("tile/T-%02d.bmp", i);
		Tile[i].SetName(Name).Load();
	}

	String Map = String::FromAsset("mapdata.json");
	MapData.LoadJson(SO_NeedCopy, Map);

    String Event = String::FromAsset("eventdata.json");
	doc.LoadJson(SO_NeedCopy, Event);
}

void EventToolExample::Beginner::OnQuit(Context& doc)
{
    doc.SaveJson().ToAsset("eventdata.json");
}

void EventToolExample::Beginner::OnKey(Key key, Context& doc)
{
    int Focus = doc("focus").GetInt();
    if(key == Key::LEFT)
        doc.At("focus").Set(String::FromInteger(Math::Max(0, Focus - 1)));
    else if(key == Key::RIGHT)
        doc.At("focus").Set(String::FromInteger(Math::Min(Focus + 1, 60 - 1)));
}

static void RenderTimeline(ZayPanel& panel, Context& doc)
{
	ZAY_RGB(panel, 128, 192, 255)
		panel.fill();
	ZAY_RGB(panel, 0, 0, 0)
		panel.rect(1);

    int Focus = doc("focus").GetInt();

    for(int y = 0; y < 5; ++y)
    {
        bool IsShow = false;
        for(int x = 0; x < 60; ++x)
        {
            String UIName = String::Format("Timeline-%d-%d", x, y);
            ZAY_LTRB_UI(panel, panel.w() * x / 60, panel.h() * y / 5, panel.w() * (x + 1) / 60, panel.h() * (y + 1) / 5, UIName,
                ZAY_GESTURE_T(t, x, y, &doc)
                {
                    if(t == GT_Pressed)
                    {
                        int Spot = doc("key")[x][y]("spot").GetInt();
                        doc.At("key").At(x).At(y).At("spot").Set(String::FromInteger((Spot + 1) % 3));
                    }
                })
            {
                // 보여짐상태
                int Spot = doc("key")[x][y]("spot").GetInt();
                if(Spot == 1)
                    IsShow = true;
                else if(Spot == 2)
                    IsShow = false;

                // 셀의 색상
                ZAY_RGBA(panel, 0, 0, 0, ((x + y) % 2)? 64 : 48)
                ZAY_RGBA_IF(panel, 255, 0, 0, 128, IsShow)
                ZAY_RGBA_IF(panel, 224, 224, 128, 192, x == Focus) // 포커싱
		            panel.fill();

                // 스팟
                ZAY_XYRR(panel, panel.w() / 2, panel.h() / 2, 2, 2)
                ZAY_RGB(panel, 128, 0, 128)
                {
                    if(Spot == 1)
                        panel.fill();
                    else if(Spot == 2)
                        panel.rect(1);
                }
            }
        }
    }
}

static void RenderObject(ZayPanel& panel, Context& doc)
{
    int Focus = doc("focus").GetInt();
    for(int y = 0; y < 5; ++y)
    {
        int LastSpot = -1;
        int FocusPrev = -1;
        int FocusNext = -1;
        for(int x = 0; x < 60; ++x)
        {
            // 마지막 보여짐상태
            int Spot = doc("key")[x][y]("spot").GetInt();
            if(Spot == 1) LastSpot = x;
            else if(Spot == 2) LastSpot = -1;

            // 전후스팟을 기록
            if(x == Focus)
                FocusPrev = LastSpot;
            else if(Focus < x)
            {
                if(FocusNext == -1 && Spot != 0)
                {
                    if(Spot == 1)
                        FocusNext = x;
                    break;
                }
            }
        }

        // 오브젝트
        if(FocusPrev != -1)
        {
            int XPos = 0, YPos = 0;
            // 고정위치
            if(FocusNext == -1)
            {
                XPos = doc("key")[FocusPrev][y]("x").GetInt();
                YPos = doc("key")[FocusPrev][y]("y").GetInt();
            }
            // 변동위치
            else
            {
                int XPos1 = doc("key")[FocusPrev][y]("x").GetInt();
                int YPos1 = doc("key")[FocusPrev][y]("y").GetInt();
                int XPos2 = doc("key")[FocusNext][y]("x").GetInt();
                int YPos2 = doc("key")[FocusNext][y]("y").GetInt();
                XPos = (XPos1 * (FocusNext - Focus) + XPos2 * (Focus - FocusPrev)) / (FocusNext - FocusPrev);
                YPos = (YPos1 * (FocusNext - Focus) + YPos2 * (Focus - FocusPrev)) / (FocusNext - FocusPrev);
            }
            // 출력
            bool UIEnable = (doc("key")[Focus][y]("spot").GetInt() == 1);
            String UIName = String::Format("Object-%d", y);
            ZAY_XYRR_UI(panel, panel.w() / 2 + XPos, panel.h() / 2 + YPos, 20, 20, (UIEnable)? UIName : String(""),
                ZAY_GESTURE_VNTXY(v, n, t, xpos, ypos, &doc, Focus, y)
                {
                    if(t == GT_InDragging || t == GT_OutDragging)
                    {
                        point64 OldXY = v->oldxy(n);
                        int XMove = xpos - OldXY.x;
                        int YMove = ypos - OldXY.y;
                        int XPos = doc("key")[Focus][y]("x").GetInt();
                        int YPos = doc("key")[Focus][y]("y").GetInt();
                        doc.At("key").At(Focus).At(y).At("x").Set(String::FromInteger(XPos + XMove));
                        doc.At("key").At(Focus).At(y).At("y").Set(String::FromInteger(YPos + YMove));
                        v->invalidate();
                    }
                })
            {
                ZAY_RGBA_IF(panel, 128, 192, 255, 128, UIEnable)
                ZAY_RGBA_IF(panel, 192, 192, 192, 128, !UIEnable)
                    panel.circle();
                ZAY_RGB(panel, 0, 0, 0)
                ZAY_FONT(panel, 1.5)
                    panel.text(String::Format("%d", y + 1), UIFA_CenterMiddle);
            }
        }
    }
}

static void RenderMap(ZayPanel& panel, Context& doc)
{
	int TileSize = 40;
	int XBegin = (panel.w() - TileSize * 10) / 2;
	int YBegin = (panel.h() - TileSize * 10) / 2;

	for(int y = 0; y < 10; ++y)
	for(int x = 0; x < 10; ++x)
	{
		ZAY_XYWH(panel, XBegin + TileSize * x, YBegin + TileSize * y, TileSize, TileSize)
		{
			int CurTile = MapData("tile")[x][y].GetInt(0);
			panel.stretch(Tile[CurTile], true);
		}
	}

	ZAY_XYWH(panel, XBegin, YBegin, TileSize * 10, TileSize * 10)
    {
        ZAY_RGB(panel, 0, 0, 0)
		    panel.rect(1);
        RenderObject(panel, doc);
    }
}

void EventToolExample::Beginner::OnRender(ZayPanel& panel, Context& doc)
{
	ZAY_LTRB(panel, 0, 0, panel.w(), 120)
	ZAY_INNER(panel, 10)
		RenderTimeline(panel, doc);

	ZAY_LTRB(panel, 0, 110, panel.w(), panel.h())
        RenderMap(panel, doc);
}
