#pragma once

#include <string>
#include <gtkmm.h>

#include "../context/patcher.h"

struct archive;

namespace context
{
    class Patcher;
}

namespace tools
{
    class Minitar
    {
        public:
            Minitar(context::Patcher*) noexcept;
            void extract(const char *filename, int do_extract, int flags) noexcept;
            int  copy_data(archive *ar, archive *aw) noexcept;

            void msg(const char *m) noexcept;
            void errmsg(const char *m) noexcept;
            void warn(const char *f, const char *m) noexcept;
            void fail(const char *f, const char *m, int r) noexcept;

            void setVerboseDebug(bool) noexcept;
            bool isVerboseDebug() const noexcept;
        protected:
            bool verbose;
            std::string log;
            context::Patcher *parent;
    };
}
