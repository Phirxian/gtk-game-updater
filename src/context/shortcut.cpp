#include "shortcut.h"
#include "repair.h"
#include "../app.h"

namespace context
{
    Shortcut::Shortcut(App *a) noexcept
        : app(a), vbox(Gtk::ORIENTATION_VERTICAL)
    {
        set_keep_above(true);
        set_modal(true);
        set_default_size(250,75);
        set_title("Repair");
        set_position(Gtk::WIN_POS_CENTER);

        signal_delete_event().connect(
            sigc::mem_fun(this, &Shortcut::exit)
        );

        vbox.set_homogeneous(false);
        add(vbox);

        #define impshortcut(var, name, exec)                       \
            var.set_label(name);                                   \
            var.signal_clicked().connect(                          \
                [this]{ Shortcut::launch(exec); }                  \
            );                                                     \
            vbox.pack_start(var, true, true, 5);                   \

            impshortcut(ed, "editor",  "blicht");
            impshortcut(tr, "terrain", "terrain");
            impshortcut(sr, "server",  "server");

        #undef impshortcut

        realize();
        show_all();
    }
    Shortcut::~Shortcut() noexcept
    {
    }
    bool Shortcut::exit(GdkEventAny*) noexcept
    {
        delete this;
        return false;
    }
    void Shortcut::launch(std::string main) noexcept
    {
        #if defined __WIN32
            main += ".exe";
            //ShellExecute(NULL, NULL, main.c_str(), NULL, "./", SW_SHOWNORMAL);
        #elif defined __linux
            std::string to = "./";
            to += main;
            main = to;
        #else
            printf("url %s is not open because os not supported\n", main);
        #endif
        app->exec(main.c_str());
        delete this;
    }
}
