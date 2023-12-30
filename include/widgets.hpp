// include guard
#ifndef GUI_HPP
#define GUI_HPP

#include <string>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

typedef std::shared_ptr<sf::RenderTexture> SFRenderTexturePtr; 

// following https://www.sfml-dev.org/tutorials/2.5/graphics-transform.php#object-hierarchies-scene-graph 
// the abstract base class
// keep the coordinate system the same as SFML (top-left is 0, 0)
enum class WidgetInputs{
    NONE,
    HANDLED,
};
class Widget
{
public:
    bool highlighted_{false};
    sf::Rect<double> bounds_{0, 0, 0, 0}; // left & top should be same as the transform_ origin
    sf::Transform transform_{sf::Transform::Identity}; // transform_ is this widget's transform w.r.t. its immediate parent
    std::vector<std::shared_ptr<Widget>> children_;

    void draw(sf::RenderTarget& target, const sf::Transform& parentTransform) const;
    sf::Rect<double> CalculateBounds();
    void Finalize();
    double GetParentX();
    double GetParentY();
    void AddChild(std::shared_ptr<Widget> child){children_.push_back(child);}
    
    // virtuals
    virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const=0;
    virtual sf::Rect<double> onCalculateBounds() {return bounds_;}
    virtual void Update(double dt, double x, double y);
    virtual WidgetInputs HandleInput(sf::Event& event){return WidgetInputs::NONE;}

};
typedef std::shared_ptr<Widget> WidgetPtr;

// a simple derived class: a widget that draws a sprite
class SpriteWidget : public Widget
{
public:

    // .. functions to define the sprite


    virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const
    {
        target.draw(m_sprite, transform);
    }

    sf::Sprite m_sprite;
};

class Button: public Widget
{
    public:
        sf::RectangleShape shape_;
        sf::Text text_;
        std::string text_str_;
        sf::Font font_;
        double border_height_{10}; // pixels
        double char_height_{30}; // pixels
        double char_width_{18}; // 0.6 & char height
        double text_width_{0};
        virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;

        Button(double x, double y, std::string str);
        virtual sf::Rect<double> onCalculateBounds() override;
};
typedef std::shared_ptr<Button> ButtonPtr;

class Dropdown: public Widget
{
    public:
        Dropdown(double x, double y, std::vector<std::string> strs);
        void AddItem(std::string str);
        int GetHighlightedChild();
        virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;
        virtual WidgetInputs HandleInput(sf::Event& event);
};
typedef std::shared_ptr<Dropdown> DropdownPtr;










#endif // GUI_HPP