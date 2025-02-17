/*
There comes a time in every video game dev project when synchronous coupled code becomes arduous and a more asynchronous, modular, less-coupled system is needed.

I come from a robotics background so the idea of an event manager is second nature. In robotics, we simply use ROS2 as the event manager which operates network wide and takes care of all the nasty details for us.

Unfortunately, I don't think it's prudent to make ROS2 a dependency of this game so instead I suppose I'll remake the core features of ROS2 as a barebones event manager.

Needed features:
    * create_publisher
    * create_subscriber
*/

// include guard
#pragma once

#include "globals.hpp"

struct TopicBase
{
    using TopicBasePtr = std::shared_ptr<TopicBase>;

    // members
    std::string m_topic_name;

    // constructor
    TopicBase(std::string topic_name)
    {
        m_topic_name = topic_name;
    }

    // virtuals
    virtual bool Update(double dt)
    {
        return true;
    }
};

template <typename T>
struct Topic : TopicBase
{
    // usings
    using TopicPtr = std::shared_ptr<Topic>;
    using CallbackFcn = std::function<void(T msg)>;

    // members
    std::queue<T> message_queue;
    std::vector<CallbackFcn> subscribers;

    // constructor
    Topic(std::string topic_name)
    : TopicBase(topic_name)
    {

    }

    // methods
    bool AddSubscriber(CallbackFcn cb)
    {
        subscribers.push_back(cb);

        return true;
    }

    ///
    bool AddMessage(T msg)
    {
        message_queue.push(msg);

        return true;
    }

    bool Update(double dt) override
    {
        // process all new messages on this topic
        while (message_queue.size() > 0)
        {
            // remove msg from the queue
            auto msg = message_queue.front();
            message_queue.pop();

            // iterate through all subscribers to this topic
            for (auto& subscriber: subscribers)
            {
                // DEBUG
                std::cout << "Call subscriber: " << m_topic_name << std::endl;
                // invoke the subscriber callback
                subscriber(msg);
            }
        }

        return true;
    }
};



class EventManager
{
    public:
        // members
        std::map<std::string, TopicBase::TopicBasePtr> m_topics;

        // methods
        EventManager();
        
        
        /// @brief Subscriber Creation
        /// @tparam T 
        /// @param topic_name 
        /// @param callback 
        template <typename T>
        void CreateSubscriber(std::string topic_name, std::function<void(T msg)> callback)
        {
            // get the topic
            TopicBase::TopicBasePtr topic_base;

            if (GetTopicIfExist(topic_name, topic_base))
            {
                // cast to our topic
                Topic<T>::TopicPtr topic = std::dynamic_pointer_cast<Topic<T>>(topic_base);

                // add the callback to the topic
                topic->AddSubscriber(callback);
            }
            else
            {
                // make the topic
                auto topic_ptr = std::make_shared<Topic<T>>(topic_name);

                // add to the map
                m_topics[topic_name] = topic_ptr;

                // now that the topic exists, recurse and it should find it
                CreateSubscriber<T>(topic_name, callback);
            }
        }

        ///
        template <typename T>
        void Publish(std::string topic_name, T msg)
        {
            // get the topic
            TopicBase::TopicBasePtr topic_base;

            if (GetTopicIfExist(topic_name, topic_base))
            {
                // cast to our topic
                Topic<T>::TopicPtr topic = std::dynamic_pointer_cast<Topic<T>>(topic_base);

                // DEBUG
                std::cout << "Publish: " << topic_name << std::endl;

                // add the msg to the topic's queue
                topic->AddMessage(msg);
            }
            else
            {
                // make the topic
                auto topic_ptr = std::make_shared<Topic<T>>(topic_name);

                // add to the map
                m_topics[topic_name] = topic_ptr;

                // now that the topic exists, recurse and it should find it
                Publish<T>(topic_name, msg);
            }
        }

        // core update method
        bool Update(double dt);

        // helper methods
        bool GetTopicIfExist(std::string, TopicBase::TopicBasePtr&);

        ///
        TopicBase::TopicBasePtr GetTopic(std::string);
};


//// Event Manager / Queue
// forward declaration, variable declaration
extern EventManager EVENT_MANAGER;