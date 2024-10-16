#include "art.hpp"

using namespace ArtNamespace;

// constructor
Art::Art(Bounds_m& bounds_m) : bounds_m_(bounds_m)
{
}
void Art::LoadArt(const char* path, sf::Texture& texture)
{
    if (!GetLoaded()){
        if (!texture.loadFromFile(path))
            texture = TEXTURES->art;
        GetSprite().setTexture(texture);
    }
}

void Art::onDraw(sf::RenderTarget& target)
{
    // scale the sprite?
    GetSprite().setPosition(ConvertToPixelsX(bounds_m_.x), ConvertToPixelsY(bounds_m_.GetTop())); // convert to game coord sys
    auto rect = GetSprite().getTextureRect();
    GetSprite().setScale(sf::Vector2f(PIXELS_PER_METER_X / (float)rect.width, PIXELS_PER_METER_Y / (float)rect.height));
    target.draw(GetSprite());
}

// static variable settings
sf::Sprite Art::sprite_;
bool Art::loaded_ = false;
sf::Sprite WorkerArtM::sprite_;
bool WorkerArtM::loaded_ = false;
sf::Sprite WorkerArtF::sprite_;
bool WorkerArtF::loaded_ = false;
sf::Sprite Lumber::sprite_;
bool Lumber::loaded_ = false;
sf::Sprite Stone::sprite_;
bool Stone::loaded_ = false;
sf::Sprite Water::sprite_;
bool Water::loaded_ = false;
sf::Sprite Grass::sprite_;
bool Grass::loaded_ = false;
sf::Sprite Cornstalk::sprite_;
bool Cornstalk::loaded_ = false;
