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


typedef std::function<void(std::string topic, void * topicOption)> TopicFunc;


class PubSub {
protected:
    void *_context;

private:

    typedef struct topic_data {
        TopicFunc subscriber;
    } TopicData;

    std::map<const std::string, std::map<int, std::vector<TopicData> > > _subscriberList;
    std::map<const std::string, std::vector<TopicData> > _subscriberOnceList;


public:
    PubSub();

    virtual ~PubSub();

    void publish(std::string topic, void *topicOption);

    void subscribe(const std::string &topic, TopicFunc subscriber, int priority = 0);

    void subscribeOnce(const std::string &topic, TopicFunc subscriber, int priority = 0);

    void unsubscribe(const std::string &topic);

    void unsubscribe(const std::string &topic, TopicFunc subscriber);

    void unsubscribeAll(const std::string &topic);

    bool hasTopic(const std::string &topic);

};

#endif //PUBSUB_PUBSUB_H
