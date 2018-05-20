#pragma once
#include "rains_example.hpp"
#include <element/boss_vector.hpp>

class RainElement
{
public:
    RainElement() {mVisible = false; mZ = 0;}
    ~RainElement() {}
public:
    bool mVisible;
    Vector mPos;
    float mZ;
};

class rainsData : public ZayObject
{
public:
    rainsData();
    ~rainsData();

public:
    enum {mRainCount = 256};
    RainElement mRains[mRainCount];
    Size mScreenSize;
    Point mTouchPos;
};
