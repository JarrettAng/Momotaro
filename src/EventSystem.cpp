/*!************************************************************************
\file EventSystem.cpp
\author Ang Jiawei Jarrett
\par DP email: a.jiaweijarrett@digipen.edu
\par Course: CSD1171B
\date 12/01/2023
\brief
This source file implements a generic event system that allows functions with
no parameters/ 1 parameter of any type. 
**************************************************************************/

//#include <EventSystem.h>

namespace EventSystem {
    /*
        EVENT CLASS IMPLEMENTATION
    */
    //template <class T>
    //void Event<T>::Invoke(T var) {
    //    for (auto i = subscribers.begin(); i != subscribers.end(); ++i) {
    //        (i->fun_ptr)(var);
    //    }
    //}
    //
    //template <class T>
    //void Event<T>::Subscribe(void (*fun_ptr)(T), int priority) {
    //    EventData<T> new_sub = { fun_ptr, priority };

    //    if (subscribers.size() >= 1) {
    //        for (auto i = subscribers.begin(); i != subscribers.end(); ++i) {
    //            if (i->priority <= priority) {
    //                subscribers.insert(i, new_sub);
    //                return;
    //            }
    //        }
    //    }
    //    subscribers.push_back(new_sub);
    //}

    //template <class T>
    //void Event<T>::Unsubscribe(void (*fun_ptr)(T)) {
    //    for (auto i = subscribers.begin(); i != subscribers.end(); ++i) {
    //        if (i->fun_ptr == fun_ptr) {
    //            subscribers.erase(i);
    //            break;
    //        }
    //    }
    //}

    //template <class T>
    //void Event<T>::UnsubscribeAll() {
    //    subscribers.clear();
    //}

    ///*
    //    VOID SPECIALIZATION
    //*/
    //void Event<void>::Invoke() {
    //    for (auto i = subscribers.begin(); i != subscribers.end(); ++i) {
    //        (i->fun_ptr)();
    //    }
    //}

    //void Event<void>::Subscribe(void (*fun_ptr)(), int priority) {
    //    EventData<void> new_sub = { fun_ptr, priority };

    //    if (subscribers.size() >= 1) {
    //        for (auto i = subscribers.begin(); i != subscribers.end(); ++i) {
    //            if (i->priority <= priority) {
    //                subscribers.insert(i, new_sub);
    //                return;
    //            }
    //        }
    //    }

    //    subscribers.push_back(new_sub);
    //}

    //void Event<void>::Unsubscribe(void (*fun_ptr)()) {
    //    for (auto i = subscribers.begin(); i != subscribers.end(); ++i) {
    //        if (i->fun_ptr == fun_ptr) {
    //            subscribers.erase(i);
    //            break;
    //        }
    //    }
    //}

    //void Event<void>::UnsubscribeAll() {
    //    subscribers.clear();
    //}
}
