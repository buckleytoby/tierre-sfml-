#include "widgets.hpp"

/////////////// shortcuts

WidgetPtr MakeWidget(double x, double y){return std::make_shared<Widget>(x, y);}
ButtonPtr MakeButton(double x, double y, std::string str){return std::make_shared<Button>(x, y, str);}





/////////////// End SHortcuts
Widget::Widget(double x, double y){
    transform_ = sf::Transform::Identity;
    transform_.translate(x, y);
    bounds_.left = x;
    bounds_.top = y;
    CalculateBounds();
}
Widget::Widget(double x, double y, double width, double height){
    Widget(x, y);
    bounds_.width = width;
    bounds_.height = height;
    CalculateBounds();
}

void Widget::Update(double dt, double x, double y){
    
    // inputs: x, y are the mouse position w.r.t. the parent
    // update the widget
    if(bounds_.contains(x, y)){
        if (!hovered_){
            hovered_ = true;
            onHover();
        }
    } else {
        if (hovered_){
            hovered_ = false;
            onLeaveHover();
        }
    }

    // call the self update
    onUpdate(dt, x, y);

    // update the children
    for (auto& child: children_){
            child->Update(dt, x - bounds_.left, y - bounds_.top);
    }
}
void Widget::draw(sf::RenderTarget& target, const sf::Transform& parentTransform) const {
    // if not visible, don't draw anything, including children
    if (!visible_){
        return;
    }

    // combine the parent transform with the widget's one
    sf::Transform combinedTransform = parentTransform * transform_;

    // let the widget draw itself
    onDraw(target, combinedTransform);

    // draw its children
    for (auto& child: children_){
            child->draw(target, combinedTransform);
    }
}

sf::Rect<double> Widget::CalculateBounds(){
    // calculate the bounds of the widget

    // self bounds
    auto bounds = onCalculateBounds();

    // iterate through children, find the max x, y, min x, y
    double min_x = bounds.left;
    double min_y = bounds.top;
    double max_x = bounds.left + bounds.width;
    double max_y = bounds.top + bounds.height;

    for (auto& child: children_){
        auto child_bounds = child->CalculateBounds();
        auto left = bounds.left + child_bounds.left;
        auto top = bounds.top + child_bounds.top;
        if (left < min_x){
            min_x = left;
        }
        if (top < min_y){
            min_y = top;
        }
        if (left + child_bounds.width > max_x){
            max_x = left + child_bounds.width;
        }
        if (top + child_bounds.height > max_y){
            max_y = top + child_bounds.height;
        }
    }

    bounds_.left = min_x;
    bounds_.top = min_y;
    bounds_.width = max_x - min_x;
    bounds_.height = max_y - min_y;

    return bounds_;
}
void Widget::Finalize(){
    // TODO: optimize this. Right now dropdown calls the recursive CalculateBounds() 3 times, once in additem, once here, then inside each of the children's finalize call

    // calculate the bounds of the widget
    CalculateBounds();

    // iterate through children, Finalize
    for (auto& child: children_){
        child->Finalize();
    }
}
double Widget::GetParentX(){
    // "Return the transform as a 4x4 matrix This function returns a pointer to an array of 16 floats containing the transform elements as a 4x4 matrix, which is directly compatible with OpenGL functions."

    // ref: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/#transformation-matrices 

    // get the parent's x
    auto mat = transform_.getMatrix();
    return mat[12];
}
double Widget::GetParentY(){
    return transform_.getMatrix()[13];
}
WidgetInputs Widget::HandleInput(sf::Event& event){
    // handle the input
    // only interact if the widget is visible, hovered, and  left-clicked
    // return HANDLED if the input is handled, NONE otherwise
    if (visible_){
        if(wasLeftClicked(event)){
            // reset the clicked_ status. Clicked_ depends on children. If a child is clicked, the parent is NOT clicked.
            clicked_ = false;
            // since children are drawn on top of the parent, iterate through children first and give them precedence, handle the input
            // doesn't require the parent to be hovered upon
            for (auto& child: children_){
                auto input = child->HandleInput(event); // recursion
                if (input == WidgetInputs::HANDLED){
                    return WidgetInputs::HANDLED;
                }
            }

            // if no children handle the click, execute this widget's onClick and return handled
            if (hovered_){
                clicked_ = true;
                if (onClick()){
                    return WidgetInputs::HANDLED;
                } else {
                    return WidgetInputs::NONE;
                }
            }
        }
    } else {
        return WidgetInputs::NONE;
    }
    return WidgetInputs::NONE;
}
bool Widget::onClick(){
    // debug
    std::cout << GetID() << " clicked." << std::endl;
    // call the callback
    if (onClick_cb_ == nullptr){
        return false;
    } else {
        onClick_cb_(); // TODO: force the onClick_cb_ to return a bool for success
        return true;
    }
}
bool Widget::onHover(){
    // debug
    std::cout << GetID() << " hovered." << std::endl;
    // call the callback
    if (onHover_cb_ == nullptr){
        return false;
    } else {
        onHover_cb_(); // TODO: force the onHover_cb_ to return a bool for success
        return true;
    }
}
bool Widget::onLeaveHover(){
    // debug
    std::cout << GetID() << " left hovered." << std::endl;
    // call the callback
    if (onLeaveHover_cb_ == nullptr){
        return false;
    } else {
        onLeaveHover_cb_(); // TODO: force the onLeaveHover_cb_ to return a bool for success
        return true;
    }
}
void Widget::AddChild(std::shared_ptr<Widget> child){
    if (child != nullptr){
        // add a child to the widget
        children_.push_back(child);
    }
}