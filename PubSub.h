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


template<
        typename Topic = std::string,
        typename TopicHandler = std::function<void(const Topic &topic, const Topic &str)>
>
class PubSub {
private:

    std::size_t _observerCount = 0; // TODO: カウント数の定義・使用用途が曖昧なため現状使用不可
    std::map<const Topic, std::map<int, std::vector<TopicHandler> > > _subscriberList;
    std::map<const Topic, std::unordered_set<TopicHandler *> > _subscriberOnceList;

public:

    template<class ...Args>
    void publish(const Topic &topic, Args &&... args) {

        if (!hasTopic(topic)) {
            return;
        }
        auto &topicList = _subscriberList[topic]; // std::map

        for (auto priorityList = topicList.begin(); priorityList != topicList.end(); ++priorityList) {
            auto &subscriberList = priorityList->second; // std::vector

            for (auto priorityItem = subscriberList.begin(); priorityItem != subscriberList.end();) {
                auto &topicHandler = *priorityItem; // TopicHandler
                topicHandler(topic, std::forward<Args>(args)...);

                if (_subscriberOnceList.count(topic)) {
                    if (_subscriberOnceList[topic].count(&topicHandler)) {
                        priorityItem = subscriberList.erase(
                                subscriberList.begin() + std::distance(subscriberList.begin(), priorityItem)
                        );
                        _subscriberOnceList[topic].erase(&topicHandler);
                    } else {
                        ++priorityItem;
                    }
                } else {
                    ++priorityItem;
                }
            }
        }

    };


    void subscribe(const Topic &topic, TopicHandler subscriber, int priority = 0) {
        _subscriberList[topic][priority].push_back(subscriber);
        ++_observerCount;
    };


    void subscribeOnce(const Topic &topic, TopicHandler subscriber, int priority = 0) {
        auto &listeners = _subscriberList[topic][priority];
        listeners.push_back(subscriber);
        _subscriberOnceList[topic].insert(&listeners.back());
        ++_observerCount;
    };


    void unsubscribe(const Topic &topic) {

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
        _observerCount = 0;
    };


    bool hasTopic(const Topic &topic) {
        return (_subscriberList.find(topic) != _subscriberList.end());
    };



    bool remove(const Topic &topic, TopicHandler &subscriber) {

        if (!hasTopic(topic)) {
            return false;
        }

        auto &topicList = _subscriberList[topic]; // std::map

        for (auto &priorityList : topicList) {
            auto &subscriberList = priorityList.second;

            for (auto priorityItem = subscriberList.begin(); priorityItem != subscriberList.end();) {
                if (&*priorityItem == &subscriber) {
                    subscriberList.erase(priorityItem);
                    --_observerCount;
                    return true;
                }
            }
        }

        return false;
    }



    std::size_t countListeners() const {
        return _observerCount;
    }


};

#endif //PUBSUB_PUBSUB_H
