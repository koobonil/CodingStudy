#pragma once
#include "tiles_example.hpp"

class tilesData : public ZayObject
{
public:
    tilesData();
    ~tilesData();

public:
    sint32 mXCount;
    sint32 mYCount;
    sint32 mTickWait;
    sint32 mTileW;
    sint32 mTileH;
    sint32 mTileX;
    sint32 mTileY;
};
