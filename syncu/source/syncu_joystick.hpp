#pragma once
#include <service/boss_zay.hpp>
#include "syncu.hpp"

class syncu_joystickData : public syncuObject
{
public:
    syncu_joystickData();
    ~syncu_joystickData();

public:
    void OnDegreePressed(sint32 degree);
    void OnDegreeReleased();
    void OnButtonPressed(sint32 button);
    void OnButtonReleased();

public:
    sint32 mDegree;
    sint32 mButton;
};
