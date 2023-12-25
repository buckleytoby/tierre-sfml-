#include "skills.hpp"
#include <cmath>

/////////////////////////////////////// Skill ///////////////////////////////////////
void Skill::LevelUp()
{
    // increase level, calculate next level up exp value
    AddLevel(1.0);
    next_lvl_exp_ = exp_a_ * pow(exp_b_, GetLevel()) + exp_c_;
}