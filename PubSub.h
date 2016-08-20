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


typedef void (*topicFunctionPtr)(std::string topic, void * data);


class PubSub {
protected:
    void *_context;

private:

    typedef struct topic_data {
        topicFunctionPtr subscriber;
    } TopicData;

    std::map<const std::string, std::map<int, std::vector<TopicData> > > _subscriberList;

public:
    PubSub();

    virtual ~PubSub();

    void publish(std::string topic, void *data);

    void subscribe(const std::string &topic, topicFunctionPtr subscriber, int priority = 0);

    void unsubscribe(const std::string &topic, topicFunctionPtr subscriber);

    void unsubscribeAll(const std::string &topic);

    bool hasTopic(const std::string &topic);
};

#endif //PUBSUB_PUBSUB_H
