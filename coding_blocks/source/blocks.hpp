#pragma once
#include "blocks_example.hpp"

class blocksData : public ZayObject
{
public:
    blocksData();
    ~blocksData();

public:
    sint32 mXCount;
    sint32 mYCount;
    sint32s mBlocks;
};
