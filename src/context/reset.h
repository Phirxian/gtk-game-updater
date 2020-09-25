#pragma once

#include <gtkmm.h>

namespace context
{
    class Reset : public Gtk::Window
    {
        public:
            Reset();
            ~Reset();

            void logMsg(Gtk::TextView &l, const char *msg) noexcept;

            bool exit(GdkEventAny*);
        public:
            Gtk::Window win;
            Gtk::TextView log;
            bool ended;
    };
}