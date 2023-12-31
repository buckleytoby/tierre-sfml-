// include guard
#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>
#include "widgets.hpp"
#include "viewport.hpp"
#include "map.hpp"
#include "taskmanager.hpp"


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

class GUI
{
    public:
        std::vector<WidgetPtr> widgets_;
        double mouse_x_{0}, mouse_y_{0}; // in meters
        MapPtr map_ref_{nullptr};
        ViewportPtr viewport_ref_{nullptr};
        sf::Font font_;

        void Draw(sf::RenderWindow& window);
        GUIInputs HandleInput(sf::Event& event);
        void Update(double dt);
        void SetMousePosition(double x, double y){mouse_x_ = x; mouse_y_ = y;}
        void AddWidget(WidgetPtr widget){widgets_.push_back(widget);}
};

// GUI specific to the gameplay gamescreen
class HUD: public GUI
{
private:
    using inherited = GUI;
public:
        TaskManagerWidgetPtr task_manager_panel_ptr_{nullptr};
        WidgetPtr selected_unit_status_ptr_{nullptr}; 

        // void Draw(sf::RenderWindow& window);

        HUD();

};

#endif // GUI_HPP