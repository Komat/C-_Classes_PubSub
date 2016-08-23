/**
 * @description
 * @fileName PubSub.h.
 * @author komatsu
 * @date 8/16/16.
 * @version 0.0
 */

#ifndef PUBSUB_PUBSUB_H
#define PUBSUB_PUBSUB_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>


template<
        class TopicHandler = std::function<void()>
>
class PubSub {
private:

    std::map<const std::string, std::map<int, std::vector<TopicHandler> > > _subscriberList;
    std::map<const std::string, std::unordered_set<const TopicHandler *> > _subscriberOnceList;

public:

    template<class ...Args>
    void publish(const std::string &topic, Args &&... args) {

        if (!hasTopic(topic)) {
            return;
        }

        auto &topicList = _subscriberList[topic];

        for (auto priorityList = topicList.rbegin(); priorityList != topicList.rend(); ++priorityList) {
            auto &subscriberList = priorityList->second;

            for (auto priorityItem = subscriberList.rbegin(); priorityItem != subscriberList.rend();) {
                auto &listener = *priorityItem;
                listener(topic, std::forward<Args>(args)...);

                if (_subscriberOnceList.count(topic)) {
                    if (_subscriberOnceList[topic].count(&listener)) {
//                        priorityItem = subscriberList.erase(priorityItem);
                        _subscriberOnceList[topic].erase(&listener);
                    } else {
                        ++priorityItem;
                    }
                } else {
                    ++priorityItem;
                }

//                if (_subscriberOnceList.count(topic)) {
//                    if (!_subscriberOnceList[topic].empty()) {
//                        auto onceItem = _subscriberOnceList[topic].begin();
//                        while (onceItem != _subscriberOnceList[topic].end()) {
//                            std::cout << (onceItem == listener ? "TRUE" : "FALSE") << std::endl;
////                            if (onceItem == listener) {
////                                priorityItem = priorityItem.eraes(onceItem);
////                                onceItem = _subscriberOnceList[topic].erase(onceItem);
////                            } else {
//                                ++onceItem;
////                            }
//                        }
//                    }
//                }
            }
        }

    };


    void subscribe(const std::string &topic, TopicHandler subscriber, int priority = 0) {
        _subscriberList[topic][priority].push_back(subscriber);
    };


    void subscribeOnce(const std::string &topic, TopicHandler subscriber, int priority = 0) {
        _subscriberList[topic][priority].push_back(subscriber);
        _subscriberOnceList[topic].insert(&subscriber);
    };


    void unsubscribe(const std::string &topic) {
        unsubscribeAll(topic);
    };


    void unsubscribe(const std::string &topic, TopicHandler subscriber) {
        std::map<int, std::vector<TopicHandler> > &topicList = _subscriberList[topic];

//        for (std::map<int, std::vector<TopicHandler> >::reverse_iterator priorityList = topicList.rbegin();
//             priorityList != topicList.rend(); ++priorityList) {
//
//            std::vector<TopicHandler> &subscriberList = priorityList->second;
//            std::vector<TopicHandler>::iterator priorityItem;
//
//            for (priorityItem = subscriberList.begin(); priorityItem != subscriberList.end();) {
//                if (priorityItem == subscriber) {
//                    priorityItem = subscriberList.erase(
//                            subscriberList.begin() + std::distance(subscriberList.begin(), priorityItem));
//                    continue;
//                }
//                ++priorityItem;
//            }
//        }

        if (topicList.empty()) _subscriberList.erase(topic);
    };


    void unsubscribeAll(const std::string &topic) {
        if (!hasTopic(topic)) return;

//        std::map<int, std::vector<TopicHandler> > &topicList = _subscriberList[topic];
//        std::map<int, std::vector<TopicHandler> >::iterator priorityList = topicList.begin();
//
//        while (priorityList != topicList.end()) {
//            topicList.erase(priorityList++);
//        }

//        if (topicList.empty()) _subscriberList.erase(topic);
    };


    bool hasTopic(const std::string &topic) {
        return (_subscriberList.find(topic) != _subscriberList.end());
    };


};

#endif //PUBSUB_PUBSUB_H
