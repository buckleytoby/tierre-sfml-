// include guards
#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <memory>

#include <SFML/Graphics.hpp>
#include "globals.hpp"
#include "art.hpp"
#include "interactive.hpp"

/*
This class is for any object which can appear in the game world.
Common elements of all game objects are:
    bounds
    art
    update function (use the Update & onUpdate schema)

*/

class GameObject: public Interactive
{
    private:
        static int id_counter_;
    public:
        int id_;
        GameObject();
        GameObject(std::string name);

        // bounds
        Bounds_m bounds_m_; // ground truth

        // art
        std::shared_ptr<ArtNamespace::Art> art_;

        // update function
        void Update(double dt);
        void Draw(sf::RenderTarget& target);
        void LoadArt(std::string name);

        // getters
        double GetX(){return bounds_m_.x;}
        double GetY(){return bounds_m_.y;}
        XY<double> GetCenter(){return bounds_m_.GetCenter();}
        std::shared_ptr<ArtNamespace::Art> GetArt(){return art_;}
        bool PointInBounds(double x, double y){return bounds_m_.Contains(x, y);}

        // setters
        void SetX(double x){bounds_m_.x = x;}
        void SetY(double y){bounds_m_.y = y;}
        void SetPos(double x, double y){SetX(x); SetY(y);}
        void MoveX(double dx){bounds_m_.x += dx;}
        void MoveY(double dy){bounds_m_.y += dy;}

        // virtuals
        virtual void onUpdate(double dt);
};
typedef std::shared_ptr<GameObject> GameObjectPtr;










#endif //GAMEOBJECT_HPP