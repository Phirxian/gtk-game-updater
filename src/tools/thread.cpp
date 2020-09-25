#include "thread.h"
#include <string.h>
#include <iostream>
#include <thread>

namespace tools
{
    static unsigned int numberThread;
    int getNumberThread() noexcept { return numberThread; }
    int getHardwareConcurrency() noexcept { return std::thread::hardware_concurrency(); }

    Thread::Thread(IThreadWorker *w, const std::string &n) noexcept
        : running(false), work(w), name(n)
    {
        policy = SCHED_OTHER;
    }
    
    Thread::~Thread() noexcept
    {
        join();
    }
    
    int Thread::setPolicy(const int i) noexcept
    {
        #ifndef _WIN32
            if(running)
                pthread_setschedprio(thread, policy);
        #endif
        policy = i;
        return i;
    }
    
    bool Thread::start() noexcept
    {
        if(running) return 0;
        running = true;
        errno = pthread_create(&thread, 0, threadFunc, this);

        if(errno != 0)
        {
            std::cout << strerror(errno) << std::endl;
            return false;
        }

        pthread_setname_np(thread, name.c_str());
        setPolicy(policy);
        numberThread ++;

        #ifndef _WIN32
            return thread;
        #else
            return true;
        #endif
    }
    
    void Thread::join() noexcept
    {
        if(!running) return;
        running = false;

        std::cout << "waiting thread " << numberThread << std::endl;
        #ifndef _WIN32
        if(thread)
        #endif
            pthread_join(thread, NULL);
        numberThread --;
        #ifndef _WIN32
            thread = 0;
        #else
            running = false;
        #endif
    }
    
    void Thread::shutdown() noexcept
    {
        if(!running) return;
        running = false;
        #ifndef _WIN32
        if(thread)
        #endif

        pthread_cancel(thread);
        std::cout << "delete thread " << numberThread << std::endl;
        numberThread --;

        #ifndef _WIN32
            thread = 0;
        #else
            running = false;
        #endif
    }
    
    void Thread::update() noexcept
    {
        while(running)
            work->run();
    }
    
    bool Thread::isRunning() const noexcept
    {
        return running;
    }
    
    bool Thread::isRunning() noexcept
    {
        return running;
    }
    
    void* Thread::threadFunc(void* args) noexcept
    {
        Thread* pThread = reinterpret_cast<Thread*>(args);
        pThread->update();
        return 0;
    }
}
