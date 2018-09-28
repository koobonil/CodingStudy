#include <boss.hpp>
#include "eventtool.hpp"
MISSION_VIEW_DECLARE(MISSION_NAME, "eventtoolView", EventToolExample)

#include <resource.hpp>
ZAY_DECLARE_VIEW_CLASS("eventtoolView", eventtoolData)

ZAY_VIEW_API OnCommand(CommandType type, chars topic, id_share in, id_cloned_share* out)
{
    if(type == CT_Signal)
    {
        sint32 KeyType = 0;
		branch;
		jump(!String::Compare(topic, "KeyPress")) KeyType = 1;
		jump(!String::Compare(topic, "KeyRelease")) KeyType = 2;
        if(0 < KeyType)
        {
            pointers KeyPtr(in);
            sint32 KeyCode = *((sint32*) KeyPtr[0]);
		    if(KeyType == 1)
            {
                if(m->mKeyCode != KeyCode)
                    m->mKeyCode = KeyCode;
            }
            else if(KeyType == 2)
            {
                if(m->mKeyCode == KeyCode)
                {
                    m->mKeyCode = 0;
                    m->mTickMsec = 0;
                }
            }
        }
    }
    else if(type == CT_Tick)
    {
        if(m->mKeyCode != 0)
        if(m->mTickMsec + 100 < Platform::Utility::CurrentTimeMsec())
        {
            m->mTickMsec = Platform::Utility::CurrentTimeMsec();
		    switch(m->mKeyCode)
		    {
            case 0x01000012: exam->mKey(Key::LEFT, m->mDoc); break; // Left
		    case 0x01000013: exam->mKey(Key::UP, m->mDoc); break; // Up
		    case 0x01000014: exam->mKey(Key::RIGHT, m->mDoc); break; // Right
		    case 0x01000015: exam->mKey(Key::DOWN, m->mDoc); break; // Down
		    }
            m->invalidate();
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
    ZAY_RGB(panel, 128, 128, 128)
        panel.fill();

    exam->mRender(panel, m->mDoc);

	MissionCollector::RenderUI(panel);
}

eventtoolData::eventtoolData()
{
    mKeyCode = 0;
    mTickMsec = 0;
    exam->mInit(mDoc);
}

eventtoolData::~eventtoolData()
{
    exam->mQuit(mDoc);
}
