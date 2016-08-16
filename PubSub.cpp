/**
 * @description
 * @fileName PubSub.cpp.
 * @author komatsu
 * @date 8/16/16.
 * @version 0.0
 */

#include "PubSub.h"

PubSub::PubSub() {
    _context = this;
}

PubSub::~PubSub() {
    _context = NULL;
}

void PubSub::publish(std::string topic, void *data) {

    if(!hasTopic(topic)) {
        return;
    }

    std::map<int, std::vector<TopicData> > &list = _subscriberList[topic];

    for(std::map<int, std::vector<TopicData> >::reverse_iterator i = list.rbegin(); i != list.rend(); ++i) {
        std::vector<TopicData> &funcList = i->second;
        for(std::vector<TopicData>::iterator func = funcList.begin(); func != funcList.end(); ++func){
            (*func->subscriber)(topic, data);
        }
    }

}

void PubSub::subscribe(const std::string &topic, topicFunctionPtr subscriber, int priority) {

    TopicData data;
    data.subscriber = subscriber;

    _subscriberList[topic][priority].push_back(data);

}

void PubSub::unsubscribe(const std::string &topic, topicFunctionPtr subscriber) {

    if (!hasTopic(topic)) return;

    std::map<int, std::vector<TopicData> > &allFunctions = _subscriberList[topic];
    std::map<int, std::vector<TopicData> >::iterator it = allFunctions.begin();

    while (it != allFunctions.end()) {
        allFunctions.erase(it);
        if (it->second.empty()) {
            allFunctions.erase(it++);
        } else {
            ++it;
        }
    }

    if (allFunctions.empty()) _subscriberList.erase(topic);

}

bool PubSub::hasTopic(const std::string &topic) {
    return (_subscriberList.find(topic) != _subscriberList.end());
}
