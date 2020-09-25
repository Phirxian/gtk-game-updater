#pragma once

#include <gtkmm.h>

namespace tools
{
    class Config;
}
namespace context
{
    class Settings : public Gtk::Window
    {
        public:
            Settings(tools::Config*);
            virtual ~Settings();

            bool exit(GdkEventAny*);
        protected:
            Gtk::Box vbox;
            Gtk::Button save;

            Gtk::Separator sep[2];

            Gtk::CheckButton auto_start;
            Gtk::CheckButton systray;
            Gtk::CheckButton start_minimized;
            Gtk::CheckButton minimize_on_close;
            Gtk::CheckButton systray_on_minimized;
            Gtk::CheckButton peer2peer;

            Gtk::Label       download_limit_label;
            Gtk::SpinButton  download_limit;
        private:
            Glib::RefPtr<Gtk::Adjustment> digits;
            tools::Config *cfg;
    };
}
