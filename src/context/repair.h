#pragma once

#include "../tools/download.h"

namespace context
{
    class Repair : public Patcher, public Gtk::Window
    {
        public:
            Repair(App*) noexcept;
            virtual ~Repair() noexcept;

            virtual void logMsg(std::string) noexcept;

            bool exit(GdkEventAny*) noexcept;
        public:
            int state, current;
            bool ended;
        private:
            Gtk::Box vbox;
            Gtk::TextView log;
            Gtk::ScrolledWindow scroll;
    };
}
