//--------------------------------------------------------------------------------
// Copyright (c) 2017-2020, sanko-shoko. All rights reserved.
//--------------------------------------------------------------------------------

#ifndef __SP_THREAD_H__
#define __SP_THREAD_H__


//--------------------------------------------------------------------------------
// thread
//--------------------------------------------------------------------------------

#include <thread>
#include <mutex>
#include <functional>

namespace sp {

    class Thread {
    private:
        bool m_used;
        bool m_init;
        std::mutex m_mtx;

    public:
        Thread() {
            init();
            m_used = false;
        }

        void init() {
            m_init = true;
        }

        void freeze() {
            std::lock_guard<std::mutex> lock(m_mtx);
            m_init = false;
        }

        bool used() {
            return m_init == false || m_used == true;
        }

        std::mutex& mutex() {
            return m_mtx;
		}

        template<class Class, void (Class::*Func)()>
        bool run(Class *ptr, const bool wait = true) {
            if (m_init == false) return false;
            if (wait == false && m_used == true) return false;

            std::thread th([this, ptr, wait] {
                std::lock_guard<std::mutex> lock(m_mtx);
                m_used = true;
                (ptr->*Func)();
                m_used = false;
            });
            th.detach();
            return true;
        }

        bool run(std::function<void()> func, const bool wait = true) {
            if (m_init == false) return false;
            if (wait == false && m_used == true) return false;

            std::thread th([this, func, wait] {
                std::lock_guard<std::mutex> lock(m_mtx);
                m_used = true;
                func();
                m_used = false;
            });
            th.detach();
            return true;
        }

        bool run(void (*func)(), const bool wait = true) {
            if (m_init == false) return false;
            if (wait == false && m_used == true) return false;

            std::thread th([this, func, wait] {
                std::lock_guard<std::mutex> lock(m_mtx);
                m_used = true;
                func();
                m_used = false;
            });
            th.detach();
            return true;
        }
    };
}

#endif

