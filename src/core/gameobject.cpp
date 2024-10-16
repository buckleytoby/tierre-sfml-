#include "gameobject.hpp"

GameObject::GameObject(): Interactive()
{
    // constructor
    id_ = id_counter_++;
    bounds_m_.width = 1.0; // in meters
    bounds_m_.height = 1.0; // in meters
}
GameObject::GameObject(std::string name): GameObject()
{
    LoadArt(name);
}
void GameObject::LoadArt(std::string name){
    // art
    art_ = ArtNamespace::ArtFactory::CreateArt(name, bounds_m_);
}

// update
void GameObject::Update(double dt)
{
    // call the virtual self update
    selfUpdate(dt);
}

// virtuals
void GameObject::selfUpdate(double dt)
{
    // virtual
}

void GameObject::Draw(sf::RenderTarget& target)
{
    if (art_ == nullptr)
        return;
    // draw the art
    art_->onDraw(target);
}