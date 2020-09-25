#pragma once

#include <gtkmm.h>

class App;

namespace context
{
    class Header : public Gtk::HeaderBar
    {
        public:
            using SlotType = Glib::SignalProxy0<void>::SlotType;
        public:
            Header(App *a);
            virtual ~Header();

            Gtk::Button* insert(bool left, const char* icon, const SlotType&);

            void setting() noexcept;
            void about() noexcept;
        protected:
            Gtk::Box left, right;
            Gtk::Menu popup;
            std::vector<Gtk::MenuItem*> items;
            std::vector<Gtk::Button*> tools;
        private:
            App *app;
    };
}