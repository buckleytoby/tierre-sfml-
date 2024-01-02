#include "resources.hpp"


const std::string to_string(ResourceTypes p){
  switch(p)
  {
    case ResourceTypes::LUMBER: return "L";
    case ResourceTypes::STONE: return "S";
    case ResourceTypes::WATER: return "W";
    case ResourceTypes::GRASS: return "G";
    case ResourceTypes::CORNSTALK: return "C";

  }
  return ""; // or an empty string
}
const std::string to_full_string(ResourceTypes p){
  switch(p)
  {
    case ResourceTypes::LUMBER: return "Lumber";
    case ResourceTypes::STONE: return "Stone";
    case ResourceTypes::WATER: return "Water";
    case ResourceTypes::GRASS: return "Grass";
    case ResourceTypes::CORNSTALK: return "Cornstalk";

  }
  return ""; // or an empty string
}

const sf::Color to_color(ResourceTypes p){
  switch(p)
  {
    case ResourceTypes::LUMBER: return sf::Color::Red;
    case ResourceTypes::STONE: return sf::Color::Yellow;
    case ResourceTypes::WATER: return sf::Color::Blue;
    case ResourceTypes::GRASS: return sf::Color::Green;

  }
  return sf::Color::White;
}