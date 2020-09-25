#pragma once
#include <mutex>
#include <pthread.h>

namespace tools
{
    class IThreadWorker
    {
        public:
            virtual void run() = 0;
    };
    
    class Thread
    {
        public:
            Thread(IThreadWorker*, const std::string &name = "") noexcept;
            virtual ~Thread() noexcept;

            virtual bool start() noexcept;
            virtual void join() noexcept;
            virtual void shutdown() noexcept;

            virtual bool isRunning() const noexcept;
            virtual bool isRunning() noexcept;

            virtual int setPolicy(const int) noexcept;
        protected:
            std::string name;
            bool running;
            int policy;

            pthread_attr_t attr;
            pthread_t thread;

            IThreadWorker *work;
        protected:
            static void* threadFunc(void* args) noexcept;
            virtual void update() noexcept;
    };
}