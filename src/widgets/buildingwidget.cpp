#include "widgets.hpp"

BuildingWidget::BuildingWidget(double x, double y, double w, double h, BuildingPtr building_ptr): Widget(x, y)
{
    building_ptr_ = building_ptr;

    //// make the children that never change
    // border rectangle
    border_ = std::make_shared<RectWidget>(0, 0, w, h);
    AddChild(border_);

    text_ = std::make_shared<TextBox>(0, 0, "Change Active Recipe");
    AddChild(text_);
    
    // recipes list
    recipes_list_ = std::make_shared<Dropdown>(50, 10, building_ptr_->GetRecipeNames());
    recipes_list_->SetOnClickCallback([this](){
        // cb: set this worker's task to the selected idx from the dropdown menu
        building_ptr_->SetActiveRecipe(
            recipes_list_->GetClickedIdx()
        );
        reDraw();
        return true;
    });
    AddChild(recipes_list_);
}