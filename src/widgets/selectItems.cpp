#include "widgets.hpp"
int Widget::id_counter_ = 0;

SelectItems::SelectItems(double x, double y, double w, double h): Widget(x, y, w, h){
    // iterate through itemtypes to get list of strings
    std::vector<std::string> item_strs;
    int count=0;
    for (auto& item : ITEM_TYPES){
        if (item == ItemTypes::NONE)
            continue;
        item_strs.push_back(to_full_string(item));
        idx_to_item_type_map_[count] = item;
        count++;
    }
    // make the dropdown menu
    list_of_items_ = std::make_shared<Dropdown>(0, 50, item_strs);

    // set the on-click callback
    list_of_items_->SetOnClickCallback([this](){
        std::cout << "clicked: " << list_of_items_->GetClickedIdx() << std::endl;
        list_of_items_->SetTextColor(sf::Color::Green);

        return true;
    });
    // add to children
    AddChild(list_of_items_);

    // make the done button
    done_ = std::make_shared<Button>(0, 0, "Done");
    done_->SetOnClickCallback([this](){
        // make self invisible
        MakeInvisible();
        // call the SelectItems onClick callback
        this->onClick();
        return true;
    });
    AddChild(done_);

    // make the leave or take button
    leave_or_take = std::make_shared<Button>(100, 0, "Taking");
    leave_or_take->SetOnClickCallback([this](){
        if (taking){
            leave_or_take->SetHighlighted();
            leave_or_take->SetText("Leaving");
        } else {
            leave_or_take->SetUnHighlighted();
            leave_or_take->SetText("Taking");
        }
        taking = !taking;
        return true;
    });
    AddChild(leave_or_take);

    // make the cancel button
    cancel_ = std::make_shared<Button>(200, 0, "Cancel");
    cancel_->SetOnClickCallback([this](){
        // make self invisible
        MakeInvisible();
        return true;
    });
    AddChild(cancel_);
}

std::vector<ItemTypes> SelectItems::GetSelectedItems(){
    std::vector<ItemTypes> selected_items;
    for (auto& idx : list_of_items_->GetClickedIdxs()){
        selected_items.push_back(idx_to_item_type_map_[idx]);
    }
    return selected_items;
}