/*!************************************************************************
\file EventSystem.hpp
\author Ang Jiawei Jarrett
\par DP email: a.jiaweijarrett@digipen.edu
\par Course: CSD1171B
\date 12/01/2023
\brief
This header file declares a generic event system that allows functions with
no parameters/ 1 parameter of any type.  
Below are 2 usage examples of these events.

======== Example 1: Printing score to cout whenever it updates ========

Step 1: Define the event variable wherever the score is handled
EventSystem::Event<int> onScoreUpdate;

Step 2: In the printing file, subscribe to event (You can expose it in a header file or something)
void start() {
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

======== Example 2: Have fireworks play once on the first building placed ========

Step 1: Define the event wherever the building placing is handled
EventSystem::Event<void> onBuildingPlaced;

Step 2: In the file that plays fireworks, subscribe to event (You can expose it in a header file or something)
void start() {
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
**************************************************************************/
#pragma once

#include <list>

namespace EventSystem {
	template<class T>
	struct EventData {
		void (*fun_ptr)(T);
		int priority;
	};

	template<>
	struct EventData<void> {
		void (*fun_ptr)();
		int priority;
	};

	template<class T>
	class Event {
		private:
			std::list<EventData<T>> subscribers;

		public:
			void Invoke(T var) {
				for (auto i = subscribers.begin(); i != subscribers.end(); ++i) {
					(i->fun_ptr)(var);
				}
			}

			void Subscribe(void (*fun_ptr)(T), int priority = 0) {
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

			void Unsubscribe(void (*fun_ptr)(T)) {
				for (auto i = subscribers.begin(); i != subscribers.end(); ++i) {
					if (i->fun_ptr == fun_ptr) {
						subscribers.erase(i);
						break;
					}
				}
			}

			void UnsubscribeAll() {
				subscribers.clear();
			}
	};

	template<>
	class Event<void> {
		private:
			std::list<EventData<void>> subscribers;

		public:
			void Invoke() {
				for (auto i = subscribers.begin(); i != subscribers.end(); ++i) {
					(i->fun_ptr)();
				}
			}

			void Subscribe(void (*fun_ptr)(), int priority = 0) {
				EventData<void> new_sub = { fun_ptr, priority };

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

			void Unsubscribe(void (*fun_ptr)()) {
				for (auto i = subscribers.begin(); i != subscribers.end(); ++i) {
					if (i->fun_ptr == fun_ptr) {
						subscribers.erase(i);
						break;
					}
				}
			}

			void UnsubscribeAll() {
				subscribers.clear();
			}
	};
}