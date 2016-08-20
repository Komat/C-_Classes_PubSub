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

void subscriber04(const std::string topic, void *data) {
    std::cout << "(04) :::::::::::::" << topic << " : " << (char *) data << std::endl;
}


void subscriber05(const std::string topic, void *data) {
    std::cout << "(05) ~~~~~~~~~~~~" << topic << " : " << (char *) data << std::endl;
}


int main() {


    std::cout << "\n[ READY ]\n" << std::endl;

    pub_sub.subscribe(INITIALIZE, (topicFunctionPtr) subscriber01, 1);
    pub_sub.subscribe(INITIALIZE, (topicFunctionPtr) subscriber02, 4);
    pub_sub.subscribe(INITIALIZE, (topicFunctionPtr) subscriber03, 3);
    pub_sub.subscribe(INITIALIZE, (topicFunctionPtr) subscriber04, 2);
    pub_sub.subscribeOnce(INITIALIZE, (topicFunctionPtr) subscriber05, 2);

    char charData[] = "PARAMS";

    pub_sub.publish(INITIALIZE, &charData);

    std::cout << "\n[ PLAY01 ]\n" << std::endl;

    pub_sub.publish(INITIALIZE, &charData);

    pub_sub.unsubscribeAll(INITIALIZE);

    std::cout << "\n[ PLAY02 ]\n" << std::endl;

    pub_sub.publish(INITIALIZE, &charData);

    return 0;
}