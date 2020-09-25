#pragma once

#include "../tools/thread.h"
#include "../tools/minitar.h"
#include "../tools/download.h"

#include <mysql/mysql.h>
#include <vector>
//#include <zlib.h>

class App;

namespace context
{
    class Patcher : public tools::IThreadWorker
    {
        public:
            Patcher(App *a);
            ~Patcher();

            virtual void logMsg(std::string) noexcept;

            virtual void setProgressBar(float) noexcept;
            virtual void setProgressBar(const gchar*) noexcept;
            virtual void setProgressBarColor(const gchar*) noexcept;

            virtual void connect();
            virtual void query();
            virtual void download();
            virtual void extract();
            virtual void close();

            void pulse();
            void unpulse();

            bool on_click(GdkEventButton*);
            bool on_timeout();

            virtual void save() noexcept;
            virtual bool isEnd() const noexcept;
            virtual void run() noexcept;
        public:
            bool ended;

            App *app;
            tools::Minitar *tar;
            tools::Thread *thread;

            MYSQL mysql;
            Gtk::Separator padding;
            Gtk::ProgressBar progress;
            Gtk::VBox working;
            Gtk::EventBox event;

            std::string dest, ip;
            std::string version;
            std::string sql;

            std::vector<std::string> update;
        protected:
            sigc::connection timeout;
            Glib::Dispatcher dispatcher;
        private:
            GtkCssProvider *provider;
    };
}
