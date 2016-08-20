#C++ Classes PubSub

- PubSub
- EventDispatcher
- EventEmitter

## API

### INSTANCE METHODS

#### void publish(std::string topic, void *topicOption);

#### void subscribe(const std::string &topic, topicFunctionPtr subscriber, int priority = 0);

#### void subscribeOnce(const std::string &topic, topicFunctionPtr subscriber, int priority);

#### void unsubscribe(const std::string &topic);

#### void unsubscribe(const std::string &topic, topicFunctionPtr subscriber);

#### void unsubscribeAll(const std::string &topic);

#### bool hasTopic(const std::string &topic);

