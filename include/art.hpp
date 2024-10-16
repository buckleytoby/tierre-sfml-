// include guard
#ifndef ART_HPP
#define ART_HPP

#include <memory>
#include <SFML/Graphics.hpp>
#include "globals.hpp"
#include "utils.hpp"



namespace ArtNamespace{
/*
Art class is for loading and storing all the art assets per game object
Contains static functions for loading art assets
Unlike widgets, art tracks in game coords, but then must convert to pixel coords for drawing
*/
class Art
{
    public:
        Art(Bounds_m& bounds_m);

        static sf::Texture texture_;
        static sf::Sprite sprite_;
        static bool loaded_;

        Bounds_m& bounds_m_; // bounds in meters

        // Methods
        void onDraw(sf::RenderTarget& target);
        void LoadArt(const char* path, sf::Texture& texture);
        
        // Virtuals
        virtual sf::Sprite& GetSprite(){return sprite_;}
        virtual bool GetLoaded(){return loaded_;}
};

class WorkerArtM: public Art
{
    public:
        static sf::Texture texture_;
        static sf::Sprite sprite_;
        static bool loaded_;

        WorkerArtM(Bounds_m& bounds_m): Art(bounds_m){}

        // Virtuals
        virtual sf::Sprite& GetSprite(){return sprite_;}
        virtual bool GetLoaded(){return loaded_;}
};

class WorkerArtF: public Art
{
    public:
        static sf::Texture texture_;
        static sf::Sprite sprite_;
        static bool loaded_;

        WorkerArtF(Bounds_m& bounds_m): Art(bounds_m){}

        // Virtuals
        virtual sf::Sprite& GetSprite(){return sprite_;}
        virtual bool GetLoaded(){return loaded_;}
};

class Lumber: public Art
{
    public:
        static sf::Texture texture_;
        static sf::Sprite sprite_;
        static bool loaded_;

        Lumber(Bounds_m& bounds_m): Art(bounds_m){}

        // Virtuals
        virtual sf::Sprite& GetSprite(){return sprite_;}
        virtual bool GetLoaded(){return loaded_;}
};
class Stone: public Art
{
    public:
        static sf::Texture texture_;
        static sf::Sprite sprite_;
        static bool loaded_;

        Stone(Bounds_m& bounds_m): Art(bounds_m){}

        // Virtuals
        virtual sf::Sprite& GetSprite(){return sprite_;}
        virtual bool GetLoaded(){return loaded_;}
};
class Water: public Art
{
    public:
        static sf::Texture texture_;
        static sf::Sprite sprite_;
        static bool loaded_;

        Water(Bounds_m& bounds_m): Art(bounds_m){}

        // Virtuals
        virtual sf::Sprite& GetSprite(){return sprite_;}
        virtual bool GetLoaded(){return loaded_;}
};
class Grass: public Art
{
    public:
        static sf::Texture texture_;
        static sf::Sprite sprite_;
        static bool loaded_;

        Grass(Bounds_m& bounds_m): Art(bounds_m){}

        // Virtuals
        virtual sf::Sprite& GetSprite(){return sprite_;}
        virtual bool GetLoaded(){return loaded_;}
};
class Cornstalk: public Art
{
    public:
        static sf::Texture texture_;
        static sf::Sprite sprite_;
        static bool loaded_;

        Cornstalk(Bounds_m& bounds_m): Art(bounds_m){}

        // Virtuals
        virtual sf::Sprite& GetSprite(){return sprite_;}
        virtual bool GetLoaded(){return loaded_;}
};



// factory
class ArtFactory
{
    public:
        static std::shared_ptr<Art> CreateArt(std::string name, Bounds_m& bounds_m){
            if (name == "worker_m"){
                auto ptr = std::make_shared<WorkerArtM>(bounds_m);
                ptr->LoadArt("../../../assets/worker_m.png", TEXTURES->worker_m);
                return ptr;
            }
            else if (name == "worker_f"){
                auto ptr = std::make_shared<WorkerArtF>(bounds_m);
                ptr->LoadArt("../../../assets/worker_f.png", TEXTURES->worker_f);
                return ptr;
            }
            else if (name == "lumber"){
                auto ptr = std::make_shared<Lumber>(bounds_m);
                ptr->LoadArt("../../../assets/lumber.png", TEXTURES->lumber);
                return ptr;
            }
            else if (name == "stone"){
                auto ptr = std::make_shared<Stone>(bounds_m);
                ptr->LoadArt("../../../assets/stone.png", TEXTURES->stone);
                return ptr;
            }
            else if (name == "water"){
                auto ptr = std::make_shared<Water>(bounds_m);
                ptr->LoadArt("../../../assets/water.png", TEXTURES->water);
                return ptr;
            }
            else if (name == "grass"){
                auto ptr = std::make_shared<Grass>(bounds_m);
                ptr->LoadArt("../../../assets/grass.png", TEXTURES->grass);
                return ptr;
            }
            else if (name == "cornstalk"){
                auto ptr = std::make_shared<Cornstalk>(bounds_m);
                ptr->LoadArt("../../../assets/cornstalk.png", TEXTURES->cornstalk);
                return ptr;
            }
            else{
                auto ptr = std::make_shared<Art>(bounds_m);
                ptr->LoadArt("../../../assets/default.png", TEXTURES->art);
                return ptr;
            }
        }
};
}



#endif //ART_HPP