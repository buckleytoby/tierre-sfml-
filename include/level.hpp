// include guard
#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <memory>
#include <vector>
#include <random>


class Level
{
    public:
        int level_{0};
        double lvl_up_likelihood_{0.2}; // probability of leveling up
        std::random_device rd;
        std::mt19937 e2;
        std::uniform_real_distribution<> uniform_dist_;


        // ctor
        Level();
        Level(const Level& t); // copy ctor
        Level& operator=(const Level& t); // assignment operator
        bool operator==(const Level& other){return level_ == other.level_;}

        int GetLevel(){return level_;}
        void Update(Level &other);
        void SetLevel(int level) { level_ = level; }
        void Reset(){level_ = 0;}

        
        int GetAverageLevel(Level& other);
        int GetAverageLevel(std::vector<std::shared_ptr<Level>> others);
        bool TryLevelUp();
};
typedef std::shared_ptr<Level> LevelPtr;










#endif // LEVEL_HPP