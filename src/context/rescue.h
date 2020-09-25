#pragma once

#include <gtkmm.h>

class App;

namespace context
{
    class Rescue : public Gtk::Window
    {
        public:
            Rescue(App*) noexcept;
            virtual ~Rescue() noexcept;

            void resetSetting() noexcept;
            void tryToRepair() noexcept;

            bool exit(GdkEventAny*) noexcept;
        public:
            App *app;
            Gtk::Box vbox;
            Gtk::Button setting, repair;
    };
}
