#include "dirent.h"
#include "repair.h"

#include "../app.h"
#include "../tools/minitar.h"

#include <glibmm.h>

namespace context
{
    // TODO
    Repair::Repair(App *a) noexcept
        : Patcher(a), vbox(Gtk::ORIENTATION_VERTICAL)
    {
        sql = "SELECT * from `game_update` WHERE 1";
        tar->setVerboseDebug(true);

        set_keep_above(true);
        set_modal(true);
        set_default_size(500,300);
        set_title("Launcher");

        signal_delete_event().connect(
            sigc::mem_fun(this, &Repair::exit)
        );

        add(vbox);
        scroll.add(log);
        scroll.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
        vbox.pack_start(scroll);
        vbox.pack_start(working, Gtk::PACK_SHRINK);

        log.set_editable(false);

        setProgressBar(0.f);
        setProgressBar((gchar*)"downloading update list");

        realize();
        show_all();
    }
    Repair::~Repair() noexcept
    {
    }
    bool Repair::exit(GdkEventAny*) noexcept
    {
        delete this;
        return false;
    }
    void Repair::logMsg(std::string t) noexcept
    {
        if(!t.size())
            return;

        Patcher::logMsg(t);
        t += "\n";

        auto buffer = log.get_buffer();
        auto iter = buffer->get_iter_at_offset(0);
        buffer->insert(iter, Glib::ustring(t.c_str()));
    }
}
