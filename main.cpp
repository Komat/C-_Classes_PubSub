#include <iostream>
#include "PubSub.h"



PubSub<> pub_sub;

std::string TOPIC_LAUNCH = "topic_launch";

std::string TOPIC_RECEIVE = "topic_receive";



/**
 * 購読解除
 * @param data
 */
void unsubscribe_callback(const std::string a, const std::string b) {
    std::cout << "FIRST::" << a << b << std::endl;
//    pub_sub.unsubscribe(TOPIC_LAUNCH);
}

/**
 * 購読解除
 * @param data
 */
void unsubscribe_demo_callback(const std::string a, const std::string b) {
    std::cout << "SECOND::" << a << b << std::endl;
//    pub_sub.unsubscribe(TOPIC_RECEIVE);
}



int main() {


    pub_sub.subscribe(TOPIC_LAUNCH, unsubscribe_callback);
    pub_sub.publish(TOPIC_LAUNCH, "TOPIC LAUNCH 01");
    pub_sub.publish(TOPIC_LAUNCH, "TOPIC LAUNCH 02");

    pub_sub.subscribe(TOPIC_RECEIVE, unsubscribe_demo_callback);
    pub_sub.publish(TOPIC_RECEIVE, "TOPIC RECEIVE 01");
    pub_sub.publish(TOPIC_RECEIVE, "TOPIC RECEIVE 02");

    return 0;
}
