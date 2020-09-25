#include "download.h"
#include "../tools/os.h"
#include "../app.h"

#include <assert.h>

namespace tools
{
    Download::Download(context::Patcher* main, std::string link) noexcept : up(main), file(link), state(0.f)
    {
        curl = curl_easy_init();
    }
    Download::~Download() noexcept
    {
        curl_easy_cleanup(curl);
    }
    void Download::run() noexcept
    {
        std::string link = up->ip;
        link += "/update/";
        link += file;

        std::string to = tools::getExecutablePath();
            to = to.substr(0, to.rfind("/", to.size()));
            to = "update/";
            to += file.c_str();
        local = to;

        printf("download: %s\n", link.c_str());
        if(curl)
        {
            outfile = fopen(to.c_str(), "w");
            if(!outfile)
            {
                printf("could not open %s\n", to.c_str());
                return;
            }

            curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, outfile);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DWrite);
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, DRead);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
            curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, DProgress);
            curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &up->progress);

            if(up->app->cfg->download_limit > 0)
            {
                curl_easy_setopt(
                    curl, CURLOPT_MAX_RECV_SPEED_LARGE,
                    up->app->cfg->download_limit*1000
                );
            }

            res = curl_easy_perform(curl);

            fclose(outfile);
            curl_easy_cleanup(curl);
        }
    }
    size_t Download::DWrite(void *ptr, size_t size, size_t nmemb, FILE *stream) noexcept
    {
        return fwrite(ptr, size, nmemb, stream);
    }
    size_t Download::DRead(void *ptr, size_t size, size_t nmemb, FILE *stream) noexcept
    {
        return fread(ptr, size, nmemb, stream);
    }
    int Download::DProgress(Gtk::ProgressBar *progress, double t,double d,double ultotal,double ulnow) noexcept
    {
        progress->set_fraction(d/t);
        return 0;
    }
}
