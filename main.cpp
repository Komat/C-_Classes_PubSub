#include <iostream>
#include "PubSub.h"


using TopicHandlerT = std::function< void(const std::string &topic, const std::string & str) >;


PubSub<TopicHandlerT> pub_sub;

std::string INITIALIZE = "initialize";
std::string DEINITIALIZE = "deinitialize";

void subscriber01(const std::string &topic, const std::string & str) {
    std::cout << "(01) >>>>>>>>>>>>>" << topic << "::" << str  <<  std::endl;
}


void subscriber02(const std::string &topic, const std::string & str) {
    std::cout << "(02) /////////////" << topic << "::" << str  << std::endl;
}

void subscriber03(const std::string &topic, const std::string & str) {
    std::cout << "(03) -------------" << topic << "::" << str  << std::endl;
}

void subscriber04(const std::string &topic, const std::string & str) {
    std::cout << "(04) :::::::::::::" << topic << "::" << str  << std::endl;
}


void subscriber05(const std::string &topic, const std::string & str) {
    std::cout << "(05) ~~~~~~~~~~~~" << topic << "::" << str  << std::endl;
}






int main() {


    std::cout << "\n[ READY ]\n" << std::endl;


    pub_sub.subscribe(INITIALIZE, subscriber01, 1);
    pub_sub.subscribe(INITIALIZE, subscriber02, 4);
    pub_sub.subscribe(INITIALIZE, subscriber03, 3);
    pub_sub.subscribe(INITIALIZE, subscriber04, 2);
    pub_sub.subscribeOnce(INITIALIZE, subscriber05, 2);

    pub_sub.subscribe(DEINITIALIZE, subscriber01, 1);
    pub_sub.subscribe(DEINITIALIZE, subscriber02, 4);
    pub_sub.subscribe(DEINITIALIZE, subscriber03, 3);
    pub_sub.subscribe(DEINITIALIZE, subscriber04, 2);
    pub_sub.subscribeOnce(DEINITIALIZE, subscriber05, 2);

    std::cout << "\n[ PLAY01 START ]\n" << std::endl;

    pub_sub.publish(INITIALIZE, "PARAMS");
    pub_sub.publish(DEINITIALIZE, "PARAMS");

    std::cout << "\n[ PLAY01 END ]" << std::endl;
    std::cout << "[ PLAY02 START ]\n" << std::endl;

    pub_sub.publish(INITIALIZE, "PARAMS");
    pub_sub.publish(DEINITIALIZE, "PARAMS");

    pub_sub.unsubscribe(DEINITIALIZE);

    std::cout << "\n[ PLAY02 END ]" << std::endl;
    std::cout << "[ PLAY03 START ]\n" << std::endl;

    pub_sub.publish(INITIALIZE, "PARAMS");
    pub_sub.publish(DEINITIALIZE, "PARAMS");

    pub_sub.unsubscribeAll();

    std::cout << "\n[ PLAY03 END ]" << std::endl;
    std::cout << "[ PLAY04 START ]\n" << std::endl;

    pub_sub.publish(INITIALIZE, "PARAMS");
    pub_sub.publish(DEINITIALIZE, "PARAMS");

    std::cout << "\n[ PLAY04 END ]\n" << std::endl;


    return 0;
}