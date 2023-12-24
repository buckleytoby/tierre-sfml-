// import protection
#ifndef GLOBALS_HPP_INCLUDED
#define GLOBALS_HPP_INCLUDED


// globals
extern int FRAMERATE;
extern double DELTATIME; // tie physics to framerate

enum class WindowState {
    TITLE_SCREEN,
    GAMEPLAY,
    PAUSE_MENU,
    GAME_OVER
};

enum class HandleInputActions {
    QUIT = 1 << 0, // 1
    CHANGEACTIVEGAMESCREENTITLESCREEN = 1 << 1, // 2
    CHANGEACTIVEGAMESCREENGAMEPLAY = 1 << 2, // 4
    Flag4 = 1 << 3, // 8
    Flag5 = 1 << 4, // 16
    Flag6 = 1 << 5, // 32
    Flag7 = 1 << 6, // 64
    Flag8 = 1 << 7  // 128
};




#endif // GLOBALS_HPP_INCLUDED