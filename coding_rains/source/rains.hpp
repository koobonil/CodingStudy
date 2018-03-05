#pragma once
#include <service/boss_zay.hpp>
#include <element/boss_vector.hpp>

void RenderBowl(ZayPanel& panel);
int TestRain(int x1, int y1, int x2, int y2);

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
    void OnRenderBowl(ZayPanel& panel);
    int OnTestRain(int x1, int y1, int x2, int y2);

public:
    enum {mRainCount = 256};
    RainElement mRains[mRainCount];
    Size mScreenSize;
    Point mTouchPos;
};
