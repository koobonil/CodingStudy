#include <boss.hpp>
#include "bullets_example.hpp"
#include <resource.hpp>

MISSION_JUNIOR_DECLARE(MISSION_NAME, 0, "STEP_0")

int BulletsExample::Junior::OnAdd(float& x, float& y)
{
	static sint32 Count = 0;
	x = (Platform::Utility::Random() % 400) - 200.0f;
    y = (Platform::Utility::Random() % 400) - 200.0f;
    return (++Count < 30)? 20 : -1;
}

int BulletsExample::Junior::OnAct(int& type, float& vx, float& vy)
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

void BulletsExample::Junior::OnRender(ZayPanel& panel, int type)
{
    ZAY_XYRR(panel, 0, 0, 10, 10)
    {
        if(type == 1)
        {
            ZAY_RGBA(panel, 128, 0, 128, 224)
				panel.circle();
        }
        else
        {
            ZAY_RGBA(panel, 224, 255, 0, 224)
				panel.fill();
        }
    }
}

int BulletsExample::Junior::OnRenderUser(ZayPanel& panel, int id, float vx, float vy)
{
	ZAY_RGB_IF(panel, 0, 0, 255, id == 0)
	ZAY_RGB_IF(panel, 255, 0, 0, id != 0)
	ZAY_XYRR(panel, 0, 0, 20, 20)
        panel.circle();
	return 20;
}
