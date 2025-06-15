#pragma once

enum TileType { GRASS, SPIKE };
class Tile {
public:
    virtual TileType getType() = 0;
};

class Grass :public Tile {
    TileType getType();
};
class Spike :public Tile {
    TileType getType();
};


