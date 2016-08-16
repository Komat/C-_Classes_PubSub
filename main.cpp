#include <iostream>
#include "PubSub.h"


PubSub pub_sub;

const std::string INITIALIZE = "initialize";


void subscriber(const std::string topic, void *data) {
    std::cout << ">>>>>>>>>>>>>>>>>>" << std::endl;
    std::cout << topic << std::endl;
    std::cout << (char *) data << std::endl;
    std::cout << ">>>>>>>>>>>>>>>>>>" << std::endl;
    pub_sub.unsubscribe(INITIALIZE, (topicFunctionPtr) subscriber);
}


void subSubscriber(const std::string topic, void *data) {
    std::cout << "///////////////////////////////" << std::endl;
    std::cout << "///////////////////////////////" << std::endl;
    std::cout << topic << std::endl;
    std::cout << (char *) data << std::endl;
    std::cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\" << std::endl;
    std::cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\" << std::endl;
}


int main() {

    std::cout << "READY" << std::endl;

    pub_sub.subscribe(INITIALIZE, (topicFunctionPtr) subscriber);
    pub_sub.subscribe(INITIALIZE, (topicFunctionPtr) subSubscriber);

    char charData[] = "TEST";

    pub_sub.publish(INITIALIZE, &charData);

    pub_sub.publish(INITIALIZE, &charData);



    return 0;
}