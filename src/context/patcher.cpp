#include "patcher.h"
#include "../app.h"
#include "../tools/thread.h"
#include "../tools/string.h"
#include "../tools/os.h"

#include <iostream>

#ifdef __WIN32
#   include <dirent.h>
#else
#   include <sys/stat.h>
#endif

namespace context
{
    Patcher::Patcher(App *a)
        : app(a), ended(false)
    {
        provider = gtk_css_provider_new();
        progress.get_style_context()->add_provider(Glib::wrap(provider), 999);

        curl_global_init(CURL_GLOBAL_ALL);
        tar = new tools::Minitar(this);

        dispatcher.connect(sigc::mem_fun(*this, &Patcher::close));
        event.signal_button_press_event().connect(sigc::mem_fun(*this, &Patcher::on_click));
        event.set_events(Gdk::BUTTON_PRESS_MASK);
        event.add(progress);

        progress.set_name("update");
        setProgressBarColor("");
        setProgressBar(0.f);

        padding.set_size_request(-1, 5);
        working.pack_start(padding, false, true, 0);
        working.pack_start(event, true, true, 0);
        //progress.set_show_text(true);

        ip = app->getHost();

        thread = new tools::Thread(this);
        thread->start();
    }
    Patcher::~Patcher()
    {
        thread->join();
        delete thread;
        delete tar;
        ended = true;
    }
    bool Patcher::on_click(GdkEventButton *event)
    {
        if(event->type == GDK_2BUTTON_PRESS)
        {
            setProgressBarColor("");
            setProgressBar(0.f);
            thread->start();
            return true;
        }
        return false;
    }
    void Patcher::pulse()
    {
        if(timeout.empty() || !timeout.connected())
            timeout = Glib::signal_timeout().connect(
                sigc::mem_fun(*this, &Patcher::on_timeout), 20
            );
    }
    void Patcher::unpulse()
    {
        timeout.disconnect();
    }
    void Patcher::setProgressBar(float i) noexcept
    {
        progress.set_fraction(i);
    }
    void Patcher::setProgressBar(const gchar *a) noexcept
    {
        printf("%s\n", a);
        progress.set_text(a);
        progress.set_tooltip_text(a);
        logMsg(a);
    }
    void Patcher::setProgressBarColor(const gchar *color) noexcept
    {
        std::string css = "#update progress {background-color: ";
            css += std::string("") == color ? "white" : color;
            css += ";}";
        gtk_css_provider_load_from_data(GTK_CSS_PROVIDER(provider), css.c_str(), css.size(), nullptr);
    }
    void Patcher::logMsg(std::string t) noexcept
    {
        printf("%s\n", t.c_str());
    }
    bool Patcher::isEnd() const noexcept
    {
        return ended;
    }
    void Patcher::save() noexcept
    {
        if(dest != "")
            tools::saveToFile(dest, "update/current");
    }
    void Patcher::connect()
    {
        pulse();
        DIR *folder = opendir("update");

        #ifdef __WIN32
        if(!folder) mkdir("update");
        #else
        if(!folder) mkdir("update", 0766);
        #endif
        else closedir(folder);

        version = "update/current";
        version = tools::loadFromFile(version);

        if(version.find("\n",0) >= 0)
            version = version.substr(0, version.find("\n",0));
        if(version == "") version = "0";

        std::cout << version << std::endl;

        int timeout = 4;
        mysql_init(&mysql);
        //mysql.reconnect = false;
        mysql.options.connect_timeout = timeout;
        mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "option");
        mysql_options(&mysql, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);

        if(mysql_real_connect(&mysql, ip.c_str(), "guest", "", "immortalgalaxy",3306,0,0))
        {
            query();
            mysql_close(&mysql);
            unpulse();

            if(update.size())
                download();
            else
            {
                setProgressBar((gchar*)"nothink to do");
                setProgressBarColor("green");
                setProgressBar(1.f);
            }
        }
        else
        {
            unpulse();
            setProgressBar((gchar*)mysql_error(&mysql));
            setProgressBarColor("red");
            setProgressBar(1.f);
        }
    }
    void Patcher::query()
    {
        logMsg("query updates");
        logMsg(sql);
        int query = mysql_query(&mysql, sql.c_str());

        MYSQL_ROW row;
        MYSQL_RES *result = mysql_use_result(&mysql);

        logMsg("fetch result");
        if(result)
        {
            while(row = mysql_fetch_row(result))
            {
                dest = row[1];
                update.push_back(row[0]);
            }
        }

        mysql_free_result(result);
    }
    void Patcher::download()
    {
        logMsg("download updates");
        for(unsigned int i = 0; i<update.size(); ++i)
        {
            std::string str = "download update ";
                str += i+1;
                str += "/";
                str += update.size();
                str += " ...";
            setProgressBar((gchar*)str.c_str());

            tools::Download item(this, update[i]);
            item.run();
        }

        setProgressBar(1.f);
        extract();
    }
    void Patcher::extract()
    {
        logMsg("extract updates");
        for(int i = 0; i<update.size(); ++i)
        {
            std::string str = "apply update ";
                str += i+1;
                str += "/";
                str += update.size();
                str += " ...";
            setProgressBar((gchar*)str.c_str());

            //ARCHIVE_EXTRACT_TIME = 4

            setProgressBar(0.f);
                std::string to = "update/"; to += update[i].c_str();
                tar->extract(to.c_str(), 1, 4);
            setProgressBar(1.f);
        }

        setProgressBar(1.f);
        setProgressBar((gchar*)"game is up to date");
        setProgressBarColor("green");
    }
    void Patcher::close()
    {
        setProgressBar(1.f);
        tools::Sleeping(120);
        ended = true;
        save();
    }
    bool Patcher::on_timeout()
    {
        progress.pulse();
        return true;
    }
    void Patcher::run() noexcept
    {
        tools::Sleeping(120);
        connect();
        dispatcher.emit();
        thread->join();
    }
}
