#include "mapgen.hpp"


namespace MapGen{
    double val1 = 500;
    // define map gen probability constants per resource
    std::map<ResourceTypes, double> resource_probabilities = {
        {ResourceTypes::LUMBER, 1},
        {ResourceTypes::STONE, 1},
        // {ResourceTypes::WATER, 0.1},
        {ResourceTypes::GRASS, 1000},
        {ResourceTypes::CORNSTALK, 1},
    };
    std::map<ResourceTypes, double> resource_neighbor_probabilities = {
        {ResourceTypes::LUMBER, val1},
        {ResourceTypes::STONE, val1},
        // {ResourceTypes::WATER, 0.2},
        {ResourceTypes::GRASS, 1},
        {ResourceTypes::CORNSTALK, val1},
    };
    // amount to generate per resource type
    std::map<ResourceTypes, double> resource_amounts = {
        {ResourceTypes::LUMBER, 20},
        {ResourceTypes::STONE, 100},
        // {ResourceTypes::WATER, 1000},
        {ResourceTypes::GRASS, 1},
        {ResourceTypes::CORNSTALK, 4},
    };
    

    void GenerateResources(std::vector<std::vector<std::shared_ptr<Tile>>> &tiles)
    {
        // iterate through tiles
        for (auto &row : tiles)
        {
            for (auto &tile_ptr : row)
            {
                // if tile has no resource
                if (!tile_ptr->HasResource())
                {
                    // generate a resource
                    // tile_ptr->AddResource(Resource::GenerateResource());
                }
            }
        }
    }
    ResourcePtr GenerateResourceFromNeighbors(std::vector<TilePtr> &tiles){
        // tiles are the neighbors of the target tile
        std::map<ResourceTypes, double> probs = resource_probabilities; // seed with base resource probs

        // iterate through tiles
        for (auto &tile_ptr : tiles)
        {
            // if tile has a resource
            if (tile_ptr->HasResource())
            {
                // get resource type
                ResourceTypes resource_type = tile_ptr->GetResource()->GetResourceType();
                // add to probs
                probs[resource_type] += resource_neighbor_probabilities[resource_type];
            }
        }
        double total = 0.0;
        // iterate through probs
        for (auto &prob : probs)
        {
            // add to total
            total += prob.second;
        }

        // get random value
        double random = total * ((double)std::rand()/(double)RAND_MAX); // random value from 0 to total
        double val = 0.0;

        // iterate through probs
        for (auto &prob : probs){
            val += prob.second;

            // if random is less than val
            if (random < val)
            {
                // generate a resource
                return ResourceFactory::MakeResource(prob.first, resource_amounts[prob.first]);
            }
        }
        return nullptr;
    }












} // namespace MapGen