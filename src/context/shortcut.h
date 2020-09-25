#pragma once

#include <gtkmm.h>
#include <string>

class App;

namespace context
{
    class Shortcut : public Gtk::Window
    {
        public:
            Shortcut(App*) noexcept;
            virtual ~Shortcut() noexcept;
        public:
            bool exit(GdkEventAny*) noexcept;
            void launch(std::string main) noexcept;
            App *app;
        private:
            Gtk::Box vbox;
            Gtk::Button ed, tr, ef, sr;
    };
}
