#include "event_manager.hpp"

EventManager::EventManager()
{

}

bool EventManager::Update(double dt)
{
    // iterate through all active topics
    for (auto& topic_map: m_topics)
    {
        auto& topic = topic_map.second;

        topic->Update(dt);
    }

    return true;
}

bool EventManager::GetTopicIfExist(std::string topic_name, TopicBase::TopicBasePtr& topic)
{
    if (m_topics.find(topic_name) != m_topics.end())
    {
        // de-reference the iterator
        topic = m_topics[topic_name];

        return true;
    }
    else
    {
        return false;
    }
}

// singleton, initialization
EventManager EVENT_MANAGER;