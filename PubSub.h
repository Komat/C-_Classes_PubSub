/**
 * @description
 * @fileName PubSub.h.
 * @author komat
 * @date 8/16/16.
 * @version 0.0
 */

#ifndef PUBSUB_PUBSUB_H
#define PUBSUB_PUBSUB_H

#include <iostream>
#include <string>
#include <map>
#include <unordered_set>
#include <vector>


template<typename TopicHandler = std::function<void()>>
class PubSub {
private:

    std::size_t _topicCount = 0; // TODO: カウント数の定義・使用用途が曖昧なため現状使用不可
    std::map<const std::string, std::map<int, std::vector<TopicHandler> > > _subscriberList;
    std::map<const std::string, std::unordered_set<TopicHandler *> > _subscriberOnceList;

public:

    template<class ...Args>
    void publish(const std::string &topic, Args &&... args) {

        if (!hasTopic(topic)) {
            return;
        }
        auto &topicList = _subscriberList[topic]; // std::map

        for (auto priorityList = topicList.begin(); priorityList != topicList.end(); ++priorityList) {
            auto &subscriberList = priorityList->second; // std::vector

            for (auto priorityItem = subscriberList.begin(); priorityItem != subscriberList.end();) {
                auto &listener = *priorityItem; // TopicHandler
                listener(topic, std::forward<Args>(args)...);

                if (_subscriberOnceList.count(topic)) {
                    if (_subscriberOnceList[topic].count(&listener)) {
                        priorityItem = subscriberList.erase(
                                subscriberList.begin() + std::distance(subscriberList.begin(), priorityItem)
                        );
                        _subscriberOnceList[topic].erase(&listener);
                    } else {
                        ++priorityItem;
                    }
                } else {
                    ++priorityItem;
                }
            }
        }

    };


    void subscribe(const std::string &topic, TopicHandler subscriber, int priority) {
        _subscriberList[topic][priority].push_back(subscriber);
        ++_topicCount;
    };


    void subscribeOnce(const std::string &topic, TopicHandler subscriber, int priority = 0) {
        auto &listeners = _subscriberList[topic][priority];
        listeners.push_back(subscriber);
        _subscriberOnceList[topic].insert(&listeners.back());
        ++_topicCount;
    };


    void unsubscribe(const std::string &topic) {

        if (!hasTopic(topic)) {
            return;
        }

        auto &topicList = _subscriberList[topic]; // std::map

        for (auto priorityList = topicList.begin(); priorityList != topicList.end(); ++priorityList) {
            auto &subscriberList = priorityList->second; // std::vector

            for (auto priorityItem = subscriberList.begin(); priorityItem != subscriberList.end();) {
                priorityItem = subscriberList.erase(
                        subscriberList.begin() + std::distance(subscriberList.begin(), priorityItem)
                );
            }
        }

        if (!_subscriberOnceList[topic].empty()) {
            _subscriberOnceList[topic].clear();
        }

        if (topicList.empty()) _subscriberList.erase(topic);

    };


    void unsubscribeAll() {
        _subscriberList.clear();
        _subscriberOnceList.clear();
        _topicCount = 0;
    };


    bool hasTopic(const std::string &topic) {
        return (_subscriberList.find(topic) != _subscriberList.end());
    };

};

#endif //PUBSUB_PUBSUB_H
