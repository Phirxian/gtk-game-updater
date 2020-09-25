#include "header.h"
#include "settings.h"
#include "../tools/os.h"
#include "../app.h"

static void openURL(const char *url, bool local = false)
{
    #if defined __WIN32
        if(local) ShellExecute(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
        else ShellExecute(NULL, NULL, url, NULL, "", SW_SHOWNORMAL);
    #elif defined __linux
        std::string tmp("xdg-open ");
                    tmp += url;
        system(tmp.c_str());
    #else
        #warning unimplemented
    #endif
}

namespace context
{
    Header::Header(App *a)
        : app(a), left(Gtk::ORIENTATION_HORIZONTAL),
          right(Gtk::ORIENTATION_HORIZONTAL)
    {
        app->set_titlebar(*this);
        set_title("Launcher");
        //set_show_close_button(true);
        pack_start(left);
        pack_end(right);

        signal_delete_event().connect(sigc::mem_fun(app, &App::exit));

        // "preferences-system", "gtk-home", "gtk-network", "help-browser"
        insert(true, "asset/gtk/preferences-system.png", sigc::mem_fun(*this, &Header::setting));
        insert(false, "asset/gtk/help-browser.png", sigc::mem_fun(*this, &Header::about));

        static std::pair<const char*, SlotType> entries[] = {
            { "open website",     []{ openURL(WEB_GAME_INDEX); }},
            { "open forum",       []{ openURL(WEB_GAME_FORUM); }},
            { "open game folder", []{
                //! windows fix
                std::string a = tools::getExecutablePath();
                a = a.substr(0, a.rfind("/")+1);
                openURL(a.c_str(), true);
            }},
            { "preference",       [this]{ new Settings(app->cfg); } },
            { "quit",             [this]{ delete app; } },
            { "cancel",           [this]{ } }
        };

        for(auto &it : entries)
        {
            auto item = new Gtk::MenuItem(it.first);
            item->signal_activate().connect(it.second);
            popup.add(*item);
            items.push_back(item);
        }
    }
    Header::~Header()
    {
        for(auto &it : tools)
            delete it;
        for(auto &it : items)
            delete it;
    }
    Gtk::Button* Header::insert(bool l, const char* icon, const SlotType &slot)
    {
        Gtk::Button *button = Gtk::manage(new Gtk::Button());
        Gtk::Image *image = Gtk::manage(new Gtk::Image());
            //image->set_from_icon_name(icon, Gtk::ICON_SIZE_BUTTON);
            image->set(icon);
            image->show();
        button->add(*image);

        if(l) left.add(*button);
        else  right.add(*button);

        tools.push_back(button);

        button->signal_clicked().connect(slot);
        button->show_all_children();
        return button;
    }
    void Header::setting() noexcept
    {
        popup.show_all();
        popup.popup(0, gtk_get_current_event_time());
    }
    void Header::about() noexcept
    {
        Gtk::AboutDialog dialog;
            dialog.set_title("About Launcher");
            dialog.set_program_name("Launcher v2.2");
            dialog.set_version(g_strdup_printf(
                "\nRunning against GTK+ %d.%d.%d",
                gtk_get_major_version (),
                gtk_get_minor_version (),
                gtk_get_micro_version ()
            ));
            dialog.set_copyright("(C) 2005-2016 Jehan-Antoine Vayssade");
            dialog.set_website(WEB_GAME_INDEX);
            dialog.set_comments("Program to update the game data");
        dialog.run();
    }
}
