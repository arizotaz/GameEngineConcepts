#include <iostream>
#include <game/tile.h>

Tile::Tile(int id)
    : id(id)
{
    if (TileList::GetInstance().GAME_TILE_LIST[id] != nullptr)
        throw std::runtime_error(std::string("A tile with an id of ") + std::to_string(id) + std::string(" already exists"));

    std::cout << "[Tile] Registering tile with id " << id << std::endl;
    TileList::GetInstance().GAME_TILE_LIST[id] = this;
}