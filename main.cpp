#include <iostream>
#include <vector>
#include <map>


typedef void (*topicFunctionPtr)(std::string topic, void *data);

typedef struct topic_data {
    topicFunctionPtr subscriber;
} TopicData;


std::map<const std::string, std::map<int, std::vector<TopicData> > > _subscriberList;


bool hasTopic(const std::string &topic) {
    return (_subscriberList.find(topic) != _subscriberList.end());
}

void subscribe(const std::string &topic, topicFunctionPtr subscriber, int priority = 0) {
    TopicData data;
    data.subscriber = subscriber;
    _subscriberList[topic][priority].push_back(data);
}


void unsubscribe(const std::string &topic, topicFunctionPtr subscriber) {
    if (!hasTopic(topic)) return;

    int counter = 0;

    std::map<int, std::vector<TopicData> > &list = _subscriberList[topic]; // priority Array

    for (std::map<int, std::vector<TopicData> >::reverse_iterator i = list.rbegin(); i != list.rend(); ++i) {
        std::vector<TopicData> &funcList = i->second;

        for (std::vector<TopicData>::iterator data = funcList.begin(); data != funcList.end(); ++data) {
            if (data->subscriber == subscriber) {
                funcList.erase(funcList.begin() + counter);
            }

            counter++;

        }
    }

    if (list.empty()) _subscriberList.erase(topic);
}



void unsubscribeAll(const std::string &topic) {
    if (!hasTopic(topic)) return;

    std::map<int, std::vector<TopicData> > &list = _subscriberList[topic];
    std::map<int, std::vector<TopicData> >::iterator data = list.begin();

    while (data != list.end()) {
        list.erase(data);
        if (data->second.empty()) {
            list.erase(data++);
        } else {
            ++data;
        }
    }

    if (list.empty()) _subscriberList.erase(topic);
}



void publish(std::string topic, void *data) {

    if (!hasTopic(topic)) {
        return;
    }

    std::map<int, std::vector<TopicData> > &list = _subscriberList[topic];

    for (std::map<int, std::vector<TopicData> >::reverse_iterator i = list.rbegin(); i != list.rend(); ++i) {
        std::vector<TopicData> &funcList = i->second;
        std::vector<TopicData>::reverse_iterator iterator;

        for (iterator = funcList.rbegin(); iterator != funcList.rend(); ++iterator)
        {
            (*iterator->subscriber)(topic, data);
        }
    }

}


const std::string INITIALIZE = "initialize";


void subscribeHandler(const std::string topic, void *data) {
    std::cout << "(01) >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
//    unsubscribe(INITIALIZE, (topicFunctionPtr) subscribeHandler);
}

void subSubscriber(const std::string topic, void *data) {
    std::cout << "(02) ///////////////////////////////" << std::endl;
}

void subSubSubscriber(const std::string topic, void *data) {
    std::cout << "(03) -------------------------------" << std::endl;
}
int main() {


    subscribe(INITIALIZE, (topicFunctionPtr) subscribeHandler, 2);
    subscribe(INITIALIZE, (topicFunctionPtr) subSubscriber, 0);
    subscribe(INITIALIZE, (topicFunctionPtr) subSubSubscriber, 1);


    char charData[] = "TEST";


    publish(INITIALIZE, charData);
    publish(INITIALIZE, charData);
    publish(INITIALIZE, charData);



    return 0;
}

