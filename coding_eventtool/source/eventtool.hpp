#pragma once
#include "eventtool_example.hpp"

class eventtoolData : public ZayObject
{
public:
    eventtoolData();
    ~eventtoolData();

public:
    Context mDoc;
    sint32 mKeyCode;
    uint64 mTickMsec;
};
