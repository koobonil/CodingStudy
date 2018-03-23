#pragma once
#include <service/boss_zay.hpp>
#include "syncu.hpp"

class syncu_touchpadData : public syncuObject
{
public:
    syncu_touchpadData();
    ~syncu_touchpadData();

public:
    void OnPressed();
    void OnDragging();
    void OnReleased();

public:
    sint32 mScreenWidth;
    sint32 mScreenHeight;
    bool mPressed;
    point64 mCursorPos;
};
