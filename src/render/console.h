#pragma once

#include <gtkmm.h>
#include <thread>
#include <mutex>
#include <queue>

class App;

namespace render
{
    struct ShelText
    {
        Glib::ustring str;
        std::vector<Glib::ustring> color;
    };
    class Console : public Gtk::ScrolledWindow
    {
        public:
            Console(App*, const char*) noexcept;
            virtual ~Console() noexcept;

            void notify();
            void read();

            void exit();
        public:
            Gtk::TextView infos;
            App *app;
        private:
            FILE *pipe;
            int process;
            Gtk::HBox box;
            Gtk::Label label;
            Gtk::EventBox event;
            Gtk::Button close;
            Glib::Dispatcher dispatcher;
            std::queue<ShelText> buffer;
            std::thread reader;
            std::mutex mutex;
    };
}
