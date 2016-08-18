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


     std::cout << "SUBSCRIBER :: " << &subscriber << " : " << &data.subscriber << std::endl;


    _subscriberList[topic][priority].push_back(data);
}


void unsubscribe(const std::string &topic, topicFunctionPtr subscriber) {
    if (!hasTopic(topic)) return;

    std::map<int, std::vector<TopicData> > &list = _subscriberList[topic];

    for (std::map<int, std::vector<TopicData> >::reverse_iterator i = list.rbegin(); i != list.rend(); ++i) {
        std::vector<TopicData> &funcList = i->second;
        for (std::vector<TopicData>::iterator func = funcList.begin(); func != funcList.end(); ++func) {

            std::cout << "UNSUBSCRIBE :: " << &(func->subscriber) << " : " << &subscriber << std::endl;
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
        for (std::vector<TopicData>::iterator func = funcList.begin(); func != funcList.end(); ++func) {

            std::cout << "PUBLISH :: " << &(func->subscriber) << std::endl;

            (*func->subscriber)(topic, data);
        }
    }

}


const std::string INITIALIZE = "initialize";


void subscribeHandler(const std::string topic, void *data) {
    unsubscribe(INITIALIZE, (topicFunctionPtr) subscribeHandler);
}

int main() {


    subscribe(INITIALIZE, (topicFunctionPtr) subscribeHandler);


    char charData[] = "TEST";


    publish(INITIALIZE, charData);



    return 0;
}

