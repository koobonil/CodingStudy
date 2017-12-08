#pragma once
#include <service/boss_zay.hpp>

class Ball
{
    BOSS_DECLARE_NONCOPYABLE_CLASS(Ball)
public:
    Ball() {time = 0; sizeR = 0;}
    ~Ball() {}
    Ball(Ball&& rhs) {operator=(ToReference(rhs));}
    Ball& operator=(Ball&& rhs)
    {
        time = rhs.time;
        sizeR = rhs.sizeR;
        pos = rhs.pos;
        vec = rhs.vec;
        return *this;
    }

public:
    uint64 time;
    float sizeR;
    Point pos;
    Point vec;
};

class ballsData : public ZayObject
{
public:
    ballsData() {}
    ~ballsData() {}

public:
    enum class Wall {LEFT, TOP, RIGHT, BOTTOM};

public:
    void OnTick(float sec, Point& pos, Point& vec);
    bool OnCrashWall(Wall wall, Point& vec);
    bool OnCrashBall(Point& vecA, Point& vecB);

public:
    Array<Ball> balls;
};
