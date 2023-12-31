// import protection
#ifndef GLOBALS_HPP_INCLUDED
#define GLOBALS_HPP_INCLUDED

template <typename T>
struct Rect
{
    T x_{0}, y_{0}, width_{0}, height_{0};
};
template <typename T>
struct XY
{
    T x_{0}, y_{0};
};
template <typename T>
double eucl_dist(XY<T> xy1, XY<T> xy2){
    return std::abs(xy1.x_ - xy2.x_) + std::abs(xy1.y_ - xy2.y_);
}

// globals
extern int FRAMERATE;
extern double DELTATIME; // tie physics to framerate
extern int PIXELS_PER_METER; // meters * PIXELS_PER_METER = pixels
extern int SCREENWIDTH;
extern int SCREENHEIGHT;

enum class GamescreenStates {
    TITLE_SCREEN,
    GAMEPLAY,
    PAUSE_MENU,
    GAME_OVER
};




#endif // GLOBALS_HPP_INCLUDED