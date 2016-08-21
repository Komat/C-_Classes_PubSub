#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>


typedef std::function< void()> TopicFunc;

class EventEmitter {
protected:
    void *_context;

private:
    std::unordered_map<std::string, std::map<int, std::vector<std::function< void() >> > > _subscriberList;
    std::unordered_map<std::string,  std::vector<std::function< void() >> > _subscriberOnceList;
    std::size_t _count = 0;


public:
    EventEmitter() {
        _context = this;
    };

    ~EventEmitter() {
        _context = NULL;
    };

    template<class ...Args>
    void emit(std::string topic, Args && ... args ) {
        if(!has(topic)) {
            return;
        }
        std::map<int, std::vector<std::function< void() >> > &topicList = _subscriberList[topic];
        for (std::map<int, std::vector<std::function< void() >> >::reverse_iterator priorityList = topicList.rbegin(); priorityList != topicList.rend(); ++priorityList) {
            std::vector<std::function< void() >> &subscriberList = priorityList->second;
            std::vector<std::function< void() >>::reverse_iterator priorityItem;

            for (priorityItem = subscriberList.rbegin(); priorityItem != subscriberList.rend(); ++priorityItem) {
                (*priorityItem)(std::forward<Args>(args)...);

                if (_subscriberOnceList.count(topic)) {
                    if (!_subscriberOnceList[topic].empty()) {
                        std::vector<std::function< void() >>::iterator onceItem = _subscriberOnceList[topic].begin();
                        while (onceItem != _subscriberOnceList[topic].end()) {
                            if ((*onceItem) == (*priorityItem)) {
                                off(topic, (*priorityItem));
                                onceItem = _subscriberOnceList[topic].erase(onceItem);
                            } else {
                                ++onceItem;
                            }
                        }
                    }
                }
            }
        }

    };

    void on(const std::string &topic, std::function< void()> subscriber, int priority = 0) {
        _subscriberList[topic][priority].push_back(subscriber);
        ++_count;
    };

    void once(const std::string &topic, std::function< void()> subscriber, int priority = 0) {
        _subscriberList[topic][priority].push_back(subscriber);
        _subscriberOnceList[topic].push_back(subscriber);
        ++_count;
    };


    void off(const std::string &topic) {
    };

    void off(const std::string &topic, std::function<void()> subscriber) {
        std::map<int, std::vector<std::function< void()>> > &topicList = _subscriberList[topic];

        for (std::map<int, std::vector<std::function< void()>> >::reverse_iterator priorityList = topicList.rbegin(); priorityList != topicList.rend(); ++priorityList) {

            std::vector<std::function< void()>> &subscriberList = priorityList->second;
            std::vector<std::function< void()>>::iterator priorityItem;

            for (priorityItem = subscriberList.begin(); priorityItem != subscriberList.end();) {
                if (priorityItem == subscriber) {
                    priorityItem = subscriberList.erase(subscriberList.begin() + std::distance(subscriberList.begin(), priorityItem));
                    continue;
                }
                ++priorityItem;
            }
        }
        if (topicList.empty()) _subscriberList.erase(topic);
    };

    void offAll() {
        _subscriberList.clear();
        _subscriberOnceList.clear();
        _count = 0;
    };

    void offAll(const std::string &topic) {
        if (!has(topic)) return;

        std::map<int, std::vector<std::function< void()>> > &topicList = _subscriberList[topic];
        std::map<int, std::vector<std::function< void()>> >::iterator priorityList = topicList.begin();

        while (priorityList != topicList.end()) {
            topicList.erase(priorityList++);
        }

        if (topicList.empty()) _subscriberList.erase(topic);
    };

    bool has(const std::string &topic) {
        return (_subscriberList.find(topic) != _subscriberList.end());
    };

};


int main() {

    EventEmitter ee;

    ee.on("event1", [](int a, std::string b) {
        std::cout << "EVENT1(01) >> " << a << " : " << b<< std::endl;
    });

    ee.on("event1", [](int a, std::string b) {
        std::cout << "EVENT1(02) >> " << a << " : " << b<< std::endl;
    });

    ee.emit("event1", 10, (std::string) "foo");

    ee.on("event2", []() {
        std::cout << "EVENT2(01)" << std::endl;
    });

    ee.emit("event2");

    int count1 = 0;

    ee.on("event3", [&]() {
        std::cout << "EVENT3(01) >> " << count1 << std::endl;
    });

    for (int i = 0; i < 10; i++) {
        ee.emit("event3");
    }

    int count2 = 0;
    ee.on("event4", [&](){
        count2++;
        std::cout << "EVENT4(01) >> " << count2 << std::endl;
    });

    ee.emit("event4");
    ee.off("event4");
    ee.emit("event4");
    ee.emit("event4");

    int count3 = 0;
    ee.on("event5", [&]() {
        count3++;
        std::cout << "EVENT5(01) >> " << count3 << std::endl;
    });

    return 0;
}
