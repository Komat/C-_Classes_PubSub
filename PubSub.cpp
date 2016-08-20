/**
 * @description
 * @fileName PubSub.cpp.
 * @author komatsu
 * @date 8/16/16.
 * @version 0.0
 */

#include "PubSub.h"

/**
 *
 * @return
 */
PubSub::PubSub() {
    _context = this;
}

/**
 *
 */
PubSub::~PubSub() {
    _context = NULL;
}

/**
 *
 * @param topic
 * @param data
 */
void PubSub::publish(std::string topic, void *data) {

    if(!hasTopic(topic)) {
        return;
    }

    std::map<int, std::vector<TopicData> > &topicList = _subscriberList[topic];

    for (std::map<int, std::vector<TopicData> >::reverse_iterator priorityList = topicList.rbegin(); priorityList != topicList.rend(); ++priorityList) {
        std::vector<TopicData> &subscriberList = priorityList->second;
        std::vector<TopicData>::reverse_iterator priorityItem;

        for (priorityItem = subscriberList.rbegin(); priorityItem != subscriberList.rend(); ++priorityItem) {
            (*priorityItem->subscriber)(topic, data);
        }
    }
}

/**
 *
 * @param topic
 * @param subscriber
 * @param priority
 */
void PubSub::subscribe(const std::string &topic, topicFunctionPtr subscriber, int priority) {

    TopicData data;
    data.subscriber = subscriber;

    _subscriberList[topic][priority].push_back(data);

}


/**
 *
 * @param topic
 * @param subscriber
 */
void PubSub::unsubscribe(const std::string &topic, topicFunctionPtr subscriber) {

    std::map<int, std::vector<TopicData> > &topicList = _subscriberList[topic];

    for (std::map<int, std::vector<TopicData> >::reverse_iterator priorityList = topicList.rbegin(); priorityList != topicList.rend(); ++priorityList) {

        std::vector<TopicData> &subscriberList = priorityList->second;

        for (std::vector<TopicData>::iterator priorityItem = subscriberList.begin(); priorityItem != subscriberList.end();) {
            if (priorityItem->subscriber == subscriber) {
                priorityItem = subscriberList.erase(subscriberList.begin() + std::distance(subscriberList.begin(), priorityItem));
                continue;
            }
            ++priorityItem;
        }
    }

    if (topicList.empty()) _subscriberList.erase(topic);
}

/**
 *
 * @param topic
 */
void PubSub::unsubscribeAll(const std::string &topic) {
    if (!hasTopic(topic)) return;

    std::map<int, std::vector<TopicData> > &topicList = _subscriberList[topic];
    std::map<int, std::vector<TopicData> >::iterator priorityList = topicList.begin();

    while (priorityList != topicList.end()) {
        topicList.erase(priorityList);
        if (priorityList->second.empty()) {
            priorityList = topicList.erase(priorityList++);
        } else {
            ++priorityList;
        }
    }

    if (topicList.empty()) _subscriberList.erase(topic);
}


/**
 *
 * @param topic
 * @return
 */
bool PubSub::hasTopic(const std::string &topic) {
    return (_subscriberList.find(topic) != _subscriberList.end());
}
