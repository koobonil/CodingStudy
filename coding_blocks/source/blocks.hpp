#pragma once
#include "blocks_example.hpp"

class blocksData : public ZayObject
{
public:
    blocksData();
    ~blocksData();

public:
    sint32 XCount;
    sint32 YCount;
    sint32s Blocks;
};
