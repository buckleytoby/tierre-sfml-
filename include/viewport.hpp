// include guard
#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include <sfml/Graphics.hpp>
#include "BitWise.hpp"
#include "globals.hpp"
#include "interactive.hpp"

enum class ViewportInputs
{
    NONE,
    HANDLED,
};
enum class ViewportActions
{
    // NONE = 0,
    NONE = 1 << 0, // 1
    SCROLL_UP = 1 << 1, // 2
    SCROLL_DOWN = 1 << 2, // 4
    SCROLL_LEFT = 1 << 3, // 8
    SCROLL_RIGHT = 1 << 4, // 16
    ZOOM_IN_ONCE = 1 << 5, // 32
    ZOOM_OUT_ONCE = 1 << 6, // 64
    Flag8 = 1 << 7  // 128
};

class Viewport: public Interactive
{
    public:
        double x_{0}, y_{0}, width_{0}, height_{0}; // in meters
        double map_mouse_x_{0}, map_mouse_y_{0}; // in meters
        double mouse_x_{0}, mouse_y_{0}; // in pixels
        BitFlag viewport_actions_{0};
        double scroll_sensitivity_{0.5}; // m/s
        double zoom_sensitivity_{0.5}; // m per tick, width
        double min_width_{10.0}, min_height_{5.0}; // in meters
        sf::RenderWindow* window_ptr_;

        // ctor
        Viewport();

        void SetX(double x);
        void SetY(double y);
        void SetWidth(double width);
        void SetHeight(double height);
        void SetWindowHandle(sf::RenderWindow* window);

        void update(double dt);
        void UpdateMousePosition();
        virtual HandleInputNS::InputResult onHandleInput(sf::Event& event);

        double GetPixelsPerMeterX(sf::RenderWindow& window);
        double GetPixelsPerMeterY(sf::RenderWindow& window);

        double ConvertPixelToMeterX(sf::RenderWindow& window, double pixel_x);
        double ConvertPixelToMeterY(sf::RenderWindow& window, double pixel_y);
        double ConvertMeterToPixelX(sf::RenderWindow& window, double meter_x);
        double ConvertMeterToPixelY(sf::RenderWindow& window, double meter_y);
        
        double ConvertPixelToMeterX(double pixel_x){return ConvertPixelToMeterX(*window_ptr_, pixel_x);}
        double ConvertPixelToMeterY(double pixel_y){return ConvertPixelToMeterY(*window_ptr_, pixel_y);}
        double ConvertMeterToPixelX(double meter_x){return ConvertMeterToPixelX(*window_ptr_, meter_x);}
        double ConvertMeterToPixelY(double meter_y){return ConvertMeterToPixelY(*window_ptr_, meter_y);}


        int GetIntX();
        int GetIntY();
        int GetIntWidth();
        int GetIntHeight();
        double GetX();
        double GetY();
        double GetWidth();
        double GetHeight();

        // get edges of bounding rect, in game coords
        double GetLeft(){return x_;}
        double GetRight(){return x_ + width_;}
        double GetBottom(){return y_;}
        double GetTop(){return y_ + height_;}
};
typedef std::shared_ptr<Viewport> ViewportPtr;


#endif // VIEWPORT_HPP