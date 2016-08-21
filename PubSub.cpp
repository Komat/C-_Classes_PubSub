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
 * @param topicOption
 */
void PubSub::publish(std::string topic, void *topicOption) {

    if(!hasTopic(topic)) {
        return;
    }

    std::map<int, std::vector<TopicData> > &topicList = _subscriberList[topic];

    for (std::map<int, std::vector<TopicData> >::reverse_iterator priorityList = topicList.rbegin(); priorityList != topicList.rend(); ++priorityList) {
        std::vector<TopicData> &subscriberList = priorityList->second;
        std::vector<TopicData>::reverse_iterator priorityItem;

        for (priorityItem = subscriberList.rbegin(); priorityItem != subscriberList.rend(); ++priorityItem) {
            (priorityItem->subscriber)(topic, topicOption);

            if (_subscriberOnceList.count(topic)) {
                if (!_subscriberOnceList[topic].empty()) {
//                    std::vector<TopicData>::iterator onceItem = _subscriberOnceList[topic].begin();
//                    while (onceItem != _subscriberOnceList[topic].end()) {
//                        if (*onceItem == *priorityItem) {
//                            unsubscribe(topic, (priorityItem->subscriber));
//                            onceItem = _subscriberOnceList[topic].erase(onceItem);
//                        } else {
//                            ++onceItem;
//                        }
//                    }
                    std::vector<TopicData>::reverse_iterator onceItem;

                    for(onceItem = subscriberList.rbegin(); onceItem != subscriberList.rend(); onceItem++)
                    {
                        if ((onceItem->subscriber) == (priorityItem->subscriber)) {
                            subscriberList.erase((++onceItem).base());
                            continue;
                        }
                        ++onceItem;
                    }
//                    while (onceItem != _subscriberOnceList[topic].end()) {
//                        if (*onceItem == *priorityItem) {
//                            unsubscribe(topic, (priorityItem->subscriber));
//                            onceItem = _subscriberOnceList[topic].erase(onceItem);
//                        } else {
//                            ++onceItem;
//                        }
//                    }
                }
            }
        }
    }
}

/**
 *
 * @param topic
 * @param subscriber
 * @param priority
 */
void PubSub::subscribe(const std::string &topic, TopicFunc subscriber, int priority) {
    TopicData data;
    data.subscriber = subscriber;

    _subscriberList[topic][priority].push_back(data);
}

/**
 *
 * @param topic
 * @param subscriber
 * @param priority
 */
void PubSub::subscribeOnce(const std::string &topic, TopicFunc subscriber, int priority) {

    TopicData data;
    data.subscriber = subscriber;

    _subscriberList[topic][priority].push_back(data);
    _subscriberOnceList[topic].push_back(data);

}


/**
 *
 * @param topic
 */
void PubSub::unsubscribe(const std::string &topic) {
    unsubscribeAll(topic);
}


/**
 *
 * @param topic
 * @param subscriber
 */
void PubSub::unsubscribe(const std::string &topic, TopicFunc subscriber) {

    std::map<int, std::vector<TopicData> > &topicList = _subscriberList[topic];
    std::map<int, std::vector<TopicData> >::reverse_iterator priorityList;

    for (priorityList = topicList.rbegin(); priorityList != topicList.rend(); ++priorityList) {

        std::vector<TopicData> &subscriberList = priorityList->second;
        std::vector<TopicData>::reverse_iterator priorityItem;

//        for(priorityItem = subscriberList.rbegin(); priorityItem != subscriberList.rend(); priorityItem++)
//        {
//            if (priorityItem->subscriber == subscriber) {
//                subscriberList.erase((++priorityItem).base());
//                continue;
//            }
//            ++priorityItem;
//        }

//        std::vector<TopicData>::reverse_iterator priorityItem;
//
//        for (priorityItem = subscriberList.begin(); priorityItem != subscriberList.end();) {
//            if (priorityItem->subscriber == subscriber) {
//                priorityItem = subscriberList.erase(subscriberList.begin() + std::distance(subscriberList.begin(), priorityItem));
//                continue;
//            }
//            ++priorityItem;
//        }
//        for (priorityItem = subscriberList.rbegin(); priorityItem != subscriberList.rend();) {
//            if (priorityItem->subscriber == subscriber) {
//                priorityItem = subscriberList.erase(subscriberList.rbegin() + priorityItem);
//                continue;
//            } else {
//                ++priorityItem;
//            }
//        }
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
        topicList.erase(priorityList++);
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
