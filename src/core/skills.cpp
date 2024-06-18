#include "skills.hpp"
#include <cmath>

/////////////////////////////////////// Skill ///////////////////////////////////////
void Skill::LevelUp()
{
    // increase level, calculate next level up exp value
    AddLevel(1.0);
    next_lvl_exp_ = exp_a_ * pow(exp_b_, GetLevel()) + exp_c_;
}
double Skill::CalcEffortUnits()
{
    // calculate an abstract "effort" value for this skill based on level, normalized to 1
    // logarithm between min and max effort, min_effort_ at level 0, max_effort_ at level 99
    // y = 0.2154ln(x+1) + 0.01
    double y = max_effort_ * (0.2154 * log(GetLevel() + 1) + min_effort_);
    return y;
}