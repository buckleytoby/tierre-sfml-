// include guard
#ifndef UTILS_HPP
#define UTILS_HPP

#include <sfml/Graphics.hpp>
#include "globals.hpp"


template <typename T>
struct XY
{
    T x_{0}, y_{0};
    XY() {}
    XY(T x, T y)
        : x_(x), y_(y) {}
};

template <typename T>
struct Rect
{
    T x_{0}, y_{0}, width_{0}, height_{0};
    Rect() {}
    Rect(T x, T y, T width, T height)
        : x_(x), y_(y), width_(width), height_(height) {}
    std::vector<XY<int>> GetCoords(){
        std::vector<XY<int>> out;
        for (int i=floor(x_); i<ceil(x_+width_); i++){
            for (int j=floor(y_); j<ceil(y_+height_); j++){
                out.push_back(XY(i, j));
            }
        }
        return out;
    }
    std::vector<XY<int>> GetCoordsByEuclDist(XY<int>& xy){
        auto coords = GetCoords();
        std::sort(coords.begin(), coords.end(), [xy](XY<int> a, XY<int> b){
            return eucl_dist<int>(a, xy) < eucl_dist<int>(b, xy);
        });
        return coords;
    }
};

struct Bounds_m
{
    double x{0}, y{0}, width{0}, height{0};

    Bounds_m(double x, double y, double width, double height)
        : x(x), y(y), width(width), height(height) {}

    Bounds_m() {}

    bool Contains(double x, double y){
        return (x >= this->x && x <= this->x + this->width &&
                y >= this->y && y <= this->y + this->height);
    }

    bool Contains(Bounds_m bounds){
        return (bounds.x >= this->x && bounds.x + bounds.width <= this->x + this->width &&
                bounds.y >= this->y && bounds.y + bounds.height <= this->y + this->height);
    }

    bool Intersects(Bounds_m bounds){
        return (bounds.x < this->x + this->width &&
                bounds.x + bounds.width > this->x &&
                bounds.y < this->y + this->height &&
                bounds.y + bounds.height > this->y);
    }

    bool operator==(const Bounds_m& rhs){
        return (this->x == rhs.x && this->y == rhs.y &&
                this->width == rhs.width && this->height == rhs.height);
    }

    double GetRight(){
        return this->x + this->width;
    }
    double GetTop(){
        return this->y + this->height;
    }
    XY<double> GetXY(){
        return XY<double>{this->x, this->y};
    }
    XY<double> GetWH(){
        return XY<double>{this->width, this->height};
    }
    XY<double> GetCenter(){
        return XY<double>{this->x + this->width/2, this->y + this->height/2};
    }
};

template <typename T>
double eucl_dist(XY<T> xy1, XY<T> xy2){
    return std::abs(xy1.x_ - xy2.x_) + std::abs(xy1.y_ - xy2.y_);
}

bool wasLeftClicked(sf::Event event);

bool wasLeftMouseDown(sf::Event event);

template <typename T>
T std_sign(T x) {
    if (x > 0) return (T)1;
    if (x < 0) return (T)-1;
    return (T)0;
}


double ConvertToPixelsX(double meters);
double ConvertToPixelsY(double meters);



template <typename T>
const std::vector<std::string> all_strings(){
    std::vector<std::string> out;
    for (int i=T::START; i<T::END; i++){
        out.push_back(to_string((T)i));
    }
    return out;
}



#endif // UTILS_HPP