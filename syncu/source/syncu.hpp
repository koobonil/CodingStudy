#pragma once
#include <service/boss_zay.hpp>

class syncuData : public ZayObject
{
public:
    syncuData();
    ~syncuData();

public:
    sint32 mSelectID;
    float mSHScroll[10];
    float mSHScrollPhy[10];
    sint32 mSHScrollValue[10];
};

class syncuObject : public ZayObject
{
public:
    syncuObject();
    virtual ~syncuObject();

public:
    void RenderUI(ZayPanel& panel);
    void SendEvent();

public:
    id_socket mSocket;
    String mSHCode;
    sint32 mLastIDX;
    Context mEvent;
};
