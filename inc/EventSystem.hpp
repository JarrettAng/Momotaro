/*!************************************************************************
\file EventSystem.hpp
\author Ang Jiawei Jarrett
\par DP email: a.jiaweijarrett@digipen.edu
\par Course: CSD1171B
\date 12/01/2023
\brief
This header file declares

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
		void Invoke(T var);

		void Subscribe(void (*fun_ptr)(T), int priority = 0);

		void Unsubscribe(void (*fun_ptr)(T));

		void UnsubscribeAll();
	};

	template<>
	class Event<void> {
	private:
		std::list<EventData<void>> subscribers;

	public:
		void Invoke();

		void Subscribe(void (*fun_ptr)(), int priority = 0);

		void Unsubscribe(void (*fun_ptr)());

		void UnsubscribeAll();
	};
}