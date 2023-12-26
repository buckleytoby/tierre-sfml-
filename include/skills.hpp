// include guard
# ifndef SKILLS_HPP
# define SKILLS_HPP

#include <memory>

// TODO: add knowledge multiplier in tasks
enum class KnowledgeTypes {
    BOTANY,
};

enum class SkillTypes {
    HAULING,
    MINING,
    CHOPPING,
    CONSTRUCTION,
    CRAFTING,
    END_OF_ENUM_VAL
};

class Skill
{
    public:
        // skill immutable parameters
        SkillTypes skill_type_;
        // level advance exponential parameters
        // y = a * (b ^ x) + c
        double exp_a_{100.0};
        double exp_b_{1.1};
        double exp_c_{0.0};
        double min_effort_{0.01};
        double max_effort_{1.0};

        // skill mutable parameters
        double level_{0.0};
        double experience_{0.0};
        double next_lvl_exp_{100.0};

        // skill methods
        Skill(){};
        Skill(SkillTypes skill_type){skill_type_ = skill_type;}
        void LevelUp();
        double GetLevel(){return level_;}
        double GetExperience(){return experience_;}
        void CheckLevelUp(){if (GetExperience() >= next_lvl_exp_) LevelUp();}
        void AddExperience(double experience){experience_ += experience; CheckLevelUp();}
        void SetLevel(double level){level_ = level;}
        void AddLevel(double level){SetLevel(GetLevel() + level);}

        double CalcEffortUnits();

};

class Construction : public Skill
{
    public:
        Construction(){skill_type_ = SkillTypes::CONSTRUCTION; exp_a_ = 100.0; exp_b_ = 1.1; exp_c_ = 0.0; min_effort_ = 0.01; max_effort_ = 1.0;}
};

class SkillFactory
{
    public:
        static std::shared_ptr<Skill> CreateSkill(SkillTypes skill_type){
            switch (skill_type)
            {
                case SkillTypes::CONSTRUCTION:
                    return std::make_shared<Construction>();
                    break;
                default:
                    return std::make_shared<Skill>();
                    break;
            }
        }
};

# endif // SKILLS_HPP