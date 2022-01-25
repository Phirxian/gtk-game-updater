#pragma once

#include <gtkmm.h>

class _WebKitWebView;
typedef struct _WebKitWebView WebKitWebView;

class App;
namespace render
{
    class Browser : public Gtk::Widget
    {
        public:
            Browser(App*) noexcept;
            virtual ~Browser() noexcept;
        public:
            WebKitWebView *webkit;
            Gtk::Widget *warp;
            App *app;
    };
}
