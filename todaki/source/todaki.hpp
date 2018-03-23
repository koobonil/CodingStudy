#pragma once
#include <service/boss_zay.hpp>
#include <service/boss_backey.hpp>

class word
{
public:
    word() {}
    ~word() {}

public:
    String mKey;
    String mComment;
    float mPosX;
    float mPosY;
    float mSpeedX;
    float mSpeedY;
    int mDeathCount;
};

class todakiData : public ZayObject
{
public:
    todakiData();
    ~todakiData();

public:
    void init();
    void try_flush();

public:
    Backey mBackey;
    Context WordPool;
    sint32 Life;
    sint32 Score;
    String CurText;
    uint64 FirstWordTime;
    uint64 LastWordTime;
    Map<word> Words;
    sint32 Result_HighScore;
    sint32 Result_TotalScore;
};
