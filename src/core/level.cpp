#include "level.hpp"

Level::Level(){
    e2 = std::mt19937(rd());
    uniform_dist_ = std::uniform_real_distribution<>(0, 1);
}
Level::Level(const Level& t){
    level_ = t.level_;
    lvl_up_likelihood_ = t.lvl_up_likelihood_;
}
Level& Level::operator=(const Level& t){
    level_ = t.level_;
    lvl_up_likelihood_ = t.lvl_up_likelihood_;
    return *this;
}

void Level::Update(Level& other){
    // update my level based on other's level
    SetLevel(GetAverageLevel(other));
}

int Level::GetAverageLevel(Level& other){
    // simple average right now
    return (level_ + other.GetLevel()) / 2;
}
int Level::GetAverageLevel(std::vector<std::shared_ptr<Level>> others){
    // simple average right now
    int sum = 0;
    for (auto& other : others){
        sum += other->GetLevel();
    }
    return sum / others.size();
}
bool Level::TryLevelUp(){
    // use probability map to determine if I level up
    auto rand_val = uniform_dist_(e2);
    if (rand_val < lvl_up_likelihood_){
        level_++;
        return true;
    } else {
        return false;
    }
}