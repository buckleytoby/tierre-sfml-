// include guard
#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>
#include "widgets.hpp"
#include "viewport.hpp"
#include "map.hpp"
#include "taskmanager.hpp"
#include "interactive.hpp"

// forward declarations
class Widget; typedef std::shared_ptr<Widget> WidgetPtr;


enum class GUIActions
{
    // NONE = 0,
    VIEWPORT = 1 << 0, // 1
    MAP = 1 << 1, // 2
    SETCRAFTING = 1 << 2, // 4
    SCROLL_LEFT = 1 << 3, // 8
    SCROLL_RIGHT = 1 << 4, // 16
    ZOOM_IN_ONCE = 1 << 5, // 32
    ZOOM_OUT_ONCE = 1 << 6, // 64
    Flag8 = 1 << 7  // 128
};
enum class GUIInputs
{
    NONE,
    HANDLED,
};

class GUI: public Interactive
{
    public:
        std::vector<WidgetPtr> widgets_;
        double mouse_x_{0}, mouse_y_{0}; // in meters
        MapPtr map_ref_;
        ViewportPtr viewport_ref_;
        sf::Font font_;

        // ctor
        GUI();

        void Draw(sf::RenderWindow& window);
        virtual HandleInputNS::InputResult onHandleInput(sf::Event& event);
        void Update(double dt);
        void SetMousePosition(double x, double y){mouse_x_ = x; mouse_y_ = y;}
        void AddWidget(WidgetPtr widget){widgets_.push_back(widget);}
        void RemoveWidget(int id);
};

// GUI specific to the gameplay gamescreen
class HUD: public GUI
{
private:
    using inherited = GUI;
public:
        WidgetPtr selected_unit_status_ptr_{nullptr}; 

        // void Draw(sf::RenderWindow& window);

        // HUD();

};

#endif // GUI_HPP