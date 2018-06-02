#include <boss.hpp>
#include "cars.hpp"
MISSION_VIEW_DECLARE(MISSION_NAME, "carsView", CarsExample)

#include <resource.hpp>
ZAY_DECLARE_VIEW_CLASS("carsView", carsData)

ZAY_VIEW_API OnCommand(CommandType type, chars topic, id_share in, id_cloned_share* out)
{
	if(type == CT_Size)
	{
		sint32s WH = in;
		m->Resize(WH[0], WH[1]);
	}
	else if(type == CT_Tick)
	{
		m->CheckMessage();
		m->invalidate();

		for(sint32 i = 0, iend = m->mCars.Count(); i < iend; ++i)
		{
			auto& CurCar = *m->mCars.AccessByOrder(i);

			// 입력처리
			if(CurCar.mDegree != -1)
			{
				if(CurCar.mDegree < 90) exam->mInput(Joystick::ARROW_U, CurCar.mVec, CurCar.mAngle360);
				else if(CurCar.mDegree < 180) exam->mInput(Joystick::ARROW_R, CurCar.mVec, CurCar.mAngle360);
				else if(CurCar.mDegree < 270) exam->mInput(Joystick::ARROW_D, CurCar.mVec, CurCar.mAngle360);
				else exam->mInput(Joystick::ARROW_L, CurCar.mVec, CurCar.mAngle360);
			}
			if(CurCar.mButton1Once) exam->mInput(Joystick::BUTTON1, CurCar.mVec, CurCar.mAngle360);
			if(CurCar.mButton2Once) exam->mInput(Joystick::BUTTON2, CurCar.mVec, CurCar.mAngle360);

			const sint32 OldTileX = Math::Clamp(CurCar.mPos.x, 0, m->mXCount - 1);
			const sint32 OldTileY = Math::Clamp(CurCar.mPos.y, 0, m->mYCount - 1);
			const char OldTile = m->mTiles[OldTileX + OldTileY * m->mXCount];

			// 애니메이션처리
			CurCar.AnimationOnce();
			CurCar.mVec.x = CurCar.mVec.x * Math::ClampF(1 - m->mFriction, 0, 1);
			CurCar.mVec.y = CurCar.mVec.y * Math::ClampF(1 - m->mFriction, 0, 1);

			const sint32 NewTileX = Math::Clamp(CurCar.mPos.x, 0, m->mXCount - 1);
			const sint32 NewTileY = Math::Clamp(CurCar.mPos.y, 0, m->mYCount - 1);
			const char NewTile = m->mTiles[NewTileX + NewTileY * m->mXCount];

			// 벽충돌
			if(OldTile != NewTile)
			{
				const char NewTileL = (0 < NewTileX)? m->mTiles[(NewTileX - 1) + NewTileY * m->mXCount] : NewTile;
				const char NewTileT = (0 < NewTileY)? m->mTiles[NewTileX + (NewTileY - 1) * m->mXCount] : NewTile;
				const char NewTileR = (NewTileX < m->mXCount - 1)? m->mTiles[(NewTileX + 1) + NewTileY * m->mXCount] : NewTile;
				const char NewTileB = (NewTileY < m->mYCount - 1)? m->mTiles[NewTileX + (NewTileY + 1) * m->mXCount] : NewTile;
				branch;
				jump(OldTileX > NewTileX && NewTile != NewTileR)
					exam->mCrashTile(NewTile, Wall::LEFT, CurCar.mVec);
				jump(OldTileY > NewTileY && NewTile != NewTileB)
					exam->mCrashTile(NewTile, Wall::TOP, CurCar.mVec);
				jump(OldTileX < NewTileX && NewTile != NewTileL)
					exam->mCrashTile(NewTile, Wall::RIGHT, CurCar.mVec);
				jump(OldTileY < NewTileY && NewTile != NewTileT)
					exam->mCrashTile(NewTile, Wall::BOTTOM, CurCar.mVec);
			}
		}

		// 자동차충돌
		for(sint32 i = 0, iend = m->mCars.Count(); i < iend; ++i)
		{
			auto& CurCar = *m->mCars.AccessByOrder(i);
			for(sint32 j = i + 1; j < iend; ++j)
			{
				auto& OtherCar = *m->mCars.AccessByOrder(j);
				if(Math::Distance(CurCar.mPos.x, CurCar.mPos.y, OtherCar.mPos.x, OtherCar.mPos.y) < m->mCarSize)
				{
					exam->mCrashCar(OtherCar.mPos - CurCar.mPos, CurCar.mVec);
					exam->mCrashCar(CurCar.mPos - OtherCar.mPos, OtherCar.mVec);
				}
			}
		}
	}
	else if(type == CT_Signal)
	{
		sint32 KeyType = 0;
		branch;
		jump(!String::Compare(topic, "KeyPress")) KeyType = 1;
		jump(!String::Compare(topic, "KeyRelease")) KeyType = 2;
		if(0 < KeyType)
		{
			auto& CurCar = m->mCars("MY");
			pointers Key(in);
			sint32 KeyCode = *((sint32*) Key[0]);
			if(KeyType == 2)
			{
				CurCar.mKeyPressMap.Remove(KeyCode);
				if(CurCar.mKeyPressMap.Count() == 0)
					CurCar.mDegreeCancelOnce = true;
			}
			else if(KeyCode == ' ') // Space
			{
				CurCar.mFart = 0;
				CurCar.mButton1Once = true;
			}
			else if(KeyCode == 0x01000004) // Enter
				CurCar.mButton2Once = true;
			else
			{
				switch(KeyCode)
				{
				case 'A': CurCar.mDegree = 315; break; // Left
				case 'W': CurCar.mDegree = 45; break; // Up
				case 'D': CurCar.mDegree = 135; break; // Right
				case 'S': CurCar.mDegree = 225; break; // Down
				case 0x01000012: CurCar.mDegree = 315; break; // Left
				case 0x01000013: CurCar.mDegree = 45; break; // Up
				case 0x01000014: CurCar.mDegree = 135; break; // Right
				case 0x01000015: CurCar.mDegree = 225; break; // Down
				default: return;
				}
				CurCar.mDegreeCancelOnce = false;
				CurCar.mKeyPressMap[KeyCode] = KeyCode;
			}
		}
	}
}

ZAY_VIEW_API OnNotify(chars sender, chars topic, id_share in, id_cloned_share* out)
{
}

ZAY_VIEW_API OnGesture(GestureType type, sint32 x, sint32 y)
{
}

ZAY_VIEW_API OnRender(ZayPanel& panel)
{
    ZAY_RGB(panel, 185, 122, 87)
        panel.fill();

	// 타일
	for(sint32 y = 0; y < m->mYCount; ++y)
    for(sint32 x = 0; x < m->mXCount; ++x)
    {
        const sint32 i = x + y * m->mXCount;
        ZAY_LTRB(panel, panel.w() * x / m->mXCount, panel.h() * y / m->mYCount,
            panel.w() * (x + 1) / m->mXCount, panel.h() * (y + 1) / m->mYCount)
        {
            char CurTile = m->mTiles[i];
            exam->mRenderTile(panel, CurTile);
        }
    }

	// 자동차
    for(sint32 i = 0, iend = m->mCars.Count(); i < iend; ++i)
    {
        auto& CurCar = *m->mCars.AccessByOrder(i);
        ZAY_XYRR(panel, panel.w() * CurCar.mPos.x / m->mXCount, panel.h() * CurCar.mPos.y / m->mYCount,
			panel.w() * m->mCarSize / 2 / m->mXCount, panel.h() * m->mCarSize / 2 / m->mYCount)
		{
			exam->mRenderCar(panel, CurCar.mAngle360, CurCar.mFart);
			if(CurCar.mFart != -1)
				CurCar.mFart++;
		}
    }

	m->RenderUI(panel);
	MissionCollector::RenderUI(panel);
}

carsData::carsData() : SyncuClient("cars")
{
	mXCount = 1;
    mYCount = 1;
	mCarSize = 1;
	mFriction = 0;
    const String NewTiles = exam->mInit(mXCount, mYCount, mCarSize, mFriction);
	char CurTile[2] = {' ', '\0'};
	for(sint32 i = 0, iend = mXCount * mYCount; i < iend; ++i)
	{
		if(i < NewTiles.Length())
			CurTile[0] = NewTiles[i];
		mTiles.Add(CurTile);
	}
	OnVisit("MY");
}

carsData::~carsData()
{
}

void carsData::OnVisit(chars id)
{
	auto& CurCar = mCars(id);
	exam->mCreateCar(CurCar.mPos, CurCar.mAngle360);
}

void carsData::OnLeave(chars id)
{
	mCars.Remove(id);
}

void carsData::OnEvent(chars id, const Context& json)
{
	auto& CurCar = mCars(id);
	chars PayloadType = json("type").GetString();
	if(!String::Compare(PayloadType, "joystick"))
	{
		const sint32 DegreeCode = json("degree").GetInt(-2);
		if(0 <= DegreeCode)
		{
			CurCar.mDegree = DegreeCode;
			CurCar.mDegreeCancelOnce = false;
		}
		else if(DegreeCode == -1)
			CurCar.mDegreeCancelOnce = true;

		const sint32 ButtonCode = json("button").GetInt(-2);
		if(ButtonCode == 3)
		{
			CurCar.mFart = 0;
			CurCar.mButton1Once = true;
		}
		else if(ButtonCode == 1)
			CurCar.mButton2Once = true;
	}
}
