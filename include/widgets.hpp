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
#include "worker.hpp"

// forward declarations
class Worker; typedef std::shared_ptr<Worker> WorkerPtr;
class Map; typedef std::shared_ptr<Map> MapPtr;
class TaskManager; typedef std::shared_ptr<TaskManager> TaskManagerPtr;
class DynamicObject; typedef std::shared_ptr<DynamicObject> DynamicObjectPtr;

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
private:
    static int id_counter_;
public:
    int id_;
    bool hovered_{false};
    bool clicked_{false};
    bool visible_{true};
    sf::Rect<double> bounds_{0, 0, 0, 0}; // left & top should be same as the transform_ origin
    sf::Transform transform_{sf::Transform::Identity}; // transform_ is this widget's transform w.r.t. its immediate parent
    std::vector<std::shared_ptr<Widget>> children_;

    // callback pointer holders
    std::function<bool()> onClick_cb_{nullptr};
    std::function<bool()> onMouseDown_cb_{nullptr};
    std::function<void()> onHover_cb_{nullptr};
    std::function<void()> onLeaveHover_cb_{nullptr};

    // callback setters
    void SetOnClickCallback(std::function<bool()> cb){onClick_cb_ = cb;}
    void SetOnMouseDownCallback(std::function<bool()> cb){onMouseDown_cb_ = cb;}
    void SetOnHoverCallback(std::function<void()> cb){onHover_cb_ = cb;}
    void SetOnLeaveHoverCallback(std::function<void()> cb){onLeaveHover_cb_ = cb;}

    // core functions
    Widget();
    Widget(double x, double y);
    Widget(double x, double y, double width, double height);
    void draw(sf::RenderTarget& target, const sf::Transform& parentTransform) const;
    sf::Rect<double> CalculateBounds();
    void Finalize();
    double GetParentX();
    double GetParentY();
    WidgetInputs HandleInput(sf::Event& event); // discrete events
    void Update(double dt, double x, double y); // continuous events
    void MakeVisible(){visible_ = true;}
    void MakeInvisible(){visible_ = false;}
    void ToggleVisibility(){visible_ = !visible_;}
    
    // virtuals
    virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const {}
    virtual void reDraw(){}
    virtual sf::Rect<double> onCalculateBounds() {return bounds_;}
    virtual void onUpdate(double dt, double x, double y){}
    virtual WidgetInputs onHandleInput(sf::Event& event){return WidgetInputs::NONE;}

    // user interactions
    virtual bool onClick();
    virtual bool onMouseDown();
    virtual bool onHover();
    virtual bool onLeaveHover();

public:
    void AddChild(std::shared_ptr<Widget> child);
    void RemoveChild(int id);
    void RemoveChild(std::shared_ptr<Widget> child);
    std::shared_ptr<Widget> ReplaceChild(std::shared_ptr<Widget> child, std::shared_ptr<Widget> new_child);
    int GetID(){return id_;}
    virtual std::string GetName(){return "Widget";}



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
        double border_thickness_{0};

        TextBox(double x, double y, std::string str);
        TextBox(double x, double y, std::string str, double border_thickness);
        void SetText(std::string str);

        // virtuals
        virtual sf::Rect<double> onCalculateBounds();
        virtual std::string GetName(){return "TextBox";}
        virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;
};
typedef std::shared_ptr<TextBox> TextBoxPtr;

class RectWidget: public Widget
{
    public:
        sf::RectangleShape shape_;
        double border_height_{10}; // pixels
        sf::Color border_color_{sf::Color::Blue};
        virtual void onDraw(sf::RenderTarget &target, const sf::Transform &transform) const;

        RectWidget(double x, double y, double w, double h);
        
        virtual std::string GetName(){return "RectWidget";}

};
class Border: public RectWidget
{
    public:
        double mouse_x_{0}, mouse_y_{0};
        Border(double x, double y, double w, double h) : RectWidget(x, y, w, h)
        {
            shape_.setFillColor(sf::Color::Transparent);
        }

        virtual void onUpdate(double dt, double x, double y){
            mouse_x_ = x;
            mouse_y_ = y;
            if (visible_){
                sf::Vector2f size(mouse_x_ - bounds_.left, mouse_y_ - bounds_.top);
                shape_.setSize(size);
            }
        }
        virtual std::string GetName(){return "Border";}
};
typedef std::shared_ptr<RectWidget> RectWidgetPtr;

class Button: public Widget
{
    public:
        sf::RectangleShape shape_;
        TextBoxPtr textbox_;
        double border_height_{5}; // pixels
        sf::Color border_color_{sf::Color::Blue};
        sf::Color fill_color_{sf::Color::White};
        sf::Color fill_color2_{sf::Color::Green};


        Button(double x, double y, std::string str);
        virtual sf::Rect<double> onCalculateBounds();
        void SetTextColor(sf::Color color);
        void SetText(std::string);
        void SetHighlighted();
        void SetUnHighlighted();
        virtual std::string GetName() { return "Button"; }
};
typedef std::shared_ptr<Button> ButtonPtr;
ButtonPtr MakeButton(double x, double y, std::string str);

class Dropdown: public Widget
{
    public:
        int clicked_idx_{-1};
        std::map<int, bool> clicked_idxs_;

        Dropdown(double x, double y, std::vector<std::string> strs);
        virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;
        virtual WidgetInputs HandleInput(sf::Event& event);
        // virtual bool onClick();

    public:
        void AddItem(std::string str);
        void SetTextColor(sf::Color color);

        int                 GetHoveredChild();
        int                 GetClickedIdx();
        std::vector<int>    GetClickedIdxs();
        virtual std::string GetName(){return "Dropdown";}

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
        virtual std::string GetName(){return "TaskManagerWidget";}
        // virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const;
        virtual bool onClick();
};
typedef std::shared_ptr<TaskManagerWidget> TaskManagerWidgetPtr;

class BuildingWidget: public Widget
{
    public:
        BuildingPtr building_ptr_;

        RectWidgetPtr border_;
        TextBoxPtr text_;

        DropdownPtr recipes_list_;

        BuildingWidget(double x, double y, double w, double h, BuildingPtr building_ptr);

        // virtuals
        virtual std::string GetName(){return "BuildingWidget";}
};
typedef std::shared_ptr<BuildingWidget> BuildingWidgetPtr;

class SelectedStatus: public Widget
{
    public:
        // analog objects
        MapPtr map_ref_;
        TaskManagerPtr task_manager_ptr_;
        DynamicObjectPtr selected_unit_ptr_{nullptr};
        TilePtr selected_tile_ptr_{nullptr};

        // UI widgets
        RectWidgetPtr border_;
        TextBoxPtr text_;
        ButtonPtr button_active_task_;
        DropdownPtr tasks_list_;
        BuildingWidgetPtr building_widget_;
        ButtonPtr button_building_widget_;

        // map-level actions
        ButtonPtr button_unit_selector;

        // Core Actions
        ButtonPtr button_core_actions;
        DropdownPtr list_core_actions;
        std::vector<std::string> core_actions_;
        std::map<int, ActionTypes> idx_to_action_type_map_;

        // Buildings
        ButtonPtr button_buildings;
        DropdownPtr list_buildings;
        std::vector<std::string> buildings_;
        std::map<int, BuildingTypes> idx_to_building_type_map_;

        SelectedStatus(double x, double y, double w, double h, MapPtr map_ref, TaskManagerPtr task_manager_ptr);

        // virtuals
        virtual void onUpdate(double dt, double x, double y);
        virtual void reDraw();
        virtual std::string GetName(){return "SelectedStatus";}
};

class WorkerWidget: public Widget
{
    public:
        WorkerPtr worker_ptr_;
        // actions
        WorkerWidget(double x, double y, double w, double h, WorkerPtr worker_ptr);
        
        // virtuals
        virtual void onUpdate(double dt, double x, double y);
        virtual void reDraw();
        virtual std::string GetName(){return "WorkerWidget";}

};

// class CheckboxList

class SelectItems: public Widget
{
    public:
        // vars
        bool taking{true};

        // UI widgets
        DropdownPtr list_of_items_;
        ButtonPtr done_;
        ButtonPtr cancel_;
        ButtonPtr leave_or_take;
        std::map<int, ItemTypes> idx_to_item_type_map_;

        SelectItems(double x, double y, double w, double h);
        std::vector<ItemTypes> GetSelectedItems();

        // virtuals
        // virtual void onUpdate(double dt, double x, double y);
        // virtual void reDraw();
        virtual std::string GetName(){return "SelectItems";}

};

#endif // WIDGETS_HPP