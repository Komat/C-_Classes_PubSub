/**
 * @description
 * @fileName PubSub.h.
 * @author komat
 * @date 8/16/16.
 * @version 0.0
 */

#ifndef PUBSUB_PUBSUB_H
#define PUBSUB_PUBSUB_H

#include <functional>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <list>

template<
        class EventType = std::string,
        class Listener = std::function< void() >
>
class PubSub
{
public:

    using Once = bool;
    using EventEmitterT = PubSub< EventType, Listener >;
    using Listeners = std::list< Listener >;
    using Range = std::pair< typename Listeners::iterator, typename Listeners::iterator >;
    using ListenersMap = std::unordered_map< EventType, Listeners >;
    using OnceMap = std::unordered_set< const Listener * >;

public:

    PubSub( const PubSub & ) = delete;
    PubSub & operator= ( const PubSub & ) = delete;

    PubSub() {}

    PubSub( EventEmitterT && ee )
            :
            _map( std::move( ee._map ) ),
            _onceMap( std::move( ee._onceMap) ),
            _countListeners( ee._countListeners )
    {}

    EventEmitterT & on( const EventType & event, const Listener & listener )
    {
        _map[ event ].push_back( listener );
        ++_countListeners;
        return *this;
    }

    EventEmitterT & subscribeOnce( const EventType & event, const Listener & listener )
    {
        auto & listeners = _map[ event ];
        listeners.push_back( listener );
        ++_countListeners;
        _onceMap.insert( &listeners.back() );

        return *this;
    }

    template<class ...Args>
    void publish(const EventType &event, Args &&... args)
    {
        auto it = _map.find( event );
        if( it != _map.end() )
        {
            auto & listeners = it->second;
            auto itListeners = listeners.begin();
            auto itEndListeners = listeners.end();

            while( itListeners != itEndListeners ) {
                auto &listener = *itListeners;
                listener(std::forward<Args>(args)...);

                if( _onceMap.count( &listener ) )
                {
                    itListeners = listeners.erase(itListeners);
                    --_countListeners;
                    _onceMap.erase( &listener );
                }
                ++itListeners;
            }
        }
    }

    void off()
    {
        _map.clear();
        _onceMap.clear();
        _countListeners = 0;
    }

    void off( const EventType & event )
    {
        auto it = _map.find( event );
        if( it != _map.end() )
        {
            auto s = it->second.size();
            _countListeners -= s;
            _map.erase( it );

            if( _map.empty() )
            {
                _onceMap.clear();
            }
        }
    }

    Range listeners( const EventType & event )
    {
        auto it = _map.find( event );
        if( it == _map.end() )
        {
            return Range{};
        }

        return Range( it->second.begin(), it->second.end() );
    }

    Listener * listener( const EventType & event )
    {
        auto it = _map.find( event );
        if( it != _map.end() )
        {
            auto & listeners = it->second;
            return &listeners.front();
        }

        return nullptr;
    }

    std::size_t countListeners() const
    {
        return _countListeners;
    }

    bool remove( const Listener & listener )
    {
        for( auto & p : _map )
        {
            auto & listeners = p.second;
            auto it = std::begin( listeners );
            auto itEnd = std::end( listeners );

            for( ; it != itEnd; ++it )
            {
                if( &*it == &listener )
                {
                    listeners.erase( it );
                    --_countListeners;
                    return true;
                }
            }
        }

        return false;
    }

private:

    ListenersMap _map;
    OnceMap _onceMap;
    std::size_t _countListeners = 0;
};


#endif //PUBSUB_PUBSUB_H
