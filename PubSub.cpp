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

    int counter = 0;

    std::map<int, std::vector<TopicData> > &topicList = _subscriberList[topic];

    for (std::map<int, std::vector<TopicData> >::reverse_iterator priorityList = topicList.rbegin(); priorityList != topicList.rend(); ++priorityList) {
        std::vector<TopicData> &subscriberList = priorityList->second;
//        std::vector<TopicData>::reverse_iterator priorityItem;

        for (std::string::reverse_iterator priorityItem = subscriberList.rbegin();
             priorityItem != subscriberList.rend(); ++priorityItem) {
            std::cout << "DELETE::COUNT(" << counter << ") ::::::::::::::::::: " << std::distance(subscriberList.begin(), priorityItem) << std::endl;
        }

//        for (priorityItem = subscriberList.rbegin(); priorityItem != subscriberList.rend(); ++priorityItem) {
//            if (priorityItem->subscriber == subscriber) {
//                std::cout << "DELETE::COUNT(" << counter << ") ::::::::::::::::::: " << std::distance(subscriberList.begin(), priorityItem) << std::endl;
////                priorityItem = subscriberList.erase(subscriberList.begin() + std::distance(subscriberList.begin(), priorityItem));
//            }
//        }

//        for (std::vector<TopicData>::iterator priorityItem = subscriberList.begin(); priorityItem != subscriberList.end(); ++priorityItem) {
//            std::cout << "UN SUBSCRIBE::COUNT(" << (priorityItem->subscriber == subscriber ? "TRUE" : "FALSE") << ")" << std::endl;
//            if (priorityItem->subscriber == subscriber) {
//                std::cout << "DELETE::COUNT(" << counter << ") ::::::::::::::::::: " << std::distance(subscriberList.begin(), priorityItem) << std::endl;
//                priorityItem = subscriberList.erase(subscriberList.begin() + std::distance(subscriberList.begin(), priorityItem));
//            }
//            counter++;
//        }
    }

    std::cout << "UN SUBSCRIBE" << std::endl;

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
            topicList.erase(priorityList++);
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
