/*!************************************************************************
\file			EventSystem.h
\author			Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:	CSD1171B
\par Software Engineering Project
\date			12-01-2023
\brief
This header file declares a generic event system that allows functions with
no parameters/ 1 parameter of any type.  

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

All content © 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

#pragma once
///////////////////////////////////////////////////////////////////////////
#include <list>
///////////////////////////////////////////////////////////////////////////

namespace EventSystem {
	///////////////////////////////////////////////////////////////////////
	// Struct for each element in the event queue
	template<class T>
	struct EventData {
		void (*fun_ptr)(T);
		int priority;
	};

	// Void specialization (functions with no parameters)
	template<>
	struct EventData<void> {
		void (*fun_ptr)();
		int priority;
	};

	///////////////////////////////////////////////////////////////////////
	// Handles a list of events for a type
	template<class T>
	class Event {
	private:
		std::list<EventData<T>> subscribers; // All the functions subscribed to *this* event

		bool isInvoking;
		bool shouldRemove;
		typename std::list<EventData<T>>::iterator eventToRemove;

	public:
		// Call each function subscribe with the parameter (var) passed here
		void Invoke(T var) {
			isInvoking = true;

			for (typename std::list<EventData<T>>::iterator i = subscribers.begin(); i != subscribers.end(); ++i) {
				(i->fun_ptr)(var);
			}

			if (shouldRemove) {
				shouldRemove = false;
				subscribers.erase(eventToRemove);
			}
			isInvoking = false;
		}

		// Add or "subscribe" a function to this event list
		void Subscribe(void (*fun_ptr)(T), int priority = 0) {
			EventData<T> new_sub = { fun_ptr, priority };

			if (subscribers.size() >= 1) {
				for (typename std::list<EventData<T>>::iterator i = subscribers.begin(); i != subscribers.end(); ++i) {
					if (i->priority <= priority) {
						subscribers.insert(i, new_sub);
						return;
					}
				}
			}
			subscribers.push_back(new_sub);
		}

		// Remove or "unsubscribe" a function from this event list
		void Unsubscribe(void (*fun_ptr)(T)) {
			for (typename std::list<EventData<T>>::iterator i = subscribers.begin(); i != subscribers.end(); ++i) {
				if (i->fun_ptr == fun_ptr) {
					if (isInvoking) {
						eventToRemove = i;
						shouldRemove = true;
					}
					else {
						subscribers.erase(i);
					}
					break;
				}
			}
		}

		// Remove all functions from this event list
		void UnsubscribeAll() {
			subscribers.clear();
		}
	};

	// Void specialization (functions with no parameters)
	template<>
	class Event<void> {
	private:
		std::list<EventData<void>> subscribers; // All the functions subscribed to *this* event

		bool isInvoking;
		bool shouldRemove;
		typename std::list<EventData<void>>::iterator eventToRemove;

	public:
		// Call each function subscribe with the parameter (var) passed here
		void Invoke() {
			isInvoking = true;

			for (typename std::list<EventData<void>>::iterator i = subscribers.begin(); i != subscribers.end(); ++i) {
				(i->fun_ptr)();
			}

			if (shouldRemove) {
				shouldRemove = false;
				subscribers.erase(eventToRemove);
			}
			isInvoking = false;
		}

		// Add or "subscribe" a function to this event list
		void Subscribe(void (*fun_ptr)(), int priority = 0) {
			EventData<void> new_sub = { fun_ptr, priority };

			if (subscribers.size() >= 1) {
				for (typename std::list<EventData<void>>::iterator i = subscribers.begin(); i != subscribers.end(); ++i) {
					if (i->priority <= priority) {
						subscribers.insert(i, new_sub);
						return;
					}
				}
			}

			subscribers.push_back(new_sub);
		}

		// Remove or "unsubscribe" a function from this event list
		void Unsubscribe(void (*fun_ptr)()) {
			for (typename std::list<EventData<void>>::iterator i = subscribers.begin(); i != subscribers.end(); ++i) {
				if (i->fun_ptr == fun_ptr) {
					if (isInvoking) {
						eventToRemove = i;
						shouldRemove = true;
					}
					else {
						subscribers.erase(i);
					}
					break;
				}
			}
		}

		// Remove all functions from this event list
		void UnsubscribeAll() {
			subscribers.clear();
		}
	};
}