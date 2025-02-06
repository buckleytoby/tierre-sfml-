/*
There comes a time in every video game dev project when synchronous coupled code becomes arduous and a more asynchronous, modular, less-coupled system is needed.

I come from a robotics background so the idea of an event manager is second nature. In robotics, we simply use ROS2 as the event manager which operates network wide and takes of all the nasty details for us.

Unfortunately, I don't think it's prudent to make ROS2 a dependency of this game so instead I suppose I'll remake the core features of ROS2 as a barebones event manager.

Needed features:
    * create_publisher
    * create_subscriber
*/

// include guard
#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include "globals.hpp"
#include <vector>

template <typename T>
struct Topic
{
    // usings
    using TopicPtr = std::shared_ptr<Topic>;

    // members
    std::queue<T> message_queue;
    std::vector<void *> subscribers;
};


class EventManager
{
    public:
        // members
        std::map<std::string, Topic<bool>::TopicPtr> m_topics;

        // methods
        EventManager();
        
        bool Update(double dt);
};

#endif