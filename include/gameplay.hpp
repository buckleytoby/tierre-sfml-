#include <SFML/Graphics.hpp>
#include <memory>
#include "globals.hpp"
#include "BitWise.hpp"
#include "gamescreen.hpp"
#include "items.hpp"
#include "skills.hpp"
#include "senses.hpp"
#include "map.hpp"
#include "tiles.hpp"
#include "widgets.hpp"
#include "gui.hpp"
#include "viewport.hpp"

const std::string to_full_name(ResourceTypes p);
const std::string to_string(ResourceTypes p);
const sf::Color to_color(ResourceTypes p);
const std::string to_string(ItemTypes p);
const std::string to_string(WorkerStates p);
const std::string to_full_string(WorkerStates p);
const std::string to_string(NeedsTypes p);
const std::string to_string(BuildingTypes p);
const std::string to_full_string(BuildingTypes p);
const std::string to_full_string(BuildingStatus p);
const std::string to_full_string(ItemTypes p);
const std::string to_full_string(RecipeTypes p);



class GamePlay : GameScreen
{
    public:
        Viewport viewport_;
        Map map_;
        HUD hud_;

        // this is the task manager object. It has an analog UI element which is TaskManagerWidget inside of hud_
        TaskManagerPtr task_manager_ptr_;

        GamePlay();
        ~GamePlay();
        void draw(sf::RenderWindow& window);
        void update(double dt);
        GameScreenInputs HandleInput(sf::Event& event);
        void load(sf::RenderWindow& window);
        void unload();

        // tests
        void MakeTask1();
};