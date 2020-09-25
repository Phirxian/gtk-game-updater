#pragma once

#include <gtkmm.h>
#include <curl/curl.h>
#include <curl/easy.h>

#include "thread.h"
#include "../context/patcher.h"

namespace context
{
    class Patcher;
}

namespace tools
{
    class Download : public IThreadWorker
    {
        public:
            Download(context::Patcher*, std::string) noexcept;
            virtual ~Download() noexcept;

            static size_t DWrite(void *ptr, size_t size, size_t nmemb, FILE *stream) noexcept;
            static size_t DRead(void *ptr, size_t size, size_t nmemb, FILE *stream) noexcept;
            static int DProgress(Gtk::ProgressBar *progress, double t,double d,double ultotal,double ulnow) noexcept;

            virtual void run() noexcept;
        public:
            float state;
            std::string file, local;
            context::Patcher *up;
        private:
            CURLcode res;
            CURL *curl;
            FILE *outfile;
    };
}
