#include <boss.hpp>
#include "polygons_example.hpp"
#include <resource.hpp>

MISSION_JUNIOR_DECLARE(MISSION_NAME, 0, "STEP_0")

int PolygonsExample::Junior::OnAdd(float& x, float& y)
{
    x = (Platform::Utility::Random() % 100) - 50.0f;
    y = (Platform::Utility::Random() % 100) - 50.0f;
    return 20;
}

int PolygonsExample::Junior::OnAct(sint32& type, float& vx, float& vy)
{
    if(type == 0)
    {
        if(Platform::Utility::Random() % 100 < 5)
            type = 2;
        else type = 1;
    }
    vx = ((Platform::Utility::Random() % 100) - 50.0f) * 0.03f;
    vy = ((Platform::Utility::Random() % 100) - 50.0f) * 0.03f;
    if(type == 1)
        return 500 + (Platform::Utility::Random() % 500);
    else if(type == 2)
        return 20 + (Platform::Utility::Random() % 20);
    return 0;
}

int PolygonsExample::Junior::OnScore(sint32 type)
{
    if(type == 1)
        return 100;
    if(type == 2)
        return -500;
    return 0;
}

void PolygonsExample::Junior::OnRender(ZayPanel& panel, sint32 type, sint32 ani)
{
    ZAY_XYRR(panel, 0, 0, 10, 10)
    {
        if(type == 1)
        {
            ZAY_RGBA(panel, 128, 0, 128, 224)
            ZAY_RGBA_IF(panel, 224, 224, 224, 224, 0 < ani)
                panel.circle();
        }
        else
        {
            ZAY_RGBA(panel, 224, 255, 0, 224)
            ZAY_RGBA_IF(panel, 224, 224, 224, 224, 0 < ani)
                panel.fill();
        }
    }

    if(0 < ani)
    {
        const float BombSize = Math::Pow(10 - ani);
        const sint32 BombOpacity = 255 * ani / 10;
        ZAY_XYRR(panel, 0, 0, BombSize, BombSize)
        ZAY_RGBA(panel, 255, 0, 0, BombOpacity)
            panel.circle();
    }
}
