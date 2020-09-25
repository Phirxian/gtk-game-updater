#include "tools/os.h"
#include "tools/color.h"
#include "browser.h"
#include "../app.h"

#include <string>
#include <iostream>
#include <cstdio>
#include <memory>
#include <signal.h>
#include <gtk/gtk.h>

#ifdef __WIN32
#include <webkit/webkit.h>
#else
#include <webkit2/webkit2.h>
#endif

namespace render
{
    Console::Console(App *a, const char *cmd) noexcept : app(a)
    {
        set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
        pipe = tools::popen2(cmd, "r", process);
        //pipe = popen(cmd, "r");

        if(!pipe)
        {
            Gtk::MessageDialog dialog(*app, "Error");
            dialog.set_modal(true);
            dialog.set_position(Gtk::WIN_POS_CENTER);
            dialog.run();
            delete this;
        }

        label.set_text(cmd);
        event.add(label);
        close.set_image_from_icon_name("gtk-close");
        box.pack_start(event);
        box.pack_start(close, Gtk::PACK_SHRINK);

        infos.set_editable(false);
        auto buffer = infos.get_buffer();
        buffer->create_mark("last_line", buffer->end(), true);

        static const char* e_color_ref_name[] =
        {
            "black",       "blue",
            "green",       "blue1",
            "brown",       "purple",
            "green4",      "gray39",
            "grey",        "cyan4",
            "green1",      "cyan",
            "red",         "pink",
            "yellow",      "white",
            0
        };

        for(int i = 0; i<tools::COUNT_DO_NOT_USE; ++i)
        {
            std::string bgname("#bg_");
            std::string fgname("#fg_");
            fgname += tools::e_color_ref_name[i];
            bgname += tools::e_color_ref_name[i];

            buffer->create_tag(bgname)->property_background().
                set_value(e_color_ref_name[i]);
            buffer->create_tag(fgname)->property_foreground().
                set_value(e_color_ref_name[i]);
        }

        close.signal_clicked().connect(sigc::mem_fun(*this, &Console::exit));
        dispatcher.connect(sigc::mem_fun(*this, &Console::notify));
        reader = std::thread(&Console::read, this);

        add(infos);
        app->pages.append_page(*this, box);
        box.show_all_children();
        show_all_children();
        show();
    }
    Console::~Console() noexcept
    {
    }
    void Console::notify()
    {
        std::lock_guard<std::mutex> tmp(mutex);
        auto text = infos.get_buffer();
        auto iter = text->end();

        auto line = buffer.front();
        iter = text->insert_with_tags_by_name(iter, line.str, line.color);
        buffer.pop();

        auto mark = text->get_mark("last_line");
        text->move_mark(mark, iter);

        text->place_cursor(text->end());
        infos.scroll_to(mark);

        Glib::RefPtr<Gtk::Adjustment> adj = get_vadjustment();
        adj->set_value(adj->get_upper());
    }
    void Console::read()
    {
        ShelText tmp;
        tmp.color = {"#fg_white", "#bg_black"};
        char buff[128];

        while(!feof(pipe) && pipe)
        {
            if(fgets(buff, 128, pipe) != NULL)
            {
                tmp.str = buff;

                #ifdef __linux
                int pos = tmp.str.find("\033[4");
                if(pos >= 0)
                {
                    int c = tmp.str.find(";3", pos);
                    int end = tmp.str.find('m', c);

                    int bgi = std::atoi(tmp.str.substr(pos, c-pos).c_str());
                    tmp.color[0] = "#bg_";
                    tmp.color[0] += tools::e_color_ref_name[bgi];

                    int fgi = std::atoi(tmp.str.substr(c+2, end-c-2).c_str());
                    tmp.color[1] = "#fg_";
                    tmp.color[1] += tools::e_color_ref_name[fgi];

                    //printf("{%s, %s}\n", tmp.color[0].c_str(), tmp.color[1].c_str());

                    tmp.str = tmp.str.erase(pos , pos-end+1);
                }
                #endif // __linux

                mutex.lock();
                    buffer.push(tmp);
                mutex.unlock();

                dispatcher.emit();
            }
        }

        mutex.lock();
            tmp.str = "\n\n>> done";
            tmp.color = {"#bg_white", "#fg_black"};
            buffer.push(tmp);
        mutex.unlock();

        dispatcher.emit();
    }
    void Console::exit()
    {
        mutex.lock();
            fclose(pipe);
            pipe = nullptr;
        mutex.unlock();

        kill(process, SIGKILL);
        pthread_cancel(reader.native_handle());
//            reader.join();

        App *tmp = app;
        app->pages.remove(*this);
        app->update_noteboook();
    }
}
