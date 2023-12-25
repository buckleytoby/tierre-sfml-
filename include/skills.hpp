// include guard
# ifndef SKILLS_HPP
# define SKILLS_HPP

enum class SkillTypes {
    HAULING,
    MINING,
    CHOPPING,
};

class Skill
{
    private:
        // skill immutable parameters
        SkillTypes skill_type_;
        // level advance exponential parameters
        // y = a * (b ^ x) + c
        double exp_a_{100.0};
        double exp_b_{1.1};
        double exp_c_{0.0};
        
    public:
        Skill(SkillTypes skill_type){skill_type_ = skill_type;}

        // skill mutable parameters
        double level_{0.0};
        double experience_{0.0};
        double next_lvl_exp_{100.0};

        // skill methods
        void LevelUp();
        double GetLevel(){return level_;}
        double GetExperience(){return experience_;}
        void CheckLevelUp(){if (GetExperience() >= next_lvl_exp_) LevelUp();}
        void AddExperience(double experience){experience_ += experience; CheckLevelUp();}
        void SetLevel(double level){level_ = level;}
        void AddLevel(double level){SetLevel(GetLevel() + level);}

};

# endif // SKILLS_HPP