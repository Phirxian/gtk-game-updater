#include "app.h"
#include "tools/os.h"
#include <unistd.h>

static void loadCss()
{
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(
        screen, GTK_STYLE_PROVIDER (provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    gtk_window_set_default_icon_from_file(LOGO_PATH, nullptr);

    gtk_css_provider_load_from_path(
        GTK_CSS_PROVIDER(provider),
        CSS_PATH, nullptr
    );

    g_object_unref(provider);
}

/**
 * TODO peer2peer
 */

int main(int argc, char *argv[])
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "");

    //! change working directory (needed in other Launcher code source)
    std::string a = tools::getExecutablePath();
    a = a.substr(0, a.rfind("/", a.size()));
    chdir(a.c_str());

    loadCss();

    App core;
    app->hold();
    app->run(core);

    return 0;
}
