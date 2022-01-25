#include "browser.h"
#include "../app.h"

#include <gtk/gtk.h>

#ifdef __WIN32
#include <webkit/webkit.h>
#else
#include <webkit2/webkit2.h>
#endif

namespace render
{
    Browser::Browser(App *a) noexcept : app(a), Gtk::Widget(webkit_web_view_new())
    {
        webkit = WEBKIT_WEB_VIEW(gobj());

        WebKitSettings *settings = webkit_settings_new();
        webkit_web_view_set_settings(WEBKIT_WEB_VIEW(webkit), settings);

        webkit_web_view_load_uri(webkit, app->getHostNews().c_str());
        gtk_widget_grab_focus(GTK_WIDGET(webkit));

        app->pages.append_page(*this, "news");
    }
    Browser::~Browser() noexcept
    {
        gtk_widget_destroy(GTK_WIDGET(webkit));
    }
}
