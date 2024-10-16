// include guards
#ifndef GAMESCREEN_HPP
#define GAMESCREEN_HPP

#include <memory>

#include <SFML/Graphics.hpp>
#include "globals.hpp"
#include "BitWise.hpp"
#include "items.hpp"
#include "skills.hpp"
#include "senses.hpp"
#include "map.hpp"
#include "tiles.hpp"
#include "widgets.hpp"
#include "gui.hpp"
#include "viewport.hpp"
#include "art.hpp"
#include "handleinput.hpp"
#include "interactive.hpp"


enum class GameScreenInputs {
    NONE,
    HANDLED,
    QUIT,
    CHANGEACTIVEGAMESCREENTITLESCREEN,
    CHANGEACTIVEGAMESCREENGAMEPLAY,
};

class GameScreen: public Interactive
{
    public:

        // ctor
        GameScreen(): Interactive() {};

        // virtuals
        virtual void Draw(sf::RenderWindow& window){};
        virtual void update(double dt){};
        virtual void load(sf::RenderWindow& window){};
        virtual void unload(){};
};


class TitleScreen : public GameScreen
{
    public:
        TitleScreen();
        ~TitleScreen();

        // methods
        HandleInputNS::InputResult onHandleInput(sf::Event& event);

        // virtuals
        virtual void Draw(sf::RenderWindow& window);
        virtual void update(double dt);
        virtual void load(sf::RenderWindow& window);
        virtual void unload();
};


class GamePlay : public GameScreen
{
    public:
        ViewportPtr viewport_;
        MapPtr map_;
        std::shared_ptr<GUI> hud_;

        // this is the task manager object. It has an analog UI element which is TaskManagerWidget inside of hud_
        TaskManagerPtr task_manager_ptr_;

        GamePlay();
        ~GamePlay();

        // methods
        HandleInputNS::InputResult onHandleInput(sf::Event& event);

        // virtuals
        virtual void Draw(sf::RenderWindow& window);
        virtual void update(double dt);
        virtual void load(sf::RenderWindow& window);
        virtual void unload();

        // tests
        void MakeTask1();
};

#endif // GAMESCREEN_HPP