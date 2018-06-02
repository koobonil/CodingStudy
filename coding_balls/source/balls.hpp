#pragma once
#include "balls_example.hpp"

class Ball
{
    BOSS_DECLARE_NONCOPYABLE_CLASS(Ball)
public:
    Ball() {mTime = 0; mSizeR = 0;}
    ~Ball() {}
    Ball(Ball&& rhs) {operator=(ToReference(rhs));}
    Ball& operator=(Ball&& rhs)
    {
        mTime = rhs.mTime;
        mSizeR = rhs.mSizeR;
        mColor = rhs.mColor;
        mPos = rhs.mPos;
        mVec = rhs.mVec;
        return *this;
    }

public:
    uint64 mTime;
    float mSizeR;
    Color mColor;
    Point mPos;
    Point mVec;
};

class ballsData : public ZayObject
{
public:
    ballsData();
    ~ballsData();

public:
    Array<Ball> mBalls;
};
