// 2 usage examples of events
/*
    === Example 1: Printing score to cout whenever it updates ===

    Step 1: Define the event wherever the score is handled
    EventSystem::Event<int> onScoreUpdate;

    Step 2: In the printing file, subscribe to event
    void printScore() {
        onScoreUpdate.Subscribe(scorePrintFunc);

        -or-

        onScoreUpdate.Subscribe(scorePrintFunc, 10); // Optional: second parameter for priority
    }

    Step 3: Whoever is handling the score will invoke/call the event
    void addScore() {
        ...
        onScoreUpdate.Invoke(score);
    }

    Step 4: All functions subscribed to the event will be called in order of priority
    [console] new score: 100


    === Example 2: Have fireworks play once on the first building placed ===

    Step 1: Define the event wherever the building placing is handled
    EventSystem::Event<void> onBuildingPlaced;

    Step 2: In the file, subscribe to event
    void printScore() {
        onBuildingPlaced.Subscribe(launchFireworks);

        -or-

        onBuildingPlaced.Subscribe(launchFireworks, -50); // Optional: second parameter for priority
    }

    Step 3: Whoever is handling the building placement will invoke/call the event
    void placeBuilding() {
        ...
        onBuildingPlaced.Invoke();
    }

    Step 4: In the function subscribed to the event, unsubscribe itself
    void launchFireworks() {
        ...
        onBuildingPlaced.Unsubscribe(launchFireworks);
    }
*/

#include <iostream>

namespace EventSystem {
    template<class T> struct EventData {
        void (*fun_ptr)(T);
        int priority;
    };

    template<class T> class Event {
    private: std::list<EventData<T>> subscribers;

    public: void Invoke(T var) {
        for (auto i = subscribers.begin(); i != subscribers.end(); ++i) {
            (i->fun_ptr)(var);
        }
    }

    public: void Subscribe(void (*fun_ptr)(T), int priority) {
        EventData<T> new_sub = { fun_ptr, priority };

        if (subscribers.size() >= 1) {
            for (auto i = subscribers.begin(); i != subscribers.end(); ++i) {
                if (i->priority <= priority) {
                    subscribers.insert(i, new_sub);
                    return;
                }
            }
        }

        subscribers.push_back(new_sub);
    }

    public: void Subscribe(void (*fun_ptr)(T)) {
        Subscribe(fun_ptr, 0);
    }

    public: void Unsubscribe(void (*fun_ptr)(T)) {
        for (auto i = subscribers.begin(); i != subscribers.end(); ++i) {
            if (i->fun_ptr == fun_ptr) {
                subscribers.erase(i);
                break;
            }
        }
    }

    public: void UnsubscribeAll() {
        subscribers.clear();
    }
    };
}
