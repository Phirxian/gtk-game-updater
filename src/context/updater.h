#pragma once

#include "patcher.h"

namespace context
{
    class Updater : public Patcher
    {
        public:
            Updater(App*) noexcept;
            virtual ~Updater() noexcept;

            void play() noexcept;
            void shortcut() noexcept;
            void rescue() noexcept;
            void exit() noexcept;

            virtual void query() noexcept;
            virtual void close() noexcept;
        private:
            Gtk::Box    hbox;
            Gtk::Button run;
            Gtk::Button quit;
            Gtk::Button dev;
            Gtk::Button setting;
    };
}
