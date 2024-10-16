// include guard
#ifndef MAPGEN_HPP
#define MAPGEN_HPP

#include <cstdlib>
#include "tiles.hpp"
#include "resources.hpp"

namespace MapGen{
    
    void GenerateResources(std::vector<std::vector<std::shared_ptr<Tile>>> &tiles);
    ResourcePtr GenerateResourceFromNeighbors(std::vector<TilePtr> &tiles);
} // namespace MapGen

#endif // MAPGEN_HPP