#pragma once

#include "render/console.h"
#include "render/browser.h"
#include "context/updater.h"
#include "context/header.h"
#include "tools/config.h"
#include "config.h"

#include <vector>
#include <utility>

class App : public Gtk::Window
{
    public:
        App() noexcept;
        virtual ~App() noexcept;

        std::string getHost() const noexcept;
        std::string getHostNews() const noexcept;

        void update_noteboook();
        void exec(const char*);

        virtual bool exit(GdkEventAny*);
    public:
        context::Header  *header;
        context::Updater *up;
        tools::Config    *cfg;

        Gtk::Notebook pages;
        Gtk::Box vbox;
    private:
        render::Browser *web;
};
