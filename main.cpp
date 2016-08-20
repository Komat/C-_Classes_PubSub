#include <iostream>
#include "PubSub.h"


PubSub pub_sub;

const std::string INITIALIZE = "initialize";


void subscriber01(const std::string topic, void *data) {
    std::cout << "(01) >>>>>>>>>>>>>" << topic << " : " << (char *) data <<  std::endl;
    pub_sub.unsubscribe(INITIALIZE, (topicFunctionPtr) subscriber01);
}


void subscriber02(const std::string topic, void *data) {
    std::cout << "(02) /////////////" << topic << " : " << (char *) data << std::endl;
    pub_sub.unsubscribe(INITIALIZE, (topicFunctionPtr) subscriber02);
}

void subscriber03(const std::string topic, void *data) {
    std::cout << "(03) -------------" << topic << " : " << (char *) data << std::endl;
}


int main() {


    std::cout << "\n[ READY ]\n" << std::endl;

    pub_sub.subscribe(INITIALIZE, (topicFunctionPtr) subscriber01, 1);
    pub_sub.subscribe(INITIALIZE, (topicFunctionPtr) subscriber02, 4);
    pub_sub.subscribe(INITIALIZE, (topicFunctionPtr) subscriber03, 3);

    char charData[] = "PARAMS";

    pub_sub.publish(INITIALIZE, &charData);

    std::cout << "\n[ PLAY ]\n" << std::endl;

    pub_sub.publish(INITIALIZE, &charData);

    return 0;
}