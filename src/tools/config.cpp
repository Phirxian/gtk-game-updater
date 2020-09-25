#include "config.h"
#include <stdio.h>
#include "../app.h"

#ifdef __WIN32
#   include <windows.h>
#endif

namespace tools
{
    Config::Config(App *a) : app(a)
    {
        peer2peer = auto_start = start_minimized = false;
        systray = minimize_on_close = systray_on_minimized = true;
        download_limit = 0;

        FILE *f = fopen(CFG_PATH, "rb");
        if(f)
        {
            fread(&auto_start, 1, sizeof(auto_start), f);
            fread(&systray, 1, sizeof(systray), f);
            fread(&start_minimized, 1, sizeof(start_minimized), f);
            fread(&minimize_on_close, 1, sizeof(minimize_on_close), f);
            fread(&systray_on_minimized, 1, sizeof(systray_on_minimized), f);
            fread(&peer2peer, 1, sizeof(peer2peer), f);
            fread(&download_limit, 1, sizeof(download_limit), f);
            fread(&close_on_exec, 1, sizeof(close_on_exec), f);
            fclose(f);
        }
        write();
    }
    Config::~Config()
    {
        write();
    }
    void Config::write()
    {
        FILE *f = fopen(CFG_PATH, "wb");
        if(f)
        {
            fwrite(&auto_start, 1, sizeof(auto_start), f);
            fwrite(&systray, 1, sizeof(systray), f);
            fwrite(&start_minimized, 1, sizeof(start_minimized), f);
            fwrite(&minimize_on_close, 1, sizeof(minimize_on_close), f);
            fwrite(&systray_on_minimized, 1, sizeof(systray_on_minimized), f);
            fwrite(&peer2peer, 1, sizeof(peer2peer), f);
            fwrite(&download_limit, 1, sizeof(download_limit), f);
            fwrite(&close_on_exec, 1, sizeof(close_on_exec), f);
            fclose(f);
        }
        apply();
    }
    void Config::apply()
    {
        if(auto_start)
        {
            #ifdef __WIN32
//                HKEY hKey;
//                CString csPath ="your path";
//                HRESULT hres = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hkey);
//                RegSetValueEx(hkey, L"hedenrock launcher", 0, REG_SZ , (BYTE*)csPath.GetBuffer(), (wcslen(csPath)+1)*2);
//                RegCloseKey(hKey);
            #else
            #endif
        }
        else
        {
            #ifdef __WIN32
            #else
            #endif
        }
        /**
         * TODO auto start
         */
        if(systray)
        {
            icon = Gtk::StatusIcon::create_from_file(LOGO_PATH);
            icon->signal_popup_menu().connect(
                [this](guint button, guint activate_time){
                    app->header->setting();
                }
            );
            icon->signal_activate().connect([this]{
                if(app->get_visible())
                {
                    app->iconify();
                    app->hide();
                }
                else
                {
                    app->deiconify();
                    app->show();
                }
            });
        }
        else
        {
            app->show();
            icon.reset();
        }
    }
}
