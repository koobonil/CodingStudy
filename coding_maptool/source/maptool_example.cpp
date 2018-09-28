#include <boss.hpp>
#include "maptool_example.hpp"
#include <resource.hpp>

////////////////////////////////////////////////////////////////////////////////
#define LEVEL_NUMBER LEVEL_USER

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 0
MISSION_DECLARE("STEP_0")

Image Tile[100];

void MapToolExample::OnInit(Context& doc)
{
	for(int i = 0; i < 100; ++i)
	{
		String Name = String::Format("tile/T-%02d.bmp", i);
		Tile[i].SetName(Name).Load();
	}

	String Json = String::FromAsset("mapdata.json");
	if(Json.Length() == 0)
	{
		for(int y = 0; y < 10; ++y)
		for(int x = 0; x < 10; ++x)
			doc.At("tile").At(x).At(y).Set("0");
	}
	else doc.LoadJson(SO_NeedCopy, Json);
}

void MapToolExample::OnQuit(Context& doc)
{
	doc.SaveJson().ToAsset("mapdata.json");
}

int LastTile = 0;
void RenderMap(ZayPanel& panel, Context& doc)
{
	int TileSize = 50;
	int XBegin = (panel.w() - TileSize * 10) / 2;
	int YBegin = (panel.h() - TileSize * 10) / 2;

	for(int y = 0; y < 10; ++y)
	for(int x = 0; x < 10; ++x)
	{
		String UIName = String::Format("%d-%d", x, y);
		ZAY_XYWH_UI(panel, XBegin + TileSize * x, YBegin + TileSize * y, TileSize, TileSize, UIName,
			ZAY_GESTURE_T(t, &doc, x, y)
			{
				if(t == GT_Pressed || t == GT_Dropping)
					doc.At("tile").At(x).At(y).Set(String::FromInteger(LastTile));
			})
		{
			int CurTile = doc("tile")[x][y].GetInt(0);
			panel.stretch(Tile[CurTile], true);

			ZAY_RGB(panel, 0, 0, 0)
				panel.rect(1);
		}
	}
}

int ToolBarX = 0;
void RenderToolBar(ZayPanel& panel)
{
	int TileSize = panel.h();
	ZAY_INNER_UI(panel, 0, "ToolBar",
		ZAY_GESTURE_VNTXY(v, n, t, x, y, TileSize)
		{
			if(t == GT_Pressed)
			{
				LastTile = (x - ToolBarX) / TileSize;
			}
			else if(t == GT_InDragging || t == GT_OutDragging)
			{
				ToolBarX += (x - v->oldxy(n).x) * 5;
				ToolBarX = Math::Clamp(ToolBarX, TileSize * -100, 0);
				v->invalidate();
			}
		})
	ZAY_RGBA(panel, 255, 0, 0, 128)
		panel.fill();

	for(int i = 0; i < 100; ++i)
	{
		ZAY_XYWH(panel, ToolBarX + TileSize * i, 0, TileSize, TileSize)
		ZAY_INNER(panel, 5)
		{
			ZAY_RGBA(panel, 128, 128, 128, 64)
			ZAY_RGBA_IF(panel, 128, 200, 200, 255, i == LastTile)
				panel.stretch(Tile[i], true);
			ZAY_RGB(panel, 0, 0, 0)
				panel.rect(1);
		}
	}
}

void MapToolExample::OnRender(ZayPanel& panel, Context& doc)
{
	ZAY_LTRB(panel, 0, 0, panel.w(), panel.h() - 60)
		RenderMap(panel, doc);

	ZAY_LTRB(panel, 0, panel.h() - 60, panel.w(), panel.h())
		RenderToolBar(panel);
}
