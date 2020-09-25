#include "app.h"
#include <unistd.h>

App::App() noexcept
    : cfg(new tools::Config(this)),
      vbox(Gtk::ORIENTATION_VERTICAL)
{
    set_default_size(750,400);
    set_title("Launcher");
    set_position(Gtk::WIN_POS_CENTER);
    set_border_width(5);

    signal_delete_event().connect(sigc::mem_fun(this, &App::exit));

    vbox.set_homogeneous(false);
//  vbox.set_spacing(5);
    vbox.pack_start(pages, true, true, 0);
    add(vbox);

    header = new context::Header(this);
    web = new render::Browser(this);
    up = new context::Updater(this);

    if(cfg->start_minimized)
        iconify();

    if(cfg->systray &&
       cfg->systray_on_minimized &&
       cfg->start_minimized)
    {
       hide();
    }
    else
    {
        realize();
        show_all();
    }
    update_noteboook();
}
App::~App() noexcept
{
    delete web;
    delete up;
    delete header;
    delete cfg;
    Gtk::Main::quit();
}
std::string App::getHost() const noexcept
{
    std::string ip;
    WEB_GAME_UPDATE(ip);
    return ip;
}
std::string App::getHostNews() const noexcept
{
    return WEB_GAME_NEWS;
}
void App::update_noteboook()
{
    int count = pages.get_children().size();
    pages.set_show_tabs(count > 1);
    pages.set_current_page(0);
}
void App::exec(const char *cmd)
{
    auto shell = Gtk::manage(new render::Console(this, cmd));
    update_noteboook();
}
bool App::exit(GdkEventAny*)
{
    if(cfg->minimize_on_close)
    {
        if(cfg->systray && cfg->systray_on_minimized)
            hide();
        iconify();
        return true;
    }

    if(up->ended)
    {
        delete this;
        return true;
    }

    return false;
}
