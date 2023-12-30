#include "widgets.hpp"

void Widget::Update(double dt, double x, double y){
    
    // inputs: x, y are the mouse position w.r.t. the parent
    // update the widget
    if(bounds_.contains(x, y)){
        highlighted_ = true;
    } else {
        highlighted_ = false;
    }

    // update the children
    for (auto& child: children_){
        child->Update(dt, x - bounds_.left, y - bounds_.top);
    }
}
void Widget::draw(sf::RenderTarget& target, const sf::Transform& parentTransform) const {
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