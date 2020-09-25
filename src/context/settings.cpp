#include "settings.h"
#include "../tools/config.h"

namespace context
{
    Settings::Settings(tools::Config *c)
        : cfg(c), vbox(Gtk::ORIENTATION_VERTICAL),
          auto_start("auto start (win32 only)"),
          systray("systray"),
          start_minimized("start minimized"),
          minimize_on_close("minimize on close"),
          systray_on_minimized("systray on minimized"),
          download_limit_label("download limit (Ko/s), set 0 to disable"),
          peer2peer("peer2peer"),
          save("save & close"),
          digits(Gtk::Adjustment::create(0.0, 0.0, 99999.0, 1.0, 0.0, 0.0))
    {
        set_modal(true);
        set_position(Gtk::WIN_POS_CENTER);
        set_modal(true);
        set_default_size(250,200);

        signal_delete_event().connect(
            sigc::mem_fun(this, &Settings::exit)
        );

        auto_start.set_active(cfg->auto_start);
        systray.set_active(cfg->systray);
        start_minimized.set_active(cfg->start_minimized);
        minimize_on_close.set_active(cfg->minimize_on_close);
        systray_on_minimized.set_active(cfg->systray_on_minimized);

        vbox.add(auto_start);
        vbox.add(systray);
        vbox.add(start_minimized);
        vbox.add(minimize_on_close);
        vbox.add(systray_on_minimized);
        vbox.pack_start(sep[1], true, true, 10);
        vbox.add(download_limit_label);
        vbox.add(download_limit);
        //vbox.add(peer2peer);
        vbox.pack_start(sep[0], true, true, 10);

        download_limit.set_adjustment(digits);
        download_limit.set_digits(cfg->download_limit);

        save.signal_clicked().connect([this](){ exit(0); });
        vbox.pack_end(save, false, true);

        add(vbox);

        realize();
        show_all();
    }
    Settings::~Settings()
    {
        cfg->auto_start = auto_start.get_active();
        cfg->systray = systray.get_active();
        cfg->start_minimized = start_minimized.get_active();
        cfg->minimize_on_close = minimize_on_close.get_active();
        cfg->systray_on_minimized = systray_on_minimized.get_active();
        cfg->download_limit = download_limit.get_digits();
        cfg->peer2peer = peer2peer.get_active();
        cfg->write();
    }
    bool Settings::exit(GdkEventAny*)
    {
        delete this;
        return false;
    }
}
