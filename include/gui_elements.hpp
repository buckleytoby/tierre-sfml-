// include guard
#ifndef GUI_HPP
#define GUI_HPP

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

typedef std::shared_ptr<sf::RenderTexture> SFRenderTexturePtr; 

class Dropdown
{
    public:
        std::vector<std::string> items_;
        double item_height_{30}; // pixels
        double border_height_{10}; // pixels
        sf::Font font_;
        int highlighted_idx_{-1};
        double parent_x_{0}, parent_y_{0};
        std::vector<sf::Text> texts_;
        std::vector<sf::RectangleShape> shapes_;
        int total_width_{0}, total_height_{0};
        bool mustDraw{true};
        SFRenderTexturePtr texture_;

        Dropdown();
        Dropdown(double x, double y){parent_x_ = x; parent_y_ = y;}
        void AddItem(std::string item){items_.push_back(item);}
        void GenerateShapes();
        SFRenderTexturePtr Draw(double x, double y);
        bool IsInside(sf::Vector2u pt, sf::Vector2u pos, sf::Vector2u size);
        int GetHighlightedIdx() { return highlighted_idx_; }
        void Update(double parent_x, double parent_y);
        bool IsInTexture(sf::Vector2u pt);
};










#endif // GUI_HPP