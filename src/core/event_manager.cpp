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

        // process all new messages on this topic
        while (topic->message_queue.size() > 0)
        {
            // remove msg from the queue
            auto msg = topic->message_queue.front();
            topic->message_queue.pop();

            // iterate through all subscribers to this topic
            for (auto& subscriber: topic->subscribers)
            {
                // invoke the subscriber callback
                // subscriber(msg);
            }
        }
    }

    return true;
}