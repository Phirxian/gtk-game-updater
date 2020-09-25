#include "dirent.h"
#include "updater.h"

#include "../app.h"
#include "../tools/os.h"
#include "rescue.h"
#include "shortcut.h"

namespace context
{
    Updater::Updater(App *a) noexcept
        : Patcher(a), hbox(Gtk::ORIENTATION_HORIZONTAL),
          run("Start"), quit("Close"),
          dev(" dev "), setting("  R  ")
    {
        dispatcher.connect(sigc::mem_fun(*this, &Updater::close));
        app->vbox.pack_start(working, false, true, 0);
        setProgressBar(0.f);
        setProgressBar((gchar*)"checking for update ...");

        hbox.set_homogeneous(false);
        hbox.pack_start(run, true, true, 0);
        hbox.pack_start(quit, true, true, 0);
        hbox.pack_start(dev, false, true, 0);
        hbox.pack_start(setting, false, true, 0);

        run.signal_clicked().connect(sigc::mem_fun(*this, &Updater::play));
        quit.signal_clicked().connect(sigc::mem_fun(*this, &Updater::exit));
        dev.signal_clicked().connect(sigc::mem_fun(*this, &Updater::shortcut));
        setting.signal_clicked().connect(sigc::mem_fun(*this, &Updater::rescue));
    }
    Updater::~Updater() noexcept
    {
    }
    void Updater::play() noexcept
    {
        std::string a = tools::getExecutablePath();
            a = a.substr(0, a.rfind("/"));

        if(ended)
        {
            std::string to;
            #if defined __WIN32
                to = "game.exe";
                //ShellExecute(NULL, NULL,"game.exe", NULL, a.c_str(), SW_SHOWNORMAL);
            #elif defined __linux
                to += "./game&";
            #else
                printf("url %s is not open because os not supported\n", to.c_str());
            #endif

            app->exec(to.c_str());
            //TODO single instance
        }
        else
        {
            Gtk::MessageDialog dialog(*app, "Game not ready, wait update");
            dialog.set_modal(true);
            dialog.set_position(Gtk::WIN_POS_CENTER);
            dialog.run();
        }
    }
    void Updater::rescue() noexcept
    {
        Gtk::manage(new Rescue(app));
    }
    void Updater::shortcut() noexcept
    {
        Gtk::manage(new Shortcut(app));
    }
    void Updater::exit() noexcept
    {
        app->exit(0);
    }
    void Updater::query() noexcept
    {
        sql = "SELECT * from `game_update` WHERE `version` > '";
        sql += version;
        sql += "'";
        Patcher::query();
    }
    void Updater::close() noexcept
    {
        app->vbox.pack_start(hbox, false, true, 0);
        app->vbox.show_all();
        app->vbox.queue_draw();
        Patcher::close();
    }
}
