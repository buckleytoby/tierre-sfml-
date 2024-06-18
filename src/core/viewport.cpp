#include "viewport.hpp"


/////////////////////////////////////// Viewport ///////////////////////////////////////
Viewport::Viewport(){   
    // constructor
    SetHandleInputCb(std::bind(&Viewport::onHandleInput, this, std::placeholders::_1));
}
void Viewport::SetX(double x){
    x_ = x;
}
void Viewport::SetY(double y){
    y_ = y;
}
void Viewport::SetWidth(double width){
    if (width < min_width_){
        return;
    } else {
        width_ = width;
    }
}
void Viewport::SetHeight(double height){
    if (height < min_height_){
        return;
    } else {
        height_ = height;
    }
}
void Viewport::SetWindowHandle(sf::RenderWindow* window){
    window_ptr_ = window;
}
int Viewport::GetIntX(){
    return floor(x_);
}
int Viewport::GetIntY(){
    return floor(y_);
}
double Viewport::GetX(){
    return x_;
}
double Viewport::GetY(){
    return y_;
}
int Viewport::GetIntWidth(){
    return (int)width_;
}
int Viewport::GetIntHeight(){
    return (int)height_;
}
double Viewport::GetWidth(){
    return width_;
}
double Viewport::GetHeight(){
    return height_;
}
void Viewport::update(double dt){
    // update the viewport
    double dx = 0;
    double dy = 0;
    double dz = 0;
    double width = GetWidth();
    double height = GetHeight();

    // if over the bitwise actions, as a function of the current width/height
    if (viewport_actions_.HasFlag((BitEnum)ViewportActions::SCROLL_UP)){
        dy += height * scroll_sensitivity_;
    }
    if (viewport_actions_.HasFlag((BitEnum)ViewportActions::SCROLL_DOWN)){
        dy -= height * scroll_sensitivity_;
    }
    if (viewport_actions_.HasFlag((BitEnum)ViewportActions::SCROLL_LEFT)){
        dx -= width * scroll_sensitivity_;
    }
    if (viewport_actions_.HasFlag((BitEnum)ViewportActions::SCROLL_RIGHT)){
        dx += width * scroll_sensitivity_;
    }
    if (viewport_actions_.HasFlag((BitEnum)ViewportActions::ZOOM_IN_ONCE)){
        dz -= width * zoom_sensitivity_;
        viewport_actions_.UnsetFlag((BitEnum)ViewportActions::ZOOM_IN_ONCE);
    }
    if (viewport_actions_.HasFlag((BitEnum)ViewportActions::ZOOM_OUT_ONCE)){
        dz += width * zoom_sensitivity_;
        viewport_actions_.UnsetFlag((BitEnum)ViewportActions::ZOOM_OUT_ONCE);
    }
    // calc center of viewport and aspect ratio
    double cx = x_ + width_ / 2;
    double cy = y_ + height_ / 2;
    double aspect_ratio = width_ / height_;

    // update width & height
    SetWidth(width_ + dz);
    SetHeight(height_ + dz / aspect_ratio);

    // new x & y
    double new_x = cx - GetWidth() / 2;
    double new_y = cy - GetHeight() / 2;

    // update x & y
    SetX(new_x + dx * dt);
    SetY(new_y + dy * dt);

    // update globals
    PIXELS_PER_METER_X = GetPixelsPerMeterX(*window_ptr_);
    PIXELS_PER_METER_Y = GetPixelsPerMeterY(*window_ptr_);
    VIEWPORT_X = GetX();
    VIEWPORT_Y = GetY();
    VIEWPORT_W = GetWidth();
    VIEWPORT_H = GetHeight();
}
void Viewport::UpdateMousePosition(){
    // update the mouse position
    auto mouse_pos = sf::Mouse::getPosition(*window_ptr_);
    mouse_x_ = mouse_pos.x;
    mouse_y_ = mouse_pos.y;
    map_mouse_x_ = ConvertPixelToMeterX(*window_ptr_, mouse_x_);
    map_mouse_y_ = ConvertPixelToMeterY(*window_ptr_, mouse_y_);
}
double Viewport::GetPixelsPerMeterX(sf::RenderWindow& window){
    // get pixels per meter x
    // TODO: cache this
    return (double)window.getSize().x / width_;
}
double Viewport::GetPixelsPerMeterY(sf::RenderWindow& window){
    // get pixels per meter y
    // TODO: cache this
    return(double)window.getSize().y / height_;
}

double Viewport::ConvertPixelToMeterX(sf::RenderWindow& window, double pixel_x){
    // convert pixel x to meter x
    double pixelsPerMeterW = GetPixelsPerMeterX(window);
    double meter_x = pixel_x / pixelsPerMeterW + x_;
    return meter_x;
}
double Viewport::ConvertPixelToMeterY(sf::RenderWindow& window, double pixel_y){
    // convert pixel y to meter y
    double pixelsPerMeterH = GetPixelsPerMeterY(window);
    // transform from SFML window coordinate system (x-right, y-down) to gameplay coordinate system (x-right, y-up)
    double new_pixel_y = window.getSize().y - pixel_y;

    double meter_y = new_pixel_y / pixelsPerMeterH + y_;
    return meter_y;
}
double Viewport::ConvertMeterToPixelX(sf::RenderWindow& window, double meter_x){
    // convert meter x to pixel x
    double pixelsPerMeterW = GetPixelsPerMeterX(window);
    double pixel_x = (meter_x - x_) * pixelsPerMeterW;
    return pixel_x;
}
double Viewport::ConvertMeterToPixelY(sf::RenderWindow& window, double meter_y){
    // convert meter y to pixel y
    double pixelsPerMeterH = GetPixelsPerMeterY(window);
    double pixel_y = (meter_y - y_) * pixelsPerMeterH;

    // transform from gameplay coordinate system (x-right, y-up) to SFML window coordinate system (x-right, y-down)
    pixel_y = window.getSize().y - pixel_y;
    return pixel_y;
}
/////////////////////////////////////// End Viewport ///////////////////////////////////////
