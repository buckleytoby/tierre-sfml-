// include guard
#ifndef WIDGETS_HPP
#define WIDGETS_HPP

#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <SFML/Graphics.hpp>
#include "utils.hpp"
#include "taskmanager.hpp"
#include "map.hpp"
#include "resources.hpp"
#include "items.hpp"
#include "recipes.hpp"
#include "buildings.hpp"
#include "dynamic_objects.hpp"

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
    bool clicked_{false};
    bool visible_{true};
    sf::Rect<double> bounds_{0, 0, 0, 0}; // left & top should be same as the transform_ origin
    sf::Transform transform_{sf::Transform::Identity}; // transform_ is this widget's transform w.r.t. its immediate parent
    std::vector<std::shared_ptr<Widget>> children_;

    // callback pointer holders
    std::function<void()> onClick_cb_{nullptr};

    // callback setters
    void SetOnClickCallback(std::function<void()> cb){onClick_cb_ = cb;}

    // core functions
    Widget(double x, double y);
    Widget(double x, double y, double width, double height);
    void draw(sf::RenderTarget& target, const sf::Transform& parentTransform) const;
    sf::Rect<double> CalculateBounds();
    void Finalize();
    double GetParentX();
    double GetParentY();
    void AddChild(std::shared_ptr<Widget> child){children_.push_back(child);}
    WidgetInputs HandleInput(sf::Event& event);
    void MakeVisible(){visible_ = true;}
    void MakeInvisible(){visible_ = false;}
    void ToggleVisibility(){visible_ = !visible_;}
    
    // virtuals
    virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const {}
    virtual sf::Rect<double> onCalculateBounds() {return bounds_;}
    virtual void Update(double dt, double x, double y);
    virtual WidgetInputs onHandleInput(sf::Event& event){return WidgetInputs::NONE;}
    virtual bool onClick();
    virtual std::string GetID(){return "Widget";}

}; 
typedef std::shared_ptr<Widget> WidgetPtr;
WidgetPtr MakeWidget(double x, double y);

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

class TextBox: public Widget
{
    public:
        sf::Text text_;
        std::string text_str_;
        sf::Font font_;
        double char_height_{15}; // pixels
        double char_width_{15}; // 0.6 & char height
        double text_width_{0};

        TextBox(double x, double y, std::string str);
        virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;

        // virtuals
        virtual sf::Rect<double> onCalculateBounds();
        virtual std::string GetID(){return "TextBox";}
};
typedef std::shared_ptr<TextBox> TextBoxPtr;

class RectWidget: public Widget
{
    public:
        sf::RectangleShape shape_;
        double border_height_{10}; // pixels
        virtual void onDraw(sf::RenderTarget &target, const sf::Transform &transform) const;

        RectWidget(double x, double y, double w, double h);
        
        virtual std::string GetID(){return "RectWidget";}

};
typedef std::shared_ptr<RectWidget> RectWidgetPtr;

class Button: public Widget
{
    public:
        sf::RectangleShape shape_;
        double border_height_{5}; // pixels
        virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;

        Button(double x, double y, std::string str);
        virtual sf::Rect<double> onCalculateBounds();
        virtual std::string GetID(){return "Button";}
};
typedef std::shared_ptr<Button> ButtonPtr;
ButtonPtr MakeButton(double x, double y, std::string str);

class Dropdown: public Widget
{
    public:
        Dropdown(double x, double y, std::vector<std::string> strs);
        void AddItem(std::string str);
        int GetHighlightedChild();
        int GetClickedIdx();
        virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;
        virtual WidgetInputs HandleInput(sf::Event& event);
        // virtual bool onClick();
        virtual std::string GetID(){return "Dropdown";}
};
typedef std::shared_ptr<Dropdown> DropdownPtr;

class TaskManagerWidget: public Widget
{
    public:
        TaskManagerPtr task_manager_ptr_;

        // UI elements
        RectWidgetPtr border;
        ButtonPtr add_new_task;
        ButtonPtr remove_task;
        ButtonPtr edit_task;

        DropdownPtr list_of_tasks;
        DropdownPtr current_task;
        DropdownPtr list_of_actions; // including custom tasks
        ButtonPtr add_action;
        ButtonPtr remove_action;

        TaskManagerWidget(double x, double y, TaskManagerPtr task_manager_ptr);
        virtual void reDraw();
        virtual std::string GetID(){return "TaskManagerWidget";}
        // virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;
        virtual bool onClick();
};
typedef std::shared_ptr<TaskManagerWidget> TaskManagerWidgetPtr;

class SelectedStatus: public Widget
{
    public:
        MapPtr map_ref_;
        TaskManagerPtr task_manager_ptr_;
        DynamicObjectPtr selected_unit_ptr_{nullptr};
        RectWidgetPtr border_;
        TextBoxPtr text_;
        ButtonPtr button_active_task_;
        DropdownPtr tasks_list_;

        SelectedStatus(double x, double y, double w, double h, MapPtr map_ref, TaskManagerPtr task_manager_ptr);

        // virtuals
        virtual void Update(double dt, double x, double y);
        virtual void reDraw();
        virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;
};

#endif // WIDGETS_HPP