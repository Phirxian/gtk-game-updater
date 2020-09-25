#include "rescue.h"
#include "repair.h"
#include "../app.h"
#include "reset.h"

namespace context
{
    Rescue::Rescue(App *a) noexcept
        : app(a), vbox(Gtk::ORIENTATION_VERTICAL),
          setting("reset all settings"),
          repair("try to repair game")
    {
        set_keep_above(true);
        set_modal(true);
        set_default_size(250,100);
        set_title("Repair");
        set_position(Gtk::WIN_POS_CENTER);

        vbox.set_homogeneous(false);
        add(vbox);

        vbox.pack_start(setting, true, true, 5);
        vbox.pack_start(repair, true, true, 5);

        signal_delete_event().connect(sigc::mem_fun(this, &Rescue::exit));
        setting.signal_clicked().connect(sigc::mem_fun(*this, &Rescue::resetSetting));
        repair.signal_clicked().connect(sigc::mem_fun(*this, &Rescue::tryToRepair));

        realize();
        show_all();
    }
    Rescue::~Rescue() noexcept
    {
    }
    bool Rescue::exit(GdkEventAny*) noexcept
    {
        delete this;
        return false;
    }
    void Rescue::resetSetting() noexcept
    {
        delete this;
        new Reset();
    }
    void Rescue::tryToRepair() noexcept
    {
        auto tmp = app;
        delete this;
        new Repair(tmp);
    }
}
