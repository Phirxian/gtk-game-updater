#include "reset.h"

//#include "data/core/string.h"
//#include "data/setting/ReadColor.h"
//#include "data/setting/ReadConfig.h"
//#include "data/setting/ReadFont.h"
//#include "data/setting/ReadKey.h"
//#include "data/setting/ReadSkin.h"
//#include <irrlicht/irrlicht.h>

namespace context
{
    Reset::Reset() : ended(false)
    {
        set_keep_above(true);
        set_modal(true);
        set_default_size(250,125);
        set_title("Setting");

        signal_delete_event().connect(
            sigc::mem_fun(this, &Reset::exit)
        );

        log.set_editable(false);
        add(log);

        show_all();
        
        /*
        irr::IrrlichtDevice *io = irr::createDevice(irr::video::EDT_NULL);
        io->getLogger()->setLogLevel(irr::ELL_INFORMATION);
        //io->setEventReceiver(this);

        #define reset(type, fs, file)                  \
        {                                              \
            auto *tmp = new type(fs, file);            \
            tmp->reset();                              \
            tmp->write();                              \
            logMsg(log, "default " file " writed\n");  \
            delete tmp;                                \
        }

        reset(data::setting::ReadConfig, io->getFileSystem(), "data/config.ini");
        reset(data::setting::ReadSkin,   io->getFileSystem(), "data/skin.ini");
        reset(data::setting::ReadFont,   io->getFileSystem(), "data/font.ini");
        reset(data::setting::ReadKey,    io->getFileSystem(), "data/keys.ini");
        reset(data::setting::ReadColor,  io->getFileSystem(), "data/color.ini");
        
        #undef reset

        io->closeDevice();
        ended = true;
        */
    }
    Reset::~Reset()
    {
    }
    void Reset::logMsg(Gtk::TextView &l, const char *msg) noexcept
    {
        auto buffer = l.get_buffer();
        auto iter = buffer->get_iter_at_offset(0);
        buffer->insert(iter, msg);
    }
    bool Reset::exit(GdkEventAny*)
    {
        if(ended)
            delete this;
        return ended;
    }
}
