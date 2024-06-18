// include guard
#ifndef INTERACTIVE_HPP_INCLUDED
#define INTERACTIVE_HPP_INCLUDED

#include <memory>
#include "SFML/Graphics.hpp"
#include "handleinput.hpp"



/*
Simple class to provide a common interface which contains the handleinput member and call function
*/
class Interactive
{
    public:
        std::shared_ptr<HandleInputNS::HandleInput> m_handleinput;

        // ctor
        Interactive(){m_handleinput = std::make_shared<HandleInputNS::HandleInput>();};
        Interactive(std::function<HandleInputNS::InputResult(sf::Event&)> cb): Interactive() {SetHandleInputCb(cb);}

        // passthroughs
        HandleInputNS::InputResult HandleInput(sf::Event& event){return m_handleinput->Run(event);}
        void SetHandleInputCb(std::function<HandleInputNS::InputResult(sf::Event&)> cb){m_handleinput->SetRunCb(cb);}
        void SetHandleInputOverride(bool override){m_handleinput->SetOverride(override);}
        void HandleInputAddChild(std::shared_ptr<HandleInputNS::HandleInput> child){m_handleinput->AddChild(child);}
        bool HandleInputLock(){return m_handleinput->Lock();}
        bool HandleInputUnlock(){return m_handleinput->Unlock();}


        // I could add onHandleInput as a virtual function here, but idk how slow it'd be
        virtual HandleInputNS::InputResult onHandleInput(sf::Event& event){return HandleInputNS::InputResult::NOTHANDLED;}
};









#endif // INTERACTIVE_HPP_INCLUDED