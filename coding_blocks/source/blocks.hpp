#pragma once
#include <service/boss_zay.hpp>

String Setting(int& xcount, int& ycount);
void Click(int& id);
void Render(ZayPanel& panel, int id);

class blocksData : public ZayObject
{
public:
    blocksData();
    ~blocksData();

public:
    String OnSetting(sint32& xcount, sint32& ycount);
    void OnClick(sint32& id);
    void OnRender(ZayPanel& panel, sint32 id);

public:
    sint32 mXCount;
    sint32 mYCount;
    sint32s mBlocks;
};
