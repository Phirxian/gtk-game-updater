#pragma once

#include "../config.h"
#include <gtkmm.h>

class App;

namespace tools
{
    class Config
    {
        public:
            Config(App*);
            virtual ~Config();

            void write();
            void apply();

            bool auto_start;

            bool systray;
            bool start_minimized;
            bool minimize_on_close;
            bool systray_on_minimized;
            bool close_on_exec;

            bool peer2peer;
            int download_limit;
        private:
            Glib::RefPtr<Gtk::StatusIcon> icon;
            App *app;
    };
}
