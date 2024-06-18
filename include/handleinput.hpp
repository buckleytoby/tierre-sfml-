// include guard
#ifndef HANDLEINPUT_HPP_INCLUDED
#define HANDLEINPUT_HPP_INCLUDED

#include <mutex>
#include <functional>
#include <vector>
#include "SFML/Graphics.hpp"

namespace HandleInputNS
{

enum class InputResult {
    NOTHANDLED,
    HANDLED,
    PERSISTENT,
};

class HandleInput
{
    private:
        std::function<InputResult(sf::Event&)> m_run_cb;

    public:
        // static std::mutex handleInputMutex; // not working
        static bool global_lock;
        bool local_lock{false};
        bool can_override{false};

        std::vector<std::shared_ptr<HandleInput>> children;

        HandleInput();
        HandleInput(std::function<InputResult(sf::Event &)> run_cb);
        bool CanAccess();
        InputResult Run(sf::Event &event);
        void SetRunCb(std::function<InputResult(sf::Event&)> cb){m_run_cb = cb;}
        void SetOverride(bool override){can_override = override;}
        InputResult UpdateLock(InputResult result);
        void AddChild(std::shared_ptr<HandleInput> child);
        bool Lock();
        bool Unlock();
};

} // namespace HandleInputNS
#endif // HANDLEINPUT_HPP_INCLUDED